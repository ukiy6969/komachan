var handler = require('../lib/komachanhandler');
var zobrist = require('../zobrist.json');
var mongoose = require('mongoose');
var GogoshogiModel = require('../models/gogoshogi-game');
var KomachanModel = require('../models/komachan.model');
mongoose.connect('mongodb://localhost/komachan');
var komaHandler = new handler();

var koma = new KomachanModel({
  useTpt: 1,
  searchDepth: 6,
  hash: zobrist
});
koma.save()
.then(function(komachan) {
  console.log(komachan);
  komaHandler.start(komachan);
  komaHandler.finish(function(mess){
    var game = GogoshogiModel(mess.game);
    game.save()
    .then(function(game){
      console.log("finish!");
    })
    .then(function(){
      console.log('hoge');
      var tpt = komaHandler.getKomachan().getTpt();
      console.log(tpt);
      //koma.save();
    });
  });
});


process.on('exit', function(){
  komaHandler.end();
});

process.on('SIGTSTP', function() {
  console.log('Got SIGTSTP.');
});
