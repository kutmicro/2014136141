/*
 Music Art Robot v1.0 (2012.May.30)
 ---------------------------------------------------------
 아두이노 + MIDI악기쉴드 이용한 DIY 건반악기 예제소스 (Arduino 1.0 호환)
 ---------------------------------------------------------
 http://ArtRobot.co.kr  
 http://RoboBob.co.kr
 재배포시, 위 URL 유지부탁드리고요, 자유롭게 사용해주세요!
 ---------------------------------------------------------
 
 아두이노 D3,D4핀은 MIDI악기쉴드 제어용이므로 다른용도로 사용하지 마세요
 아두이노 D3핀을 통해 MIDI악기쉴드에 MIDI신호를 전송합니다.(NewSerailSoft 라이브러리 필요함)
 D2핀도 SoftSerial 수신용으로 선언은됐지만, 재세팅하고 버튼 입력용으로 사용됩니다.

 버튼1 ~ 8 : 도/레/미/파/솔/라/시/도  임의 note(음계)로 변경하시면 됩니다.
 버튼9: 다른 악기 선택(현재 악기번호 + 1)  0~127
 버튼10: 다른 악기 선택(현재 악기번호 - 1)  0~127
 버튼11: 악기를 드럼셋으로 설정. 
 버튼12: defaultPatch 에 정의된 악기로 변경

 외부스위치 연결방법: 
 버튼의 양쪽선 중 한쪽은 아두이노 해당 핀에 연결하고, 나머지 한쪽은 GND에 공통 연결하면 끝.
 
 참고사항, 전원을 켠 후 소리가 나지않을 경우 리셋버튼을 눌러서 초기화 해주면 작동됩니다.


 MIDI악기쉴드에 대한 아래의 영문정보도 참고하세요.
  
 This code works with the VS1053 Breakout Board and controls the VS1053 in what is called Real Time MIDI mode. 
 To get the VS1053 into RT MIDI mode, power up the VS1053 breakout board with GPIO0 tied low, GPIO1 tied high.
 
 I use the NewSoftSerial library to send out the MIDI serial at 31250bps. This allows me to print regular messages
 for debugging to the terminal window. This helped me out a ton.
 
 5V : VS1053 VCC
 GND : VS1053 GND
 D3 (SoftSerial TX) : VS1053 RX
 D4 : VS1053 RESET
 
 Attach a headphone breakout board to the VS1053:
 VS1053 LEFT : TSH
 VS1053 RIGHT : RSH
 VS1053 GBUF : GND
 
 When in the drum bank (0x78), there are not different instruments, only different notes.
 To play the different sounds, select an instrument # like 5, then play notes 27 to 87.
 
 To play "Sticks" (31):
 talkMIDI(0xB0, 0, 0x78); //Bank select: drums
 talkMIDI(0xC0, 5, 0); //Set instrument number
 //Play note on channel 1 (0x90), some note value (note), middle velocity (60):
 noteOn(0, 31, 60);
 
 */
 
#include <SoftwareSerial.h>
#define btn1  11	// 버튼1의 아두이노 핀번호 정의
#define btn2  10	// 버튼2의 아두이노 핀번호 정의
#define btn3  9		// 버튼3의 아두이노 핀번호 정의
#define btn4  8		// 버튼4의 아두이노 핀번호 정의
#define btn5  7		// 버튼5의 아두이노 핀번호 정의
#define btn6  6		// 버튼6의 아두이노 핀번호 정의
#define btn7  5   // 버튼7의 아두이노 핀번호 정의
// 3:midi rx , 4:midi reset  아두이노 핀 3번 4번은 이미 사용중
#define btn8  2   // 버튼8의 아두이노 핀번호 정의 
                  //(SoftSerial에서 Rx핀으로 선언되지만 재 세팅 후 버튼용으로 사용)
#define btn9 A5   // 버튼9의 아두이노 핀번호 정의
#define btn10 A4  // 버튼10의 아두이노 핀번호 정의
#define btn11 A3  // 버튼11의 아두이노 핀번호 정의
#define btn12 A2  // 버튼12의 아두이노 핀번호 정의

#define defaultPatch 15 //악기 초기화 버튼 설정 악기번호

SoftwareSerial mySerial(2, 3); //SW시리얼핀 정의 D3이 MIDI신호 전송용,  D2는 미사용 

byte note = 0; //The MIDI연주될 note(음계)
byte resetMIDI = 4; // VS1053 Reset용 핀
byte ledPin = 13; //MIDI 트래픽 표시용 LED
 
boolean bs1 = false;  // 버튼1의 현재상태(눌림 or 안눌림)
boolean bs2 = false;  // 이하, 위와 유사
boolean bs3 = false;
boolean bs4 = false;
boolean bs5 = false;
boolean bs6 = false;
boolean bs7 = false;
boolean bs8 = false;
boolean bs9 = false;
boolean bs10 = false;
boolean bs11 = false;
boolean bs12 = false;

boolean br1;  // 버튼1 상태 확인용 입력값 임시저장용
boolean br2;  // 이하, 위와 유사
boolean br3;
boolean br4;
boolean br5;
boolean br6;
boolean br7;
boolean br8;
boolean br9;
boolean br10;
boolean br11;
boolean br12;

int patch = 0; //악기 대응, 연주될 악기 종류 (0~127: 기본 128 가지 선택가능)

int bn1 = 60; //버튼1의  note(음계)  가령 "도"  0~127까지 지정가능 (정확한 음계 설정은 MIDI관련정보참고)
int bn2 = 62; //버튼2의  note(음계)  가령 "레"
int bn3 = 64; //버튼3의  note(음계)  가령 "미"
int bn4 = 65; //버튼4의  note(음계)  가령 "파"
int bn5 = 67; //버튼5의  note(음계)  가령 "솔"
int bn6 = 69; //버튼6의  note(음계)  가령 "라"
int bn7 = 71; //버튼7의  note(음계)  가령 "시"
int bn8 = 72; //버튼8의  note(음계)  가령 "도~"

byte byteData;

void setup() {
  Serial.begin(31250);

  //Setup soft serial for MIDI control
  mySerial.begin(31250);
//  mySerial2.begin(57600);
  
  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  
  pinMode( btn1, INPUT);      // 버튼1 입력용 핀모드를  입력모드로 전환
  digitalWrite( btn1, HIGH);  // 내부 PullUp 설정, 스위치의 나머지 한선은 GND에 물리면 됩니다.(초간단)

  pinMode( btn2, INPUT);      // 이하, 위와 유사
  digitalWrite( btn2, HIGH);
  
  pinMode( btn3, INPUT);
  digitalWrite( btn3, HIGH);
  pinMode( btn4, INPUT);
  digitalWrite( btn4, HIGH);
  pinMode( btn5, INPUT);
  digitalWrite( btn5, HIGH);
  pinMode( btn6, INPUT);
  digitalWrite( btn6, HIGH);
  pinMode( btn7, INPUT);
  digitalWrite( btn7, HIGH);
  pinMode( btn8, INPUT);
  digitalWrite( btn8, HIGH);
  
  pinMode( btn9, INPUT);
  digitalWrite( btn9, HIGH);
  pinMode( btn10, INPUT);
  digitalWrite( btn10, HIGH);
  pinMode( btn11, INPUT);
  digitalWrite( btn11, HIGH);  
  pinMode( btn12, INPUT);
  digitalWrite( btn12, HIGH);  

}

void loop() {
    br1 = digitalRead(btn1);
    br2 = digitalRead(btn2);
    br3 = digitalRead(btn3);
    br4 = digitalRead(btn4);
    br5 = digitalRead(btn5);
    br6 = digitalRead(btn6);
    br7 = digitalRead(btn7);
    br8 = digitalRead(btn8);
    br9 = digitalRead(btn9);
    br10 = digitalRead(btn10);
    br11 = digitalRead(btn11);
    br12 = digitalRead(btn12);    
    
   if( !bs1 && !br1 ){
     noteOn(0, bn1,100);
     bs1 = true;
   }else if(bs1 && br1){
     noteOff(0, bn1,0);   
     bs1 = false;
   }
   if( !bs2 && !br2 ){
     noteOn(0, bn2,100);
     bs2 = true;
   }else if(bs2 && br2){
     noteOff(0, bn2,0);   

     bs2 = false;
   }   
   if( !bs3 && !br3 ){
     noteOn(0, bn3,100);
     bs3 = true;
   }else if(bs3 && br3){
     noteOff(0, bn3,0);   
     bs3 = false;
   }
   if( !bs4 && !br4 ){
     noteOn(0, bn4,100);
     bs4 = true;
   }else if(bs4 && br4){
     noteOff(0, bn4,0);   
     bs4 = false;
   }
   if( !bs5 && !br5 ){
     noteOn(0, bn5,100);
     bs5 = true;
   }else if(bs5 && br5){
     noteOff(0, bn5,0);   
     bs5 = false;
   }   
   if( !bs6 && !br6 ){
     noteOn(0, bn6,100);
     bs6 = true;
   }else if(bs6 && br6){
     noteOff(0, bn6,0);   
     bs6 = false;
   }   
   
   if( !bs7 && !br7 ){
     noteOn(0, bn7,100);
     bs7 = true;
   }else if(bs7 && br7){
     noteOff(0, bn7,0);   
     bs7 = false;
   }   
   if( !bs8 && !br8 ){
     noteOn(0, bn8,100);
     bs8 = true;
   }else if(bs8 && br8){
     noteOff(0, bn8,0);   
     bs8 = false;
   }   
   
   if( !bs9 && !br9 ){ //patch up (max:127)
     patch++;
     if(patch >127) patch = 0;
     talkMIDI(0xc0, patch, 0);     
     bs9 = true;
   }else if(bs9 && br9){
     bs9 = false;
   }   
   if( !bs10 && !br10 ){ //patch down (min:0)
     patch--;
     if(patch < 0) patch = 127;
     talkMIDI(0xc0, patch, 0);       
     bs10 = true;
   }else if(bs10 && br10){
     bs10 = false;
   }   
   
   if( !bs11 && !br11 ){
     //bank 0x78(drum)
     talkMIDI(0xb0, 0, 0x78);
     talkMIDI(0xb0,20, 0);
     talkMIDI(0xc0, patch, 0);     
     bs11 = true;
   }else if(bs11 && br11){
     bs11 = false;
   }   
   
   if( !bs12 && !br12 ){
     //bank MSB 0, default instruments
     patch = defaultPatch;
     talkMIDI(0xb0, 0, 0);
     talkMIDI(0xb0,20, 0);
     talkMIDI(0xc0, patch, 0);       
     bs12 = true;
   }else if(bs12 && br12){
     bs12 = false;
   }   
            

  //*************** MIDI LOOPBACK ******************//
  if(Serial.available() > 0)
  {
    byteData =  Serial.read();
    mySerial.write( byteData);
  }  
     

}

//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd );
  mySerial.write(data1 );

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2 );

  digitalWrite(ledPin, LOW);
}






