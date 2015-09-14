var game = require('../build/Release/gogoshogi');

game.start();
console.log(game.move("3545GI"));
console.log(game.search());
console.log(game.hello());
