/* -------------------- Variables -------------------- */
// Serial Connection variables
var serial;       						// Serial port object
var serialPortName = "COM8";			// Serial port name
var ardSend = {};						// JSON variable

// Variables
var Pulse = 0, BPM = 0;
var Blackheart, Redheart;

/* -------------------- Functions -------------------- */
function setup() 
{
    getAudioContext().resume();
    createCanvas(windowWidth, windowHeight);
    background(0);
       
    // Setting up the serial port
    serial = new p5.SerialPort();		// Create the serial port object
    serial.open(serialPortName);		// Open the serialport
    serial.on('open',ardCon);			// Open the socket connection
    serial.on('data',dataReceived);		// Call dataReceived when new data is received
	
	Blackheart = createImg("Blackheart.jpg");
	Blackheart.size(316,316);
	Blackheart.position((windowWidth/2) - (Blackheart.width/2), (windowHeight/2) - (Blackheart.height/2));
	Blackheart.hide();
	
	Redheart = createImg("Redheart.jpg");
	Redheart.size(316,316);
	Redheart.position((windowWidth/2) - (Redheart.width/2), (windowHeight/2) - (Redheart.height/2));
	Redheart.hide();
	
}
function draw() {}
function windowResized() {}

/* -------------------- Arduino -------------------- */
// Recieve data from Arduino
function dataReceived ()
{
	var rawData = serial.readStringUntil('\r\n');		// Read the incoming string until it sees a newline
	console.log(rawData);
	
	if( rawData.length > 1 )								// Check that there is something in the string
	{
		Pulse = JSON.parse(rawData).Pulse;		// Set variable switch1val to switch1 from the incoming string 
		BPM = JSON.parse(rawData).BPM;		// Set variable switch2val to switch2 from the incoming string 
	}
	
	if (Pulse == "1"){
		background(0);
		if (Blackheart){
		Blackheart.hide();}
		Redheart.show();
		fill(255);
		textSize(30);
		textAlign(CENTER);
		text(BPM,0,200,windowWidth,windowHeight);
		if ( BPM >= 70 ){
			text("Calm Down! Psitive Thoughts",0,300,windowWidth,windowHeight);
		}
		
	} else{
		background(0);
		if (Redheart) {
			Redheart.hide();
		}
		Blackheart.show();
		fill(255);
		textSize(30);
		textAlign(CENTER);
		text(BPM,0,200,windowWidth,windowHeight);
		if ( BPM >= 70 ){
			text("Calm Down! Psitive Thoughts",0,300,windowWidth,windowHeight);
		}
	}
}
function ardCon ()
{
    console.log("connected to the Arduino Micro! Let's Go");
}