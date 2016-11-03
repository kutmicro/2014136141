#2014136141-song,wonseop

**vibration motor sample code**

const int motorPin = 3;
void setup() {
pinMode(motorPin, OUTPUT); 
} 
void loop() { 
digitalWrite(motorPin, HIGH); 
delay(1000); 
digitalWrite(motorPin, LOW);
delay(59000);
}


**music shield sample code** 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // 
RX, TXbyte note = 0; //The MIDI note value to be playedbyte resetMIDI = 4;
//Tied to VS1053 Reset linebyte ledPin = 13; //MIDI traffic inidicatorint  instrument = 0;
void setup()
{
Serial.begin(57600);  //Setup soft serial for MIDI control  
mySerial.begin(31250);  //Reset the VS1053  pinMode(resetMIDI, OUTPUT);
digitalWrite(resetMIDI, LOW);  delay(100);  
digitalWrite(resetMIDI, HIGH);  delay(100); 
talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)
}

void loop() {  //Demo Basic MIDI instruments, GM1  //================================================================= 
Serial.println("Basic Instruments");
talkMIDI(0xB0, 0, 0x00); //Default bank GM1  //Change to different instrument  
for(instrument = 0 ; 
instrument < 127 ; 
instrument++) { 
Serial.print(" Instrument: ");    
Serial.println(instrument, DEC); 
talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command    //Play notes from F#-0 (30) to F#-5 (90):    for (note = 30 ; note < 40 ; note++) {
Serial.print("N:"); 
Serial.println(note, DEC);   //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):    
noteOn(0, note, 60);    
delay(50);      //Turn off the note with a given off/release velocity      
noteOff(0, note, 60);   
delay(50);   
} 
delay(100); //Delay between instruments
}
void noteOn(byte channel, byte note, byte attack_velocity) 
{
talkMIDI( (0x90 | channel), note, attack_velocity);
}//Send a MIDI note-off message.  Like releasing a piano keyvoid
noteOff(byte channel, byte note, byte release_velocity) { 
talkMIDI( (0x80 | channel), note, release_velocity);}//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127void talkMIDI(byte cmd, byte data1, byte data2) {  digitalWrite(ledPin, HIGH);  mySerial.write(cmd);  mySerial.write(data1);  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
if( (cmd & 0xF0) <= 0xB0)    mySerial.write(data2);  digitalWrite(ledPin, LOW);}


**touch shield sample code**
*void setup()*
{
        pinMode(irqpin, INPUT);
        digitalWrite(irqpin, HIGH); //enable pullup resistor

        Serial.begin(9600);
        Wire.begin();

        mpr121_setup();
}
*void loop()*{ 
        readTouchInputs();
    }
    void readTouchInputs(){  
    if(!checkInterrupt()){
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A,2); 
    byte LSB = Wire.read();    
    byte MSB = Wire.read();
    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states
    for (int i=0; i < 12; i++){  // Check what electrodes were pressed    
    if(touched & (1<<i)){
       
       if(touchStates[i] == 0){          //pin i was just touched          
       Serial.print("pin ");      
       Serial.print(i);    
       Serial.println(" was just touched");
        }
        else if(touchStates[i] == 1){          //pin i is still being touched    
        }  
        touchStates[i] = 1;         
        }else{     
        if(touchStates[i] == 1){      
        Serial.print("pin ");     
        Serial.print(i);      
        Serial.println(" is no longer being touched");
          //pin i is no longer being touched       }
        touchStates[i] = 0;      }
    }
  }
}
