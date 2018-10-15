var part_one,part_two,graph_temp,graph_hum,graph_soil,graph_light,
graph_externalTemp,graph_externalHum ;
var count = 0;
var socket = io();
socket.on('mqtt', function(msg){
   var res = msg.split(" ");
   console.log(res);
   part_one = res[0];
   part_two = res[1];
   if(part_one == "temperature"){
     graph_temp = part_two;
     count = count + 1;
     // console.log(graph_temp);
   }else if(part_one == "humidity"){
     graph_hum = part_two;
     count = count + 1;
   }else if(part_one == "soilMoisture"){
     graph_soil = part_two;
     count = count + 1;
   }else if(part_one == "light"){
     graph_light = part_two;
     count = count + 1;
   }else if (part_one == "externalTemp") {
       graph_externalTemp = +part_two;
       count = count + 1;
   } else if (part_one == "externalHum") {
       graph_externalHum = part_two;
       count = count + 1;
   }
});

socket.on('control', function(actuator_control){
    console.log('control here ', actuator_control);
        socket.emit('publish', actuator_control);
});

function light_control() {
   if (document.querySelectorAll("input[id='switch7']:checked").length >= 1) {
       socket.emit('publish', {topic:"light",payload:"1"});
       console.log("publish", {topic:"light",payload:"1"})
   }else{
       socket.emit('publish', {topic:"light",payload:"0"});
       console.log("publish", {topic:"light",payload:"0"})
   }
}
function humidifier_control() {
   if (document.querySelectorAll("input[id='switch8']:checked").length >= 1) {
       socket.emit('publish', {topic:"humidifier",payload:"1"});
       console.log("publish", {topic:"humidifier",payload:"1"})

   }else{
       socket.emit('publish', {topic:"humidifier",payload:"0"});
       console.log("publish", {topic:"humidifier",payload:"0"})

   }    }
function fan_control() {
   if (document.querySelectorAll("input[id='switch1']:checked").length >= 1) {
      socket.emit('publish', {topic:"fan",payload:"1"});
      console.log("publish", {topic:"fan",payload:"1"})
   }else{
      socket.emit('publish', {topic:"fan",payload:"0"});
      console.log("publish", {topic:"fan",payload:"0"})
   }
}
function rule1_apply() {
    socket.emit('rules', {topic:"fan",payload:"0"});
}
