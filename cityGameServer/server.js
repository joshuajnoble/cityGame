var path = require('path');

/********************
  CITY GAME
********************/

var CityGame = require('./libs/citygame');
var storagePath = path.join(__dirname, 'data');

var game = new CityGame(storagePath);

/********************
  EXPRESS
********************/

var express = require('express');
var http = require('http');
var app = express();
var server;

app.set('port', process.env.PORT || 3000);

app.use(express.static(path.join(__dirname, 'public')));

server = http.createServer(app).listen(app.get('port'), function () {
  console.log("Express server listening on port " + app.get('port'));
});

/********************
  SERIAL PORT
********************/

var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var moment = require('moment');

var serialPortInstance = new SerialPort("/dev/tty.usbmodemfa131", {
  baudrate: 9600,
  parser: serialport.parsers.readline("\n")
});

serialPortInstance.on('open', function () {
  console.log('serial port: open');

  serialPortInstance.on('data', function (data) {
    game.log(data);
  });

});

/********************
  SOCKET.IO
********************/

var io = require('socket.io').listen(server, { origin: '*:*' });

io.sockets.on('connection', function (socket) {
  console.log('websocket: connection');

  socket.on('disconnect', function () {
    console.log('websocket: disconnection');
  });
});

game.on('log', function (data) {
  io.sockets.emit('log', data);
});
