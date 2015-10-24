var Komachan = require('../index');
var path = require('path');
var obj = Komachan.newKomachan({
  binPath: path.join(__dirname, '../setting')
});

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
