#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "time.h"
#include "uart.h"

uint8_t __xdata uartRxBuff[64];
uint8_t __xdata rxPos = 0;
extern uint8_t HIDKey[8], HIDMouse[4];
extern uint8_t   FLAG;

void processUart() {
  while (RI) {
    RI = 0;
    uartRxBuff[rxPos] = SBUF;
    if (uartRxBuff[rxPos - 4] == 'A' && uartRxBuff[rxPos - 3] == 'T' && uartRxBuff[rxPos - 2] == 'C' && uartRxBuff[rxPos - 1] == '\r' && uartRxBuff[rxPos] == '\n' || rxPos >= 64) {

      /*  for (uint8_t i = 0; i < rxPos; i ++ )
        {
          printf( "0x%02X ", uartRxBuff[i]);
        }
        printf("\n");*/

      if (uartRxBuff[0] == 'B' && uartRxBuff[1] == 'o' && uartRxBuff[2] == 'o' && uartRxBuff[3] == 't' && rxPos == 8)jump_to_bootloader();
      if (uartRxBuff[0] == 'R' && uartRxBuff[1] == 'e' && uartRxBuff[2] == 's' && uartRxBuff[3] == 'e' && uartRxBuff[4] == 't' && rxPos == 9)CH554SoftReset();

      if (uartRxBuff[0] == 'K' && rxPos == 14) {
        HIDKey[0] = uartRxBuff[2];
        HIDKey[1] = uartRxBuff[3];
        HIDKey[2] = uartRxBuff[4];
        HIDKey[3] = uartRxBuff[5];
        HIDKey[4] = uartRxBuff[6];
        HIDKey[5] = uartRxBuff[7];
        HIDKey[6] = uartRxBuff[8];
        HIDKey[7] = uartRxBuff[9];
        while (FLAG == 0);
        send_Keyboard_data_to_usb();
        while (FLAG == 0);
        delay(5);
        if (uartRxBuff[1] == 0x11) {
          HIDKey[0] = 0x00;
          HIDKey[1] = 0x00;
          HIDKey[2] = 0x00;
          HIDKey[3] = 0x00;
          HIDKey[4] = 0x00;
          HIDKey[5] = 0x00;
          HIDKey[6] = 0x00;
          HIDKey[7] = 0x00;
          while (FLAG == 0);
          send_Keyboard_data_to_usb();
          while (FLAG == 0);
          delay(5);
        }
        sendAckMSG('K');
      }
      if (uartRxBuff[0] == 'M' && rxPos == 10) {
        HIDMouse[0] = uartRxBuff[2];
        HIDMouse[1] = uartRxBuff[3];
        HIDMouse[2] = uartRxBuff[4];
        HIDMouse[3] = uartRxBuff[5];
        while (FLAG == 0);
        send_Mouse_data_to_usb();
        while (FLAG == 0);
        delay(5);
        if (uartRxBuff[1] == 0x11) {
          HIDMouse[0] = 0x00;
          HIDMouse[1] = 0x00;
          HIDMouse[2] = 0x00;
          HIDMouse[3] = 0x00;
          while (FLAG == 0);
          send_Mouse_data_to_usb();
          while (FLAG == 0);
          delay(5);
        }
        sendAckMSG('M');
      }
      rxPos = 0;
    } else {
      rxPos++;
    }
  }
}

void sendNumMSG( uint8_t new_num) {
  printf( "0x%02X ", 0x11);
  printf( "0x%02X ", new_num);
  putchar('\n');
}

void sendAckMSG( uint8_t type) {
  printf( "0x%02X ", 0xAA);
  printf( "0x%02X ", type);
  putchar('\n');
}

void init_uart(void) {
  uint32_t x;
  uint8_t x2;

  SM0 = 0;
  SM1 = 1;
  SM2 = 0;  // UART0 Mode 0

  // Use Timer1 as the baud rate generator
  RCLK = 0;
  TCLK = 0;
  PCON |= SMOD;
  x = 10 * FREQ_SYS / UART0_BUAD / 16;
  x2 = x % 10;
  x /= 10;
  if ( x2 >= 5 ) x ++;  // Round

  TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1; // Timer1: 8-bit auto reload
  T2MOD = T2MOD | bTMR_CLK | bT1_CLK; // Select input clock for Timer1
  TH1 = 0 - x;
  TR1 = 1;  // Start Timer1
  TI = 1;
  REN = 1;  // Enable UART1
}

int putchar(int c)
{
  while (!TI);
  TI = 0;
  SBUF = c & 0xFF;
  return c;
}

int getchar()
{
  while (!RI);
  RI = 0;
  return SBUF;
}
