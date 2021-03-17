i = 0;
let arr = []
let foo;
let artwork = false;
let listen;
let words;
let synth, soundLoop, filter;
let score;
let textinput;
let timer = 0;

let timestamp = 0;
let serial;
let portName = '/dev/tty.usbserial-ADAQJDOxI'; // fill in your serial port name here
var inData;    
var outByte = 11; 
// this is the message that will be sent to the Arduino:
let starter = false


function preload() {
  //psych = loadJSON("Psych.json")
  afinn = loadJSON("afinn111.json")
  happy = loadSound('Happy_Sounds.mp3');
  angry = loadSound('Angry_Sounds.mp3');
  happyEnd = loadSound('HappyEnd.mp3');
  sadEnd = loadSound('SadEnd.mp3');

}

function setup() {
  createCanvas(windowWidth, windowHeight);
  foo = new p5.Speech(); // speech synthesis object
  let lang = navigator.language || 'en-US'
  let listen = new p5.SpeechRec(lang, gotSpeech);
  let continuous = true;
  let interim = false;
  listen.start(continuous, interim);
  let intervalInSeconds = 0.1;
  
  happy.amp(0.0)
  happy.loop()
  angry.loop()
  angry.amp(0.0)
  sadEnd.loop()
  sadEnd.amp(0.0)
  happyEnd.loop()
  happyEnd.amp(0.0)
 
    
  // make an instance of the SerialPort object
  serial = new p5.SerialPort();
  
  // Get a list the ports available
  // You should have a callback defined to see the results. See gotList, below:
  serial.list();
  
  // Assuming our Arduino is connected,  open the connection to it
  serial.open(portName);
  
  // When you get a list of serial ports that are available
  serial.on('list', gotList);
  
  // When you some data from the serial port
  serial.on('data', gotData);
  

function gotSpeech(){
    textinput = listen.resultString
    console.log(listen.resultString)
    typing()
     }
 }

// Got the list of ports
function gotList(thelist) {
  console.log("List of Serial Ports:");
  // theList is an array of their names
  // for (let i = 0; i < thelist.length; i++) {
  //   // Display in the console
  //   //console.log(i + " " + thelist[i]);
  // }
}

// Called when there is data available from the serial port
function gotData() {
  let currentString = serial.readLine();
  //console.log(currentString);
}



function draw() {
  background(255);
  let textinput = txt.value;
  let words = textinput.split(/\W/);
  console.log(outByte);
  serial.write(int(outByte))
  
//   if(millis() > timer + 60000){
//     timer = millis()
//     outByte --
    
//   }
  
  
  
  happyMap = map(outByte, 11, 16, 0, 0.8)
  happy.amp(happyMap);
  angryMap = map(outByte,11 ,6 , 0, 0.8)
  angry.amp(angryMap);
  
  
  if(outByte < 6){
    outByte = 6
  }
  
  if(outByte > 16){
    outByte = 16
    
  }
  
  if(outByte < 11){
    happy.amp(0)
  }
  
  if(outByte > 11){
    angry.amp(0)
  }

  if(outByte == 16){
    happyEnd.amp(0.4)
  }
  
  if(outByte == 6){
    sadEnd.amp(0.4)
  }


  
  // console.log("happy" + happyMap)
  // console.log("angry" + angryMap)
  
  
}

function typing() {
  // let textinput = listen.resultString;
  let words = textinput.split(/\W/);

  console.log("JUST WORDS:", words);
  let totalScore = 0;
  for (let i = 0; i < words.length; i++) {
    var word = words[i].toLowerCase();

    if (afinn.hasOwnProperty(word)) {
      let score = afinn[word];
      console.log("SCORED WORD:", word, score);
      totalScore += Number(score);
    }
  }
  console.log("TOTAL SCORE:", totalScore)
  if(totalScore < 0){
    outByte--
  }
  if(totalScore > 0){
    outByte++
  }
  
  // push total score to global Array 'arr'
  if (totalScore != 0) {
    arr.push(totalScore)
    //console.log("final array:", arr)
  }
}

