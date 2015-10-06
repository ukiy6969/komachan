var io = require('socket.io-client');

var socket = io.connect('http://komachan.co');

exports.socket = function() {
  return {
    socket: socket
  };
}
