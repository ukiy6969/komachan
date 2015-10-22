var Komachan = require('./lib/komachan');
module.exports = {
  komachan: function(opt){
    return new Komachan(opt);
  }
}
