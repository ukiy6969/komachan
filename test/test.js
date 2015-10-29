var Komachan = require('../index');
var path = require('path');
var obj1 = Komachan.newKomachan();
var obj2 = Komachan.newKomachan();

obj1.start();
obj2.start();
obj1.print();
obj2.print();
console.log(obj1.legal());
var move1 = obj1.move({
  from: {
    x: 3,
    y: 5,
  },
  to: {
    x: 3,
    y: 4,
  },
  piece: 'GI'
});
console.log(move1);

obj2.move({
  from: {
    x: 3,
    y: 5,
  },
  to: {
    x: 3,
    y: 4,
  },
  piece: 'GI'
});

obj1.print();
obj2.print();
var move12 = obj1.search();
console.log(move12);
obj1.print();
