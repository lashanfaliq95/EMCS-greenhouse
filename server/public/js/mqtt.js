var part_one,part_two,graph_temp,graph_hum,graph_soil,graph_light;
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
   }else if(part_one == "soilMoisture"){
     graph_soil = part_two;
   }else if(part_one == "light"){
     graph_light = part_two;
   }
});
function pump1_control() {
   if (document.querySelectorAll("input[id='switch7']:checked").length >= 1) {
       socket.emit('publish', {topic:"pump",payload:"1"});
   }else{
       socket.emit('publish', {topic:"pump",payload:"0"});
   }
}
function pump2_control() {
   if (document.querySelectorAll("input[id='switch8']:checked").length >= 1) {
       socket.emit('publish', {topic:"pump2",payload:"1"});
   }else{
       socket.emit('publish', {topic:"pump2",payload:"0"});
   }    }
function fan1_control(elem) {
   if (document.querySelectorAll("input[id='switch1']:checked").length >= 1) {
       socket.emit('publish', {topic:"fan",payload:"1"});
   }else{
       socket.emit('publish', {topic:"fan",payload:"0"});
   }
}
function fan2_control() {
    socket.emit('publish', {topic:"fan",payload:"0"});
     if (document.querySelectorAll("input[id='switch2']:checked").length >= 1) {
       socket.emit('publish', {topic:"fa2",payload:"1"});
   }else{
       socket.emit('publish', {topic:"fa2",payload:"0"});
   }
}
function humidifier1_control(elem) {
    if (document.querySelectorAll("input[id='switch8']:checked").length >= 1) {
        socket.emit('publish', {topic:"humidifier",payload:"1"});
    }else{
        socket.emit('publish', {topic:"humidifier",payload:"0"});
    }
}
function rule1_apply() {
    socket.emit('rules', {topic:"fan",payload:"0"});
}
