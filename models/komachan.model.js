var mongoose = require('mongoose');


var KomachanSchema = mongoose.Schema({
  useTpt: Number,
  searchDepth: Number,
  hash: {
    pieceInfoRand: mongoose.Schema.Types.Mixed,
    handInfoRand: mongoose.Schema.Types.Mixed,
    turnRand: [String]
  },
  tpt: [{
    hash: String,
    evals: {
      depth: Number,
      eval: Number
    }
  }]
});
module.exports = mongoose.model('KomachanModel', KomachanSchema);
