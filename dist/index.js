var koma, spawn;

spawn = require('child_process').spawn;

koma = spawn('./komachan/bin/komachan');

koma.stdout.on('data', function(data) {
  return process.stdout.write(data.toString());
});

koma.stdout.on('end', function() {
  return console.log('end');
});

setTimeout(function() {
  console.log('write');
  return koma.stdin.write('search\n');
}, 1000);
