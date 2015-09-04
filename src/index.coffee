spawn = require('child_process').spawn
koma = spawn './komachan/bin/komachan'

koma.stdout.on 'data', (data) ->
  process.stdout.write do data.toString

koma.stdout.on 'end',  ->
  console.log 'end'

setTimeout( ->
  console.log 'write'
  koma.stdin.write 'search\n'
, 1000)
