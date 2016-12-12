
#include <SoftwareSerial.h>

#define ledbtn1 43
#define ledbtn2 45
#define ledbtn3 47
#define ledbtn4 49
#define ledbtn5 31 //진동모터 led로 대체 풀업 핀
#define ledbtn6 33 //진동모터 led로 대체 풀업 핀

#define btn1  11  // 버튼1의 아두이노 핀번호 정의
#define btn2  10  // 버튼2의 아두이노 핀번호 정의
#define btn3  9   // 버튼3의 아두이노 핀번호 정의
#define btn4  8   // 버튼4의 아두이노 핀번호 정의
#define btn5  7   // 버튼5의 아두이노 핀번호 정의
#define btn6  6   // 버튼6의 아두이노 핀번호 정의
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


boolean ledbr1;
boolean ledbr2;
boolean ledbr3;
boolean ledbr4;
boolean ledbr5;
boolean ledbr6;

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

  //미디컨트롤을 위한 소프트웨어 시리얼을 준비합니다.
  mySerial.begin(31250);
//  mySerial2.begin(57600);
  
  //VS1053를 초기화하고 사용할 준비를 합니다.
  pinMode(42,OUTPUT);
  pinMode(43,INPUT_PULLUP);
  pinMode(44,OUTPUT);
  pinMode(45,INPUT_PULLUP);
  pinMode(46,OUTPUT);
  pinMode(47,INPUT_PULLUP);
  pinMode(48,OUTPUT);
  pinMode(49,INPUT_PULLUP);
  pinMode(30,OUTPUT);
  pinMode(31,INPUT_PULLUP);
  pinMode(32,OUTPUT);
  pinMode(33,INPUT_PULLUP);
  
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
    
    ledbr1=digitalRead(ledbtn1);
    ledbr2=digitalRead(ledbtn2);
    ledbr3=digitalRead(ledbtn3);
    ledbr4=digitalRead(ledbtn4);
    ledbr5=digitalRead(ledbtn5);
    ledbr6=digitalRead(ledbtn6);
    
    br7 = digitalRead(btn7);
    br8 = digitalRead(btn8);
    br9 = digitalRead(btn9);
    br10 = digitalRead(btn10);
    br11 = digitalRead(btn11);
    br12 = digitalRead(btn12);    

    if(ledbr1==0) //풀업저항 연결해 버튼누르면 led점등
    {
      digitalWrite(42,100);
    }
     else digitalWrite(42,0);
   if(ledbr2==0) //풀업저항 연결해 버튼누르면 led점등
    {
      digitalWrite(44,100);
    }
     else digitalWrite(44,0);
   if(ledbr3==0) //풀업저항 연결해 버튼누르면 led점등
    {
      digitalWrite(46,100);
    }
     else digitalWrite(46,0);
   if(ledbr4==0) //풀업저항 연결해 버튼누르면 led점등
    {
      digitalWrite(48,100);
    }
     else digitalWrite(48,0);
     if(ledbr5==0) //풀업저항 연결해 버튼누르면 led점등
    {
      digitalWrite(30,1);
    }
     else digitalWrite(30,0);

     if(ledbr6==0) //풀업저항 연결해 버튼누르면 led점등
    {
      digitalWrite(32,1);
    }
     else digitalWrite(32,0);
   
 
   if( !bs1 && !br1 ){
    noteOn(0x99, 35,100); //북소리      
     bs1 = true;
   }else if(bs1 && br1){
     noteOff(0, 35,0);   
     bs1 = false;
   }
   
   if( !bs2 && !br2 ){
     noteOn(0x99, 51,100);  //북소리
     bs2 = true;
   }else if(bs2 && br2){
     noteOff(0, 51,0);   
     bs2 = false;
   }   
   
   if( !bs3 && !br3 ){
     noteOn(0x99, 52,100);
     bs3 = true;
   }else if(bs3 && br3){
     noteOff(0, 52,0);   
     bs3 = false;
   }
   if( !bs4 && !br4 ){
     noteOn(0x99, 54,100);
     bs4 = true;
   }else if(bs4 && br4){
     noteOff(0, 54,0);   
     bs4 = false;
   }
   if( !bs5 && !br5 ){
     noteOn(0x99, 55,100);
     bs5 = true;
   }else if(bs5 && br5){
     noteOff(0, 55,0);   
     bs5 = false;
   }   
   if( !bs6 && !br6 ){
     noteOn(0x99, 57,100);
     bs6 = true;
   }else if(bs6 && br6){
     noteOff(0, 57,0);   
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

//노트 온 미디 메세지를 송출합니다. 버튼이 눌린상태와 같습니다.
//채널 범위는 0-15 입니다.
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//노트 오프 미디 메세지를 송출합니다. 버튼이 눌리지 않은 상태와 같습니다.
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}


void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd );
  mySerial.write(data1 );

  //모든 명령은 1바이트를 지니며, 모든 cmds는   0xBn보다  2 데이터 바이트를 덜 지닙니다.  
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2 );

  digitalWrite(ledPin, LOW);
}

