var spawn = require('child_process').spawn,
    path = require('path'),
    util = require('./util');

function Komachan(){
  var self = this;
  var bin_path = path.join(__dirname, '../BB_Attack.bin');
  var koma_path = path.join(__dirname, '../bin/komachan');
  self.koma = spawn(koma_path, ['--server-mode', '--bin-path='+bin_path]);
  self.koma.on('data', function(data){
  });
}

Komachan.prototype.move = function(move, cb){
  var self = this;
  var moveCSA = 'move ' + util.ObjectToCSA(move) + '\n';
  self.koma.stdin.write(moveCSA);
  self.koma.stdout.once('data', function(data){
    cb(util.CSAtoObject(data.toString()));
  });
}

Komachan.prototype.search = function(cb){
  var self = this;
  var search = 'search\n';
  self.koma.stdin.write(search);
  self.koma.stdout.once('data', function(data){
    cb(util.CSAtoObject(data.toString()));
  });
}

Komachan.prototype.end = function(cb){
  this.koma.kill();
}

module.exports = Komachan;
