var fs = require('fs');
var path = require('path');
var util = require('util');
var EventEmitter = require('events').EventEmitter;
var moment = require('moment');

module.exports = CityGame;

function timestamp() {
  return moment().format("HH:mm:ss");
};

function CityGame(storagePath) {
  EventEmitter.call(this);

  var fileName = moment().format("YYYY-MM-DD-HH:mm")
  this._logPath = path.join(storagePath, fileName + '.txt');
};

util.inherits(CityGame, EventEmitter);

CityGame.prototype.log = function (data) {
  var self = this;
  var replacement = ',' + timestamp() + '\r';
  var data = data.replace(/\r/, replacement);

  fs.appendFile(this._logPath, data, function (error) {
    if (error) throw error;
    self.emit('log', data);
  });
};
