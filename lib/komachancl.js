#! /usr/bin/env node

var handler = require('../lib/komachanhandler');
var zobrist = require('../zobrist.json');
var komaUtil = require('./util');
var mongoose = require('mongoose');
var readline = require('gocsp-readline');
var co = require('co');
var GogoshogiModel = require('../models/gogoshogi-game');
var KomachanModel = require('../models/komachan.model');

var program = require('commander');

var result;
var cmdValue;

program
  .version('0.0.12')
  .arguments('<cmd> [env]')
  .action(function(cmd, env){
    cmdValue = cmd;
  });

program.parse(process.argv);

if (typeof cmdValue === 'undefined') {
  console.error('no command given!');
}

if (cmdValue == 'remote') {
  gameRemote();
}

if (cmdValue == 'local') {
  gameLocal();
}

function gameLocal() {
  var koma = require('../index').newKomachan();
  co(function* (){
    try {
      mongoose.connect('mongodb://localhost/komachan');
    }catch(e) {
      console.log("could not connect mongodb");
      process.exit(1);
    }
    var startOpt = {};
    var answer = yield readline.question('Use tpt? (yes or no): ');
    if (answer === 'yes') {
      startOpt.useTpt = 1;
    }else {
      startOpt.useTpt = 0;
    }
    var sd = yield readline.question('Search depth (number): ');
    startOpt.searchDepth = sd;
    var color = yield readline.question('First or Second? (0 or 1): ');
    startOpt.color = color;
    return startOpt;
  })
  .then(function(opt){
    koma.start(opt);
    console.log('Game start!!');
    koma.print();
    return koma;
  })
  .then(function(koma){
    return co(function*() {
      while(1) {
        var command = yield readline.question('please command (next): ');
        if (command == 'search' || command == 's') {
          try {
            koma.search();
            koma.print();
          } catch(e) {
            console.log(e);
            continue;
          }
        }
        if (command == 'quit' || command == 'q') {
          koma.end();
          process.exit(0);
        }
        if (command.split(' ')[0] === 'move' ) {
          try {
            koma.move(komaUtil.csaToMove(command.split(' ')[1]));
            koma.print();
          } catch(e) {
            console.log(e);
            continue;
          }
        }
        if (command == 'back' || command == 'b') {
          koma.back();
          koma.print();
        }
      }
    })
    .catch(function(err){
      console.log('err', err);
    });
  });

}

function gameRemote() {
  var komaHandler = new handler();
  co(function* (){
    try {
      mongoose.connect('mongodb://localhost/komachan');
    }catch(e) {
      console.log("could not connect mongodb");
      process.exit(1);
    }
    var startOpt = {};
    var answer = yield readline.question('Use tpt? (yes or no): ');
    if (answer === 'yes') {
      startOpt.useTpt = 1;
    }else {
      startOpt.useTpt = 0;
    }
    var sd = yield readline.question('Search depth (number): ');
    startOpt.searchDepth = sd;
    var color = yield readline.question('First or Second? (0 or 1): ');
    startOpt.color = color;
    return startOpt;
  })
  .then(function(opt){
    komaHandler.start(opt);
    console.log('Game start!!');
    return komaHandler.finish();
  })
  .then(function(message){
    console.log('Game end!!');
    result = message.message;
    komaHandler.end();
    var game = new GogoshogiModel(message.game);
    return game.save();
  })
  .then(function(endGame){
    komaHandler.manualStart();
    co(function* (){
      var p = 0;
      komaHandler.print();
      while(1) {
        var command = yield readline.question('please command (next): ');
        switch (command) {
          case 'n':
          case 'next':
            if (p >= 0 && p < endGame.moves.length) {
              var move = endGame.moves[p++];
              komaHandler.manualMove(move);
              komaHandler.print();
              console.log('isAttack: '+move.isAttack);
              console.log('searchTime: '+move.searchTime);
              console.log('move', move);
            }
            break;
          case 'b':
          case 'back':
            if (p >= 0 && p < endGame.moves.length) {
              var move = endGame.moves[--p];
              komaHandler.manualBack();
              komaHandler.print();
              console.log('isAttack: '+move.isAttack);
              console.log('searchTime: '+move.searchTime);
              console.log('move', move);
            }
            break;
          case 'result':
            console.log('result', result);
            break;
          case 'header':
            console.log('header', endGame.header);
            break;
          default:
            console.log('finish');
            process.exit(0);
        }
      }
    })
    .catch(function(err){
      console.log(err);
    });
  });
}



process.on('exit', function(){
});

process.on('SIGTSTP', function() {
  console.log('Got SIGTSTP.');
  process.exit(0);
});

process.on('SIGINT', function(){
  console.log('Got SIGINT');
  process.exit(0);
});

process.on('SIGHUP', function() {
  console.log('Got SIGHUP');
  process.exit(0);
});

process.on('SIGTERM', function(){
  console.log('Got SIGTERM');
  process.exit(0);
});
