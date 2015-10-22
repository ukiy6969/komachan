var util = require('../lib/util');
var Komachan = require('../lib/komachan');
var co = require('co');

var koma1 = new Komachan({debug: true});
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
co(function*() {
  var legmove = yield koma1.legal();
  console.log()
})
//koma1.move(moveObj, function(err, data){
//  console.log(data);
//});
/*
koma1.move('search\n', function(data){
  console.log(data);
});
*/

process.on('close', function(){
  console.log('end');
  koma1.end();
});
