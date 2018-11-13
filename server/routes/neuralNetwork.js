var express = require('express');
var fs = require('fs');
var router = express.Router();

var trainingScript = "../NN/estimator_multilayer_perceptron.py";
var predictScript = "../NN/restore.py";
var pythonExecutable = "python";



var uint8arrayToString = function(data){
    return String.fromCharCode.apply(null, data);
};

/* GET users listing. */
router.get('/train', function(req, res, next) {
    const spawn = require('child_process').spawn;
    const scriptExecution = spawn(pythonExecutable, [trainingScript]);
    scriptExecution.stdout.on('data', (data) => {
        console.log(uint8arrayToString(data));
        res.send(uint8arrayToString(data));
    });
    // Handle error output
    scriptExecution.stderr.on('data', (data) => {
        // As said before, convert the Uint8Array to a readable string.
        console.log(uint8arrayToString(data));
    });

    scriptExecution.on('exit', (code) => {
        console.log("Process quit with code : " + code);
    });
});

router.get("/getData", function(req, res, next) {

    let rawdata = fs.readFileSync("../NN/Model/accuracy.json");
    let accuracyJsonFile = JSON.parse(rawdata);
    res.json(accuracyJsonFile);
});

router.get("/predict", function (req, res, next) {
    console.log("predicting");
    let arg1 = '2';
    let arg2 = '22';
    let arg3 = '2';
    let arg4 = '22';
    const spawn = require('child_process').spawn;
    const scriptExecution = spawn(pythonExecutable, [predictScript, arg1, arg2, arg3, arg4]);
    scriptExecution.stdout.on('data', (data) => {
        console.log(uint8arrayToString(data));
        res.send(uint8arrayToString(data));
    });
});

module.exports = router;
