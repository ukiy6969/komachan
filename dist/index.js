var client, game, io, koma, spawn;

spawn = require('child_process').spawn;

client = require('socket.io-client');

io = client.connect('http://localhost:3000');

koma = spawn('./komachan/bin/komachan', ['--server-mode']);

game = null;

io.emit('komachan', {});

io.on('confirm', function(data) {
  game = data;
  return io.emit('agree', {
    user: 'komachan'
  });
});

io.on('start', (function(_this) {
  return function(data) {
    console.log('start', game);
    if (game.header.firstMove === 'koma') {
      console.log('write');
      return koma.stdin.write('search\n');
    }
  };
})(this));

koma.stdout.on('data', function(data) {
  return console.log('koma', data.toString());
});
