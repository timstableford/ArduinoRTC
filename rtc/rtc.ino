#include "rtclib.h"
#define BUFFER 10
RTC rtc(A3, A4, A5);
unsigned char buffer[BUFFER];
Time alarm;
//Time clone;
void setup()
{
  Serial.begin(9600);
  //Power on the chip
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  //Enable trickle
  rtc.setTrickleState(2, 8);
  alarm.loadFromEEPROM(0);
}
void loop()
{
  int read = -1;
  //Read to the serial buffer
  do{
    read = Serial.read();
    if(read>=0){
      for(int i=0; i<BUFFER-1; i++){
        buffer[i] = buffer[i+1];
      }
      buffer[BUFFER-1] = (unsigned char)read;
      processIncoming();
    }
  }while(read>=0);
  rtc.read();
  digitalWrite(A0, alarm.compare( *(rtc.getDateTime()) )<0);
}
void processIncoming(){
  Time t;
  if(buffer[0]=='b'&&buffer[1]=='g'){
    switch(buffer[2]){
      case 'p':
        rtc.read();
        printTime(rtc.getDateTime());
        break;
      case 's':
        t.setYear((unsigned int)buffer[3]+2000);
        t.setMonth((unsigned int)buffer[4]);
        t.setDate((unsigned int)buffer[5]);
        t.setDay((unsigned int)buffer[6]);
        t.setHours((unsigned int)buffer[7]);
        t.setMinutes((unsigned int)buffer[8]);
        t.setSeconds((unsigned int)buffer[9]);
        rtc.setDateTime(t);
        break;
      case 'a':
        alarm.setYear((unsigned int)buffer[3]+2000);
        alarm.setMonth((unsigned int)buffer[4]);
        alarm.setDate((unsigned int)buffer[5]);
        alarm.setDay((unsigned int)buffer[6]);
        alarm.setHours((unsigned int)buffer[7]);
        alarm.setMinutes((unsigned int)buffer[8]);
        alarm.setSeconds((unsigned int)buffer[9]);
        alarm.saveToEEPROM(0);
        break;
      case 'w':
        printTime(&alarm);
        break;
      case 'c':
        rtc.read();
        Serial.println(alarm.compare( *(rtc.getDateTime()) ));
        break;
    }
    clearBuffer();
  }
}
void printTime(Time *t){

  char buffer[80];

  sprintf( buffer, "Time = %02d:%02d:%02d, ", \
    t->getHours(), \
    t->getMinutes(), \
    t->getSeconds());
  Serial.print(buffer);

  sprintf(buffer, "Date(day of month) = %d, Month = %d, " \
    "Day(day of week) = %d, Year = %d", \
    t->getDate(), \
    t->getMonth(), \
    t->getDay(), \
    t->getYear());
  Serial.println(buffer);
}
void clearBuffer(){
  for(int i=0; i<BUFFER; i++){
    buffer[i] = 0;
  }
}
