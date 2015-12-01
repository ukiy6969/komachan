var Komachan = require('../index.js');
var util = require('util');

function KomachanHandler() {
  var self = this;
  self.color;
  self.komachan = Komachan.newKomachan();
  self.socket = require('./socket').socket().socket;
  self.socket.heartbeatTimeout = 500000;
  self.latestMove;
  self.gameId;
  self.socket.on('disconnect', function(){
    console.log('disconnected');
  });
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
  self.socket.on('reconnect', function(){
    console.log('reconnect');
    self.socket.emit('restart', self.gameId);
    self.socket.on('restarted', function(id){
      console.log('id', id);
      self.socket.emit('move', self.latestMove);
    });
  });
  self.socket.on('connected', function(data) {
    console.log(data);
  });
}

KomachanHandler.prototype.connecting = function() {
  var self = this;
  self.connectTimer = setInterval(function(){
    console.log('connect');
    self.socket.emit('connect', {});
  }, 1000);
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
    if (opt.komachanOpt) {
      game.komachanOpt = opt.komachanOpt;
    }
    self.socket.emit('agree', game);
  });
  self.socket.on('start', function(game) {
    self.gameId = game._id;
    if (game.header.firstMove === 'client') {
      self.color = 0;
    } else {
      self.color = 1;
    }
  });
  self.move();
  self.moved();
  self.connecting();
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
    self.latestMove = kmove;
    self.komachan.back();
    self.socket.emit('move', kmove);
    console.log('search', kmove);
    self.komachan.print();
  });
}

KomachanHandler.prototype.moved = function() {
  var self = this;
  self.socket.on('moved', function(move){
    //if (move.color !== self.color){
      self.komachan.move(move);
      self.komachan.print();
    //}
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
      clearInterval(self.connectTimer);
    });
    self.socket.on('lose', function(lgame){
      resolve(lgame);
      clearInterval(self.connectTimer);
    });
  });
}


module.exports = KomachanHandler;
