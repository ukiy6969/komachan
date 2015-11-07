var mongoose = require('mongoose');

var komaEnum = ['OU','KI','GI','KA','HI','FU','TO','NG','UM','RY'];

var gogoShogiSchema = mongoose.Schema({
  startDate : Date,
  endDate : Date,
  header: {
    firstMove: String,
    secondMove: String,
  },
  moves: [
    {
      color: { type: Number, enum: [0, 1] },
      from: {
        x: { type: Number, min: 0, max: 5 },
        y: { type: Number, min: 0, max: 5}
      },
      to: {
        x: { type: Number, min: 0, max: 5 },
        y: { type: Number, min: 0, max: 5 }
      },
      piece: { type: String, enum: komaEnum },
      promote: { type: Boolean },
      capture: { type: String },
      isAttack: { type: Boolean },
      searchTime: { type: Number }
    }
  ],
  conclude: {
    winner: { type: Number, enum: [0, 1] },
    loser: { type: Number, enum: [0, 1] }
  }
});
module.exports = mongoose.model('GogoShogi', gogoShogiSchema);
