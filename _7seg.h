/**
 * @file _7seg.h
 * @author DAA
 * @date 06/07/2017
 * @brief 7 Segment Display Library headers.
 * @see _7seg.ino
 */
#include "_cli.h"

#ifndef _7SEG_H_
#define _7SEG_H_
//buffers
#define MAX_STR_BUF 70
//pin connections
#define PIN_DEC     2
#define PIN_A       3
#define PIN_B       4
#define PIN_C       5
#define PIN_D       6
#define PIN_E       7 
#define PIN_F       8 
#define PIN_G       9
#define PIN_SEL1    10
#define PIN_SEL2    11
#define PIN_SEL3    12
#define PIN_SEL4    13
#define PIN_STS     PIN_SEL4

static uint8_t seg_pins[8] = {PIN_A,PIN_B,PIN_C,PIN_D,PIN_E,PIN_F,PIN_G};
static uint8_t sel_pins[4] = {PIN_SEL1,PIN_SEL2,PIN_SEL3,PIN_SEL4};

//#bits per character
#define C_LEN      7
//# times to blink at state
#define SDELAY      250
#define S1          2
//size of display buffer
#define DISP_SIZE   4
#define STS_OK      0
#define STS_ERROR   1
//define to enable full alphabet (WIP)
#undef  ADDITIONAL_CHARS

static uint8_t c_SPC[] =     {0,0,0,0,0,0,0};
static uint8_t c_0[] =       {1,1,1,1,1,1,0};
static uint8_t c_1[] =       {0,1,1,0,0,0,0};
static uint8_t c_2[] =       {1,1,0,1,1,0,1};
static uint8_t c_3[] =       {1,1,1,1,0,0,1};
static uint8_t c_4[] =       {0,1,1,0,0,1,1};
static uint8_t c_5[] =       {1,0,1,1,0,1,1};
static uint8_t c_6[] =       {1,0,1,1,1,1,1};
static uint8_t c_7[] =       {1,1,1,0,0,0,0};
static uint8_t c_8[] =       {1,1,1,1,1,1,1};
static uint8_t c_9[] =       {1,1,1,1,0,1,1};
static uint8_t c_A[] =       {1,1,1,0,1,1,1};
static uint8_t c_B[] =       {0,0,1,1,1,1,1};
static uint8_t c_C[] =       {1,0,0,1,1,1,0};
static uint8_t c_D[] =       {0,1,1,1,1,0,1};
static uint8_t c_E[] =       {1,0,0,1,1,1,1};
static uint8_t c_F[] =       {1,0,0,0,1,1,1};
//ADDTIONAL CHARS
#ifdef ADDITIONAL_CHARS
  static uint8_t c_G[] =       {1,0,0,0,1,1,1};
  static uint8_t c_H[] =       {1,0,0,0,1,1,1};
  static uint8_t c_I[] =       {1,0,0,0,1,1,1};
  static uint8_t c_J[] =       {1,0,0,0,1,1,1};
  static uint8_t c_K[] =       {1,0,0,0,1,1,1};
  static uint8_t c_L[] =       {1,0,0,0,1,1,1};
  static uint8_t c_M[] =       {1,0,0,0,1,1,1};
  static uint8_t c_N[] =       {1,0,0,0,1,1,1};
  static uint8_t c_O[] =       {1,0,0,0,1,1,1};
  static uint8_t c_P[] =       {1,0,0,0,1,1,1};
  static uint8_t c_Q[] =       {1,0,0,0,1,1,1};
  static uint8_t c_R[] =       {1,0,0,0,1,1,1};
  static uint8_t c_S[] =       {1,0,0,0,1,1,1};
  static uint8_t c_T[] =       {1,0,0,0,1,1,1};
  static uint8_t c_U[] =       {1,0,0,0,1,1,1};
  static uint8_t c_V[] =       {1,0,0,0,1,1,1};
  static uint8_t c_W[] =       {1,0,0,0,1,1,1};
  static uint8_t c_X[] =       {1,0,0,0,1,1,1};
  static uint8_t c_Y[] =       {1,0,0,0,1,1,1};
  static uint8_t c_Z[] =       {1,0,0,0,1,1,1};
#endif /*ADDITIONAL_CHARS*/
#endif /*_7SEG_H_*/
