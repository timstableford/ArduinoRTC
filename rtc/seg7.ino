#define SEG_A 12
#define SEG_B 11
#define SEG_C 13
#define SEG_D 10
#define SEG_E 2
#define SEG_F A0
#define SEG_G A1
#define P1 9
#define P2 8
#define P3 7
#define P4 6
void muxSetup(){
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(P3, OUTPUT);
  pinMode(P4, OUTPUT);
}
boolean mux[][7] = {
  //0 a-f on, g off
  {
    true,true,true,true,true,true,false
  },
  //1
  {
    false,true,true,false,false,false,false
  },
  {
    true,true,true,true,false,false,true
  },
  //2
  {
    true,true,false,true,true,false,true
  },
  //4
  {
    false,true,true,false,false,true,true
  },
  //5
  {
    true,false,true,true,false,true,true
  },
  //6
  {
    true,false,true,true,true,true,true
  },
  //7
  {
    true,true,true,false,false,false,false
  },
  //8
  {
    true,true,true,true,true,true,true
  },
  //9
  {
    true,true,true,false,false,true,true
  }
  
};
int curValue = 0;
int curDigit = 0;
void pinSetup(int v){
  digitalWrite(SEG_A, !mux[v][0]);
  digitalWrite(SEG_B, !mux[v][1]);
  digitalWrite(SEG_C, !mux[v][2]);
  digitalWrite(SEG_D, !mux[v][3]);
  digitalWrite(SEG_E, !mux[v][4]);
  digitalWrite(SEG_F, !mux[v][5]);
  digitalWrite(SEG_G, !mux[v][6]);
}
void refresh(){
  refreshDigit(curDigit);
  curDigit++;
  if(curDigit>3){ curDigit = 0; }
}
void refreshDigit(int digit){
  int v;
  digitalWrite(P1, LOW);
  digitalWrite(P2, LOW);
  digitalWrite(P3, LOW);
  digitalWrite(P4, LOW);
  switch(digit){
    case 0:
      v = curValue/1000;
      digitalWrite(P1, HIGH);
      break;
    case 1:
      v = (curValue%1000)/100;
      digitalWrite(P2, HIGH);
      break;
    case 2:
      v = ((curValue%1000)%100)/10;
      digitalWrite(P3, HIGH);
      break;
    case 3:
      v = ((curValue%1000)%100)%10;
      digitalWrite(P4, HIGH);
      break;
  }
  pinSetup(v);
}
void setValue(int value){
  curValue = value;
}

