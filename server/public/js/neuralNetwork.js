function trainNeuralNetwork() {
    fetch('/neuralNetwork/train').then(function(req,res) {
        // Use the response sent here
        console.log("train");
        getGraphData();
        location.reload(true);
    })
}

var epoches = [];
var accuracy = [];
var cost = [];
var final_accuracy;

function getGraphData() {
    fetch('neuralNetwork/getData').then(function (response) {
        return response.json();
    }).then(function (myJson) {
        epoches = myJson.epoches;
        accuracy = myJson.accuracy;
        cost = myJson.cost;
        final_accuracy = (myJson.final_accuracy*100).toString()+"%";
        document.getElementById("finalAccu").innerHTML = final_accuracy;
    })


}