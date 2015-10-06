var _ = require('lodash');

function CSAtoObject (moveStr){
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
    move.capture = moveStr.slice(9,11) == '--' ? '' : moveStr.slice(9,11);
    if( moveStr.length > 11 && (moveStr.indexOf('ATTACK') > -1)){
      move.isAttack = true;
    }
    if( (moveStr.indexOf('LOSE') > -1)){
      move = {};
      move.lose = true;
      move.message = moveStr.slice(moveStr.indexOf('LOSE'),moveStr.length);
      console.log(move);
    }
  }catch(e){
    console.log(e);
    return moveStr;
  }
  return move;
}

function ObjectToCSA(moveObj){
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

function LegaltoObject(legalstr){
  var moves = [];
  if( legalstr.indexOf('LEGAL') > -1 ) {
    return;
  }
  var startIdx = legalstr.indexOf('[')+1;
  var endIdx = legalstr.indexOf(']');
  var content = legalstr.slice(startIdx, endIdx);
  var moveStrs = content.split(',');
  moveStrs.forEach(function(moveStr){
    if(!moveStr){ return; }
    var fx = parseInt(moveStr.slice(0,1));
    var fy = parseInt(moveStr.slice(1,2));
    var tx = parseInt(moveStr.slice(2,3));
    var ty = parseInt(moveStr.slice(3,4));
    var piece = moveStr.slice(4,6);
    var res = {
      from: { x: fx, y: fy },
      to: { x: tx, y: ty },
      piece: piece
    };
    moves.push(res);
  });
  return moves
}

exports.CSAtoObject = CSAtoObject;
exports.ObjectToCSA =  ObjectToCSA;
exports.LegaltoObject =  LegaltoObject;
