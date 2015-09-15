var _ = require('lodash');

exports.CSAtoObject = function(moveStr){
  move = {};
  move.from = {};
  move.to = {};
  move.from.x = moveStr.slice(0,1);
  move.from.y = moveStr.slice(1,2);
  move.to.x = moveStr.slice(2,3);
  move.to.y = moveStr.slice(3,4);
  move.piece = moveStr.slice(4,6);
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
