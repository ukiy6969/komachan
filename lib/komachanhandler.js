var Komachan = require('../index.js');

function KomachanHandler() {
  var self = this;
  self.color;
  self.komachan = Komachan.newKomachan();
  self.socket = require('./socket').socket().socket;
  self.moved();
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
    self.socket.emit('agree', game);
  });
  self.socket.on('start', function(game) {
    self.komachan.print();
    if (game.header.firstMove === 'client') {
      self.color = 0;
      self.move();
    } else {
      self.color = 1;
    }
    self.komachan.print();
  });
}

KomachanHandler.prototype.move = function() {
  var self = this;
  var kmove = self.komachan.search();
  if (!kmove) {
    return;
  }
  self.socket.emit('move', kmove);
}

KomachanHandler.prototype.moved = function() {
  var self = this;
  self.socket.on('moved', function(move){
    if (move.color !== self.color){
      self.komachan.move(move);
      self.komachan.print();
      self.move();
      self.komachan.print();
    }
  });
}

KomachanHandler.prototype.end = function() {
  this.komachan.end();
}

KomachanHandler.prototype.finish = function(listener) {
  this.socket.on('win', listener);
  this.socket.on('lose', listener);
}


module.exports = KomachanHandler;
