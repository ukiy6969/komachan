var Komachan = require('../index');
var obj1 = Komachan.newKomachan();
var obj2 = Komachan.newKomachan();
var zobrist = require('../zobrist.json');
obj1.start({
  hash: zobrist
});
//console.log(zobrist);
var zobrist = obj1.getZobrist();
console.log(JSON.stringify(obj1.getZobrist()));
