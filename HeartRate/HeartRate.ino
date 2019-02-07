/*  Getting_BPM_to_Monitor prints the BPM to the Serial Monitor, using the least lines of code and PulseSensor Library.
 *  Tutorial Webpage: https://pulsesensor.com/pages/getting-advanced
 *
--------Use This Sketch To------------------------------------------
1) Displays user's live and changing BPM, Beats Per Minute, in Arduino's native Serial Monitor.
2) Print: "♥  A HeartBeat Happened !" when a beat is detected, live.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.

// Libraries
#include <ArduinoJson.h>
#include <PulseSensorPlayground.h>

// Definitions
#define PULSEpin 0            // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
#define VIBpin 2              // Turn this one when heart rate is more than threshold
#define LEDpin 13             // Turn this one on to blink with heart beat

// Objects
PulseSensorPlayground pulseSensor;    // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

//  Variables
int myBPM = 0;
int BPMtrack = 0;
int heartBeatThreshold = 70;
int pulseSensorThreshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
                                // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                                // Otherwise leave the default "550" value. 
// Sampling
unsigned long lastSend;
int sendRate = 200;

unsigned long lastVibration;
int vibrationRate = 1200;

void setup() { 
  Serial.begin(9600);
  
  // Assign Pins
  pinMode(VIBpin,OUTPUT); 
  pinMode(LEDpin,OUTPUT);
  pulseSensor.analogInput(PULSEpin);
  pulseSensor.setThreshold(pulseSensorThreshold);
  
  pulseSensor.blinkOnPulse(LEDpin);       // Auto-magically blink Arduino's LED with heartbeat.
  
  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
  if (pulseSensor.begin()) {
    Serial.println("Everything is good!");  // This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() {
  // Send data through Serial Connection to the P5 every sendRate
  if ( millis()-lastSend >= sendRate )
  {
    myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
        
    /*if (pulseSensor.sawStartOfBeat()) {              // Constantly test to see if "a beat happened". 
      Serial.println("♥  A HeartBeat Happened ! ");   // If test is "true", print a message "a heartbeat happened".
      Serial.print("BPM: ");                          // Print phrase "BPM: " 
      Serial.println(myBPM);                          // Print the value inside of myBPM.
    }*/
    
    DynamicJsonBuffer messageBuffer(256);                   // Create the Buffer for the JSON object        
    JsonObject& p5Send = messageBuffer.createObject();      // Create a JsonObject variable in that buffer       
    
    p5Send["Pulse"] = digitalRead(LEDpin);                  // Attach the LED Status to the message's Pulse in the JSON
    p5Send["BPM"] = myBPM;                                  // Attach the BPM of the user to the message's BPM in the JSON
    
    p5Send.printTo(Serial);                                 // Print JSON object as a string
    Serial.println();
    
    lastSend = millis();
  }
  
  // Check the status of the Mini Vibrating Disk Motor every vibrationRate
  // This is to give 50 vibrations per minutes to try to calm the user
  if( millis()-lastVibration >= vibrationRate ){
    // If the BPM is higher than the set threshold turn on the vibration fro 20ms
    // Else do nothing
    if (myBPM >= heartBeatThreshold) {
      digitalWrite(VIBpin, HIGH);
      delay(20);
      digitalWrite(VIBpin, LOW);
    } else {
      digitalWrite(VIBpin, LOW);
    }
    
    lastVibration = millis();
  }
}
