var io = require('socket.io-client');

var socket = io.connect('http://komachan.co', {
  'reconnect': true,
  'timeout': 500000,
  'reconnection delay': 500,
  'max reconnection attempts': 10
});

exports.socket = function() {
  return {
    socket: socket
  };
}
