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
