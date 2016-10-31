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
