
// Retrieve
var MongoClient = require('mongodb').MongoClient;
var db;
// Connect to the db
MongoClient.connect("mongodb://localhost:27017/test", function(err, _db) {
  if(!err) {
    console.log("We are connected");
    db = _db;
  } else {
	   console.log(err);
	}
});

//start mqtt broker
var mosca = require('mosca');
var settings = {
		port:1883
		}

var server = new mosca.Server(settings);

server.on('ready', function(){
console.log("ready");
});

// Subscribe to topic in mqtt
var mqtt = require('mqtt')

var client  = mqtt.connect('mqtt://127.0.0.1:1883')
client.on('connect', function () {
    client.subscribe('newData');
    client.subscribe('updateData');
    client.subscribe('testTopic');
});
var prevId;
client.on('message', function (topic, message) {
  if(topic === "newData"){
  	context = message.toString();
  	console.log(topic, ' ' ,JSON.parse(context));
    db.collection("dataset").insertOne(JSON.parse(context), function(err, result) {
     if (err) throw err;
     console.log(result.ops[0]);
     prevId = result.ops[0]._id;
   });
 } else if(topic === "updateData"){
   context = message.toString();
   console.log(topic, ' ' ,JSON.parse(context));
   db.collection("dataset").update({_id: prevId},
     { $set: JSON.parse(context)},
      function(err, result) {
    if (err) throw err;
    console.log(result);
  });
} else {
  db.collection("inventory").insertOne({name : message.toString()}, function(err, result) {
   if (err) throw err;
   console.log(result.ops[0]._id);
 });
}
});
