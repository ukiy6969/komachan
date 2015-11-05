var Komachan = require('../index');
var obj1 = Komachan.newKomachan();
var obj2 = Komachan.newKomachan();
obj1.start();
var zobrist = obj1.getZobrist();
obj2.start({
  hash : zobrist
});
