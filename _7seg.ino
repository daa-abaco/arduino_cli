
/**
 * @file _7seg.ino
 * @author DAA
 * @date 06/06/2017
 * @brief 7 Segment Display Library.
 * @see _7seg.h
 */

#include "_7seg.h"
#include "_cli.h"
#define TERM_CHAR '\r'

char str[MAX_STR_BUF];
char buf;

char* cmd_buf[CLI_MAX_CMD_BUF];
uint8_t cmd_ind = 0;

char disp_buf[DISP_SIZE] = {'F','F','F','F'};
uint8_t disp_ind = 0;

void setup() {  
  uint8_t i;  
  if (pinSetup() == STS_OK) {
    //blink status light S1 times to signal good setup
    for (i=0;i<S1;i++){ 
      digitalWrite(PIN_STS,LOW);
      delay(SDELAY);
      digitalWrite(PIN_STS,HIGH);
      delay(SDELAY); 
    }
    digitalWrite(PIN_A,HIGH);
    digitalWrite(PIN_STS,LOW);
  } 
  else { while(1) { ; } }
  memset(cmd_buf, 0, CLI_MAX_CMD_BUF);
  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.print("Hello!\n\r");
  digitalWrite(PIN_STS,HIGH);

  lw_cli_init();
}

void loop() {
  uint8_t i,j;
  buf = '\0';
  while (Serial.available() > 0) {
    buf = Serial.read();
    disp_buf[disp_ind++] = buf;
    if (disp_ind >= DISP_SIZE) {
      disp_ind = 0; 
    }
    switch (lw_cli(buf)) {
      case CLI_OUTOFBOUNDS:
        Serial.print("\n\r--COMMAND TOO LARGE--");
        lw_cli_init();
        break;
      default:
        break;  
    }
    
    if (cmd_ind == 0) {
      //clear internal buffer
      memset(cmd_buf, 0, CLI_MAX_CMD_BUF);
      //clear serial buffer
      while (Serial.available() > 0) 
      { Serial.read(); }                               //clear serial input buf
    }
  }
  //set display
  for(i=0;i<DISP_SIZE;i++) {
    for (j=0;j<C_LEN;j++) {
      digitalWrite(seg_pins[j], convert_char(disp_buf[i])[j]);  
    }
    digitalWrite(PIN_DEC,(disp_ind==i)?HIGH:LOW); //light up decimal pin in cur item
    digitalWrite(sel_pins[i],LOW);
    delay(5);
    digitalWrite(sel_pins[i],HIGH);
  }
}



static uint8_t* convert_char(char c) {
  uint8_t ret[7];
  switch(c) {
    case '0':
      return c_0;
    case '1':
      return c_1;
    case '2':
      return c_2;
    case '3':
      return c_3;
    case '4':
      return c_4;
    case '5':
      return c_5;
    case '6':
      return c_6;
    case '7':
      return c_7;
    case '8':
      return c_8;
    case '9':
      return c_9;
    case 'A':
    case 'a':
      return c_A;
    case 'B':
    case 'b':
      return c_B;
    case 'C':
    case 'c':
      return c_C;
    case 'D':
    case 'd':
      return c_D;
    case 'E':
    case 'e':
      return c_E;
    case 'F':
    case 'f':
      return c_F;
    default:
      return c_SPC;
  }
  return ret;
}

static uint8_t pinSetup() {

  //set up control pins
  pinMode(PIN_DEC,OUTPUT);
  pinMode(PIN_A,OUTPUT);
  pinMode(PIN_B,OUTPUT);
  pinMode(PIN_C,OUTPUT);
  pinMode(PIN_D,OUTPUT);
  pinMode(PIN_E,OUTPUT);
  pinMode(PIN_F,OUTPUT);
  pinMode(PIN_G,OUTPUT);
  //set up selectors
  pinMode(PIN_SEL1,OUTPUT);
  pinMode(PIN_SEL2,OUTPUT);
  pinMode(PIN_SEL3,OUTPUT);
  pinMode(PIN_SEL4,OUTPUT);
  //active high segs, active low select
  digitalWrite(PIN_A,HIGH);
  digitalWrite(PIN_B,HIGH);
  digitalWrite(PIN_C,HIGH);
  digitalWrite(PIN_D,HIGH);
  digitalWrite(PIN_E,HIGH);
  digitalWrite(PIN_F,HIGH);
  digitalWrite(PIN_G,HIGH);
  digitalWrite(PIN_SEL1,HIGH);
  digitalWrite(PIN_SEL2,HIGH);
  digitalWrite(PIN_SEL3,HIGH);
  digitalWrite(PIN_SEL4,HIGH);
  return STS_OK;
}
