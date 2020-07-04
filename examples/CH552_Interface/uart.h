#pragma once

#ifndef  UART0_BUAD
#define  UART0_BUAD    57600
#endif

void processUart();
void sendNumMSG(uint8_t new_num);
void sendAckMSG( uint8_t type);

void init_uart(void);
