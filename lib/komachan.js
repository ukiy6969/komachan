var spawn = require('child_process').spawn,
    path = require('path'),
    split = require('split'),
    util = require('./util');

function Komachan(opt){
  var self = this;
  var bin_path = path.join(__dirname, '../BB_Attack.bin');
  var zobrist_path = path.join(__dirname,'../Zobrist_rand.bin');
  var koma_path = path.join(__dirname, '../komachan');
  self.koma = spawn(koma_path, ['--server-mode', '--bin-path='+bin_path, '--zobrist-path='+zobrist_path]);
  if (opt.debug) {
    self.koma.stdout.on('data', function(data){
      console.log('komastdout', data.toString());
    });
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
}

Komachan.prototype.move = function(move, cb){
  var self = this;
  var moveCSA = 'move ' + util.ObjectToCSA(move) + '\n';
  var splitline = split();
  return new Promise(function(resolve, reject){
    self.koma.stdin.write(moveCSA);
    self.koma.stdout.pipe(splitline).once('data', function(data){
      var moveStr = data.toString();
      if ( moveStr.indexOf('ERROR') > -1 ) {
        throw new Error(moveStr, null)
        return cb(new Error(moveStr, null));
      }
      cb(null, util.CSAtoObject(data.toString()));
      self.koma.stdout.unpipe(splitline);
      resolve(util.CSAtoObject(data.toString()));
    });
  });
}

Komachan.prototype.search = function(cb){
  var self = this;
  var search = 'search\n';
  var splitline = split();
  return new Promise(function(resolve, reject){
    self.koma.stdin.write(search);
    self.koma.stdout.pipe(splitline).once('data', function(data){
      if ( data.toString().indexOf('ERROR') > -1 ) {
        throw new Error(data.toString());
        return cb(new Error(data.toString()), null);
      }
      cb(null, util.CSAtoObject(data.toString()));
      self.koma.stdout.unpipe(splitline);
      resolve(util.CSAtoObject(data.toString()));
    });
  });
}

Komachan.prototype.legal = function(cb){
  var self = this;
  var legal = 'legal\n';
  var splitline = split();
  var legalStream;
  return new Promise(function(resolve, reject){
    self.koma.stdin.write(legal);
    self.koma.stdout.pipe(splitline).once('data', function(data){
      if ( data.toString().indexOf('ERROR') > -1 ) {
        throw new Error(data.toString())
        return cb(new Error(data.toString()), null);
      }
      cb(null, util.LegaltoObject(data.toString()));
      self.koma.stdout.unpipe(splitline);
      console.log(data.toString());
      resolve(util.LegaltoObject(data.toString()));
    });
  });
}

Komachan.prototype.end = function(cb){
  this.koma.kill();
}

module.exports = Komachan;
