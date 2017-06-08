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
