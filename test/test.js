<<<<<<< HEAD
var Komachan = require('../index');
var obj = Komachan.newKomachan();

obj.start();
obj.print();
obj.move({
  from: {
    x: 3,
    y: 5,
=======
var util = require('../lib/util');
var Komachan = require('../lib/komachan');
var co = require('co');

var koma1 = new Komachan({debug: true});
var moveObj = {
  'from':{
    'x':3,
    'y':5
>>>>>>> b864d52d0680290018cd2cddd2d6945195d8c9a9
  },
  to: {
    x: 3,
    y: 4,
  },
<<<<<<< HEAD
  piece: 'GI'
=======
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
>>>>>>> b864d52d0680290018cd2cddd2d6945195d8c9a9
});
obj.print();
