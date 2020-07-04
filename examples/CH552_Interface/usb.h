#pragma once

#include <Arduino.h>

void init_usb();
void HIDValueHandle();
void processUSB();
void USBDeviceInit();
void CH554USBDevWakeup( );
void send_Keyboard_data_to_usb();
void send_Mouse_data_to_usb();
