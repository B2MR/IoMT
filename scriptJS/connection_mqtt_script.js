var mqtt = require('mqtt')
var client = mqtt.connect([{ host: '192.168.1.29', port: 1883 }]) // Adresse IP de votre mac (identique au broker)

var i = 1;

client.on('connect', function () {

  client.subscribe('IoMT/mqtt-from-nodejs', function (err) {
    if (!err) {
      Loop();
    }
  })

})

function Loop() {

  setTimeout(function() {
    client.publish('IoMT/sensor/temp', (Math.random() * (38 - 37) + 37).toFixed(2));
    client.publish('IoMT/sensor/pulse', (Math.floor(Math.random() * (130 - 60)) + 60).toString());
    client.publish('IoMT/sensor/accelerometer',
    `{
      x: ${(Math.random() * (7.5 - 0.5) + 0.5).toFixed(4)},
      y: ${(Math.random() * (7.5 - 0.5) + 0.5).toFixed(4)},
      z: ${(Math.random() * (7.5 - 0.5) + 0.5).toFixed(4)}
    }`);
    if (i) {
      Loop();
    }
  }, 3000) // loop every 3000ms
}

client.on('message', function (topic, message) {
    // message is Buffer
    console.log(message.toString())
    client.end()
})
