var spawn = require('child_process').spawn,
    path = require('path'),
    util = require('./util');

function Komachan(){
  var self = this;
  var bin_path = path.join(__dirname, '../BB_Attack.bin');
  var koma_path = path.join(__dirname, '../komachan');
  self.koma = spawn(koma_path, ['--server-mode', '--bin-path='+bin_path]);
  self.koma.stdout.on('error', function(data){
    console.log('komastdout', data.toString());
  });
  self.koma.stdout.on('exit', function(data){
    console.log('komastdout', data.toString());
  });
  self.koma.stdout.on('disconnect', function(data){
    console.log('komastdout', data.toString());
  });
}

Komachan.prototype.move = function(move, cb){
  var self = this;
  var moveCSA = 'move ' + util.ObjectToCSA(move) + '\n';
  self.koma.stdin.write(moveCSA);
  self.koma.stdout.once('data', function(data){
    if ( data.toString().indexOf('ERROR') > -1 ) {
      cb(new Error(data.toString()), null);
      return
    }
    cb(null, util.CSAtoObject(data.toString()));
  });
}

Komachan.prototype.search = function(cb){
  var self = this;
  var search = 'search\n';
  self.koma.stdin.write(search);
  self.koma.stdout.once('data', function(data){
    if ( data.toString().indexOf('ERROR') > -1 ) {
      cb(new Error(data.toString()), null);
      return
    }
    cb(null, util.CSAtoObject(data.toString()));
  });
}

Komachan.prototype.end = function(cb){
  this.koma.kill();
}

module.exports = Komachan;
