var handler = require('../lib/komachanhandler');
var zobrist = require('../zobrist.json');
var mongoose = require('mongoose');
var readline = require('gocsp-readline');
var co = require('co');
var GogoshogiModel = require('../models/gogoshogi-game');
var KomachanModel = require('../models/komachan.model');

var komaHandler = new handler();
var result;
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
    startOpt.hash = zobrist;
  }else {
    startOpt.useTpt = 0;
  }
  var sd = yield readline.question('Search depth (number): ');
  startOpt.searchDepth = sd;
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




process.on('exit', function(){
  komaHandler.end();
});

process.on('SIGTSTP', function() {
  console.log('Got SIGTSTP.');
});
