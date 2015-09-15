var util = require('../lib/util');
var ctrl = require('../lib/gogoshogi.controller');
var Komachan = require('../lib/komachan');

var koma1 = new Komachan();
var moveObj = {
  'from':{
    'x':3,
    'y':5
  },
  'to':{
    'x':3,
    'y':4
  },
  piece:'GI'
};
koma1.move(moveObj, function(data){
  console.log(data);
});
/*
koma1.move('search\n', function(data){
  console.log(data);
});
*/

process.on('close', function(){
  console.log('end');
  koma1.end();
});
