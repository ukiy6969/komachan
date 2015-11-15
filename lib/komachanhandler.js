var Komachan = require('../index.js');
var util = require('util');

function KomachanHandler() {
  var self = this;
  self.color;
  self.komachan = Komachan.newKomachan();
  self.socket = require('./socket').socket().socket;
  self.socket.on('legal', function(lmoves){
    //console.log('legal', lmoves);
  })
  self.socket.on('win', function(mes){
    self.komachan.end();
  });
  self.socket.on('lose', function(mes) {
    self.komachan.end();
  });
  self.socket.on('err', function(err){
    self.komachan.end();
  });
}

KomachanHandler.prototype.getKomachan = function() {
  return this.komachan;
}

KomachanHandler.prototype.start = function(opt) {
  var self = this;
  self.komachan.start(opt);
  self.socket.emit('new-game', {});
  self.socket.on('confirm', function(game) {
    if (opt.color == 0) {
      game.header.firstMove = 'client';
      game.header.secondMove = 'koma';
    }
    if (opt.color == 1) {
      game.header.firstMove = 'koma';
      game.header.secondMove = 'client';
    }
    self.socket.emit('agree', game);
  });
  self.socket.on('start', function(game) {
    if (game.header.firstMove === 'client') {
      self.color = 0;
    } else {
      self.color = 1;
    }
  });
  self.move();
  self.moved();
}

KomachanHandler.prototype.manualStart = function(opt) {
  var self = this;
  self.komachan.start(opt);
}

KomachanHandler.prototype.move = function() {
  var self = this;
  self.socket.on('legal', function(legal) {
    var kmove = self.komachan.search();
    if (!kmove) {
      return;
    }
    self.socket.emit('move', kmove);
    self.komachan.print();
  });
}

KomachanHandler.prototype.moved = function() {
  var self = this;
  self.socket.on('moved', function(move){
    if (move.color !== self.color){
      self.komachan.move(move);
      self.komachan.print();
    }
  });
}

KomachanHandler.prototype.manualMove = function(move) {
    var self = this;
    self.komachan.move(move);
}

KomachanHandler.prototype.manualBack = function() {
    var self = this;
    self.komachan.back();
}

KomachanHandler.prototype.print = function() {
  var self = this;
  self.komachan.print();
}

KomachanHandler.prototype.end = function() {
  this.komachan.end();
}

KomachanHandler.prototype.finish = function() {
  var self = this;
  return new Promise(function(resolve){
    self.socket.on('win', function(wgame){
      resolve(wgame);
    });
    self.socket.on('lose', function(lgame){
      resolve(lgame);
    });
  });
}


module.exports = KomachanHandler;
