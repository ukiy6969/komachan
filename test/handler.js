var handler = require('../lib/komachanhandler');
var zobrist = require('../zobrist.json');
var komaHandler = new handler();

komaHandler.start({
  useTpt: 1,
  searchDepth: 7,
  hash: zobrist
});
komaHandler.finish(function(mess){
  console.log(mess);
  process.exit();
});

process.on('exit', function(){
  komaHandler.end();
});

process.on('SIGTSTP', function() {
  console.log('Got SIGTSTP.');
});
