var Komachan = require('../index');
var obj = Komachan.newKomachan();

obj.start();
obj.print();
obj.move({
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
obj.print();
