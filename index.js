var Komachan = require('./lib/komachan');
var komachan = require('bindings')('komachan');
var path = require('path');
module.exports = {
  newKomachan: function(opt){
    opt = opt || {};
    opt.binPath = opt.binPath || path.join(__dirname, '/setting');
    return komachan.newKomachan(opt);
  }
}
