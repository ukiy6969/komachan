var _ = require('lodash');

exports.CSAtoObject = function(moveStr){
  try {
    move = {};
    move.from = {};
    move.to = {};
    move.color = parseInt(moveStr.slice(0,1));
    move.from.x = parseInt(moveStr.slice(1,2));
    move.from.y = parseInt(moveStr.slice(2,3));
    move.to.x = parseInt(moveStr.slice(3,4));
    move.to.y = parseInt(moveStr.slice(4,5));
    move.piece = moveStr.slice(5,7);
    move.promote = moveStr.slice(7,9) != '--';
    move.capture = moveStr.slice(9,11) == '--' ? '' : moveStr.slice(8,10);
  }catch(e){
    console.log(e);
    return moveStr;
  }
  return move;
}

exports.ObjectToCSA = function(moveObj){
  if( !_.has(moveObj,'from.x') ||
      !_.has(moveObj, 'from.y') ||
      !_.has(moveObj, 'to.x') ||
      !_.has(moveObj, 'to.y') ||
      !_.has(moveObj, 'piece')){
    throw new Error('Invalid object');
  }
  var fromX = _.get(moveObj, 'from.x');
  var fromY = _.get(moveObj, 'from.y');
  var toX = _.get(moveObj, 'to.x');
  var toY = _.get(moveObj, 'to.y');
  var piece = _.get(moveObj, 'piece');
  var res = ''+fromX+fromY+toX+toY+piece;
  return res;
}
