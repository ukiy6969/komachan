var spawn = require('child_process').spawn,
    path = require('path'),
    util = require('./util');

function Komachan(){
  var self = this;
  var bin_path = path.join(__dirname, '../BB_Attack.bin');
  self.koma = spawn('./bin/komachan', ['--server-mode', '--bin-path='+bin_path]);
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
