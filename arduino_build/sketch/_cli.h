/**
 * @file _cli.h
 * @author DAA
 * @date 06/07/2017
 * @brief 7 Segment Display Library headers.
 * @see _cli.ino
 */

#ifndef _CLI_H_
#define _CLI_H_

#define IS_NEWLINE(c)     c == '\n' || c == '\r' || (byte)c == 0x0d   
#define IS_SPACE(c)       c == ' '

#define CLI_DEFAULT_CMDS_
#define CLI_MAX_CMD_BUF   12
#define CLI_MAX_CMDS      4
#define CLI_MAX_STORE     CLI_MAX_CMD_BUF * CLI_MAX_CMDS
#define CLI_PROMPT        "-> "

typedef enum {
  CLI_INIT = -1,
  CLI_OK,
  CLI_ERROR,
  CLI_UNKNOWNCMD,
  CLI_OUTOFBOUNDS,
  CLI_STOPPED
} CLI_STS;
static CLI_STS G_CLI_STATUS = CLI_INIT;

typedef struct {
  char _name[CLI_MAX_CMD_BUF]; 
  void* fn;
  CLI_STS sts;
} cmd_t;

#ifdef CLI_DEFAULT_CMDS_
  void write_reg(void);
  void read_reg(void);
  void quit(void);
  void help(void);
  static cmd_t cmds[] {
    { "w",  write_reg,  CLI_INIT },
    { "r",  read_reg,   CLI_INIT },
    { "q",  quit,       CLI_INIT },
    { "?",  help,       CLI_INIT },
    NULL
  };
#endif /*CLI_DEFAULT_CMDS_*/
#endif /*_CLI_H_*/
