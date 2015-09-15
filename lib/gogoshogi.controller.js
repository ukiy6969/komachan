var game = require('../build/Release/gogoshogi'),
    util = require('./util'),
    reload = require('require-reload')(require);

function GogoShogiCtr(){
  var self = this;
  self.game = game;
  self.game.start();
}

GogoShogiCtr.prototype.search = function (){
  this.game.search();
}

GogoShogiCtr.prototype.board = function() {
  this.game.board();
}

module.exports = GogoShogiCtr;
