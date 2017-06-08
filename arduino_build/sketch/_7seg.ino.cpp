#include <Arduino.h>
#line 1 "/home/boss/Arduino/_7seg/_7seg.ino"
#line 1 "/home/boss/Arduino/_7seg/_7seg.ino"

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

char str[70];
char buf;

char* cmd_buf[CLI_MAX_CMD_BUF];
uint8_t cmd_ind = 0;

char disp_buf[DISP_SIZE] = {'F','F','F','F'};
uint8_t disp_ind = 0;

#line 23 "/home/boss/Arduino/_7seg/_7seg.ino"
void setup();
#line 46 "/home/boss/Arduino/_7seg/_7seg.ino"
void loop();
#line 86 "/home/boss/Arduino/_7seg/_7seg.ino"
static uint8_t* convert_char(char c);
#line 133 "/home/boss/Arduino/_7seg/_7seg.ino"
static uint8_t pinSetup();
#line 17 "/home/boss/Arduino/_7seg/_cli.ino"
CLI_STS lw_cli(char c);
#line 55 "/home/boss/Arduino/_7seg/_cli.ino"
CLI_STS lw_cli_parse(char* _buf);
#line 89 "/home/boss/Arduino/_7seg/_cli.ino"
CLI_STS lw_cli_prompt();
#line 104 "/home/boss/Arduino/_7seg/_cli.ino"
CLI_STS lw_cli_init();
#line 114 "/home/boss/Arduino/_7seg/_cli.ino"
void read_reg(void);
#line 118 "/home/boss/Arduino/_7seg/_cli.ino"
void write_reg(void);
#line 122 "/home/boss/Arduino/_7seg/_cli.ino"
void quit(void);
#line 23 "/home/boss/Arduino/_7seg/_7seg.ino"
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

#line 1 "/home/boss/Arduino/_7seg/_cli.ino"
/**
   @file _cli.c
   @author DAA
   @date 06/07/2017
   @brief Basic configurable CLI.
   @see _cli.h
*/
#include "_cli.h"

static char cli_store[CLI_MAX_STORE];
static char cli_buf[CLI_MAX_CMD_BUF];
uint8_t cli_store_ind = 0;
uint8_t cli_buf_ind = 0;
bool prompt_set = false;
bool cli_mutex = false;

CLI_STS lw_cli(char c) {
  CLI_STS ret = CLI_OK;
  if (!cli_mutex) {
    uint8_t i;
    
    if (lw_cli_prompt() != CLI_OK) {
      G_CLI_STATUS = CLI_ERROR;
      return CLI_ERROR;
    } 
    else if (cli_buf_ind > CLI_MAX_CMD_BUF) {
      G_CLI_STATUS = CLI_OUTOFBOUNDS;
      return CLI_OUTOFBOUNDS;
    }
    else if (c == 0xd || (c >= 0x20 && c <= 0x7E)) {
      //handle legitimate chars
      Serial.print(c);
      if (IS_NEWLINE(c)) {
        ret = lw_cli_parse(cli_buf);    
        lw_cli_init();
      } 
      else {
        //add to buffer
        cli_buf[cli_buf_ind++] = c;
      }
    } 
    else if (c == 0x7f && cli_buf_ind>0) {
      //backspace
      cli_buf[--cli_buf_ind] = '\0';
      
      prompt_set = false;
      Serial.print("\n\r");
      lw_cli_prompt();
      Serial.print(cli_buf);
    }
  }
  return ret;
}

CLI_STS lw_cli_parse(char* _buf) {
  uint8_t i,j=0;
  char _i_buf[CLI_MAX_CMD_BUF];
  cli_store_ind = 0;

  //split up cmd words
  for (i = 0;i < cli_buf_ind; i++) {
    if ( _buf[i] == ' ' ) {
      cli_store_ind++;
      j=0;
      if (cli_store_ind >= CLI_MAX_CMDS) { return CLI_OUTOFBOUNDS; }
    }
    else {
      //sprintf(str, "\n\rcli_store : %02u\n\rind : %d",cli_store[cli_store_ind],cli_store_ind);
      //Serial.print(str);
      cli_store[cli_store_ind * CLI_MAX_CMD_BUF + j++] = _buf[i];
    }
  }  
  //populate command storage
  for (i=0;i<=cli_store_ind;i++) {
    j=0;
    memset(_i_buf,'\0',CLI_MAX_CMD_BUF);
    while (char c=cli_store[i * CLI_MAX_CMD_BUF + j]) {
      if (j >= CLI_MAX_CMD_BUF) { return CLI_OUTOFBOUNDS; }
      _i_buf[j++] = c;
    }
    Serial.print("\n\r");
    Serial.print(_i_buf);
  }

  //start parsing commands finally
  return CLI_OK;
}

CLI_STS lw_cli_prompt() {
  if (!Serial)
  {
    G_CLI_STATUS = CLI_ERROR;
    return CLI_ERROR;
  }
  if (!prompt_set)
  {
    Serial.print(CLI_PROMPT);
    prompt_set = true;
  }
  G_CLI_STATUS = CLI_OK;
  return CLI_OK;
}

CLI_STS lw_cli_init() {
  memset(cli_buf, 0, CLI_MAX_CMD_BUF);
  memset(cli_store,'\0',CLI_MAX_STORE);
  cli_buf_ind = 0;
  cli_store_ind = 0;
  prompt_set = false;
  Serial.print("\n\r");
  lw_cli_prompt();
}

void read_reg(void) {
  Serial.print("\n\rread_reg");
  return -1;
}
void write_reg(void) {
  Serial.print("\n\rwrite_reg");
  return -1;
}
void quit(void) {
  Serial.print("\n\rquit");
  return -1;
}

