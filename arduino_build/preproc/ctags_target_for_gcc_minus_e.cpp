# 1 "/home/boss/Arduino/_7seg/_7seg.ino"
# 1 "/home/boss/Arduino/_7seg/_7seg.ino"

/**
 * @file _7seg.ino
 * @author DAA
 * @date 06/06/2017
 * @brief 7 Segment Display Library.
 * @see _7seg.h
 */

# 11 "/home/boss/Arduino/_7seg/_7seg.ino" 2
# 12 "/home/boss/Arduino/_7seg/_7seg.ino" 2


char str[70];
char buf;

char* cmd_buf[12];
uint8_t cmd_ind = 0;

char disp_buf[4] = {'F','F','F','F'};
uint8_t disp_ind = 0;

void setup() {
  uint8_t i;
  if (pinSetup() == 0) {
    //blink status light S1 times to signal good setup
    for (i=0;i<2;i++){
      digitalWrite(13,0x0);
      delay(250);
      digitalWrite(13,0x1);
      delay(250);
    }
    digitalWrite(3,0x1);
    digitalWrite(13,0x0);
  }
  else { while(1) { ; } }
  memset(cmd_buf, 0, 12);
  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.print("Hello!\n\r");
  digitalWrite(13,0x1);

  lw_cli_init();
}

void loop() {
  uint8_t i,j;
  buf = '\0';
  while (Serial.available() > 0) {
    buf = Serial.read();
    disp_buf[disp_ind++] = buf;
    if (disp_ind >= 4) {
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
      memset(cmd_buf, 0, 12);
      //clear serial buffer
      while (Serial.available() > 0)
      { Serial.read(); } //clear serial input buf
    }
  }
  //set display
  for(i=0;i<4;i++) {
    for (j=0;j<7;j++) {
      digitalWrite(seg_pins[j], convert_char(disp_buf[i])[j]);
    }
    digitalWrite(2,(disp_ind==i)?0x1:0x0); //light up decimal pin in cur item
    digitalWrite(sel_pins[i],0x0);
    delay(5);
    digitalWrite(sel_pins[i],0x1);
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
  pinMode(2,0x1);
  pinMode(3,0x1);
  pinMode(4,0x1);
  pinMode(5,0x1);
  pinMode(6,0x1);
  pinMode(7,0x1);
  pinMode(8,0x1);
  pinMode(9,0x1);
  //set up selectors
  pinMode(10,0x1);
  pinMode(11,0x1);
  pinMode(12,0x1);
  pinMode(13,0x1);
  //active high segs, active low select
  digitalWrite(3,0x1);
  digitalWrite(4,0x1);
  digitalWrite(5,0x1);
  digitalWrite(6,0x1);
  digitalWrite(7,0x1);
  digitalWrite(8,0x1);
  digitalWrite(9,0x1);
  digitalWrite(10,0x1);
  digitalWrite(11,0x1);
  digitalWrite(12,0x1);
  digitalWrite(13,0x1);
  return 0;
}
# 1 "/home/boss/Arduino/_7seg/_cli.ino"
/**
   @file _cli.c
   @author DAA
   @date 06/07/2017
   @brief Basic configurable CLI.
   @see _cli.h
*/
# 9 "/home/boss/Arduino/_7seg/_cli.ino" 2

static char cli_store[12 * 4];
static char cli_buf[12];
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
    else if (cli_buf_ind > 12) {
      G_CLI_STATUS = CLI_OUTOFBOUNDS;
      return CLI_OUTOFBOUNDS;
    }
    else if (c == 0xd || (c >= 0x20 && c <= 0x7E)) {
      //handle legitimate chars
      Serial.print(c);
      if (c == '\n' || c == '\r' || (byte)c == 0x0d) {
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
  char _i_buf[12];
  cli_store_ind = 0;

  //split up cmd words
  for (i = 0;i < cli_buf_ind; i++) {
    if ( _buf[i] == ' ' ) {
      cli_store_ind++;
      j=0;
      if (cli_store_ind >= 4) { return CLI_OUTOFBOUNDS; }
    }
    else {
      //sprintf(str, "\n\rcli_store : %02u\n\rind : %d",cli_store[cli_store_ind],cli_store_ind);
      //Serial.print(str);
      cli_store[cli_store_ind * 12 + j++] = _buf[i];
    }
  }
  //populate command storage
  for (i=0;i<=cli_store_ind;i++) {
    j=0;
    memset(_i_buf,'\0',12);
    while (char c=cli_store[i * 12 + j]) {
      if (j >= 12) { return CLI_OUTOFBOUNDS; }
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
    Serial.print("-> ");
    prompt_set = true;
  }
  G_CLI_STATUS = CLI_OK;
  return CLI_OK;
}

CLI_STS lw_cli_init() {
  memset(cli_buf, 0, 12);
  memset(cli_store,'\0',12 * 4);
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
