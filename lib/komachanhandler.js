var Komachan = require('./komachan');

function KomachanHandler() {
  var self = this;
  self.color;
  self.komachan = new Komachan();
  self.socket = require('./socket').socket().socket;
  self.moved();
  self.socket.on('win', function(mes){
    console.log(mes);
  });
  self.socket.on('lose', function(mes) {
    console.log(mes);
  });
  self.socket.on('err', function(err){
    console.log(err);
  });
}

KomachanHandler.prototype.start = function() {
  var self = this;
  self.socket.emit('new-game', {});
  self.socket.on('confirm', function(game) {
    self.socket.emit('agree', game);
  });
  self.socket.on('start', function(game) {
    console.log(game);
    if (game.header.firstMove === 'client') {
      self.color = 0;
      self.move();
    } else {
      self.color = 1;
    }
  });
}

KomachanHandler.prototype.move = function() {
  var self = this;
  self.komachan.search(function(err, move){
    if(err) {
      console.log(err);
      self.finish();
      return;
    }
    self.socket.emit('move', move);
  });
}

KomachanHandler.prototype.moved = function() {
  var self = this;
  self.socket.on('moved', function(move){
    console.log('moved', move);
    //if (move.lose) { return console.log('You are lose.')}
    if (move.color !== self.color){
      self.komachan.move(move, function(err, moved) {
        if (err) { return console.log(err); }
        self.move();
      });
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
