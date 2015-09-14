spawn = require('child_process').spawn
client = require 'socket.io-client'
io = client.connect 'http://localhost:3000'
koma = spawn './komachan/bin/komachan', ['--server-mode']
game = null

io.emit 'komachan', {}

io.on 'confirm', (data)->
  game = data
  io.emit 'agree', {user: 'komachan'}

io.on 'start', (data)=>
  console.log 'start', game
  if(game.header.firstMove == 'koma')
    console.log 'write'
    koma.stdin.write 'search\n'

koma.stdout.on 'data', (data)->
  console.log 'koma', data.toString()
