var spawn = require('child_process').spawn,
    util = require('./util');

function Komachan(){
  var self = this;
  self.koma = spawn('./bin/Komachan', ['--no-logfile', '--server-mode']);
}

Komachan.prototype.move = function(move, cb){
  var self = this;
  var moveCSA = 'move ' + util.ObjectToCSA(move) + '\n';
  self.koma.stdin.write(moveCSA);
  self.koma.stdout.once('data', function(data){
    cb(data.toString());
  });
}

Komachan.prototype.end = function(cb){
  this.koma.kill();
}

module.exports = Komachan;
