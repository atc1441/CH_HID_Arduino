#include "intTemp.h"
#include "time.h"
#include "usb.h"
#include "touchinput.h"
#include "uart.h"
#include "bootloader.h"

uint32_t last;

#define LED_PIN1 7 // Pin7
SBIT(LED1, 0x90, LED_PIN1); // Port1

void setup()
{
  init_freq();
  init_usb();
  init_touch();
  init_millis();
  init_uart();
  EA  = 1;// Enable Interrupts
  delay(250);
  sendAckMSG('B');
}

void loop() {
   if ((millis() - last) > 40) {
    last = millis();
    LED1 = !LED1;
  }
 if ( Touch_IN != 0 )
  {
    if ( Touch_IN & CH2 )jump_to_bootloader();
    if ( Touch_IN & CH3 )sendAckMSG('t');
    Touch_IN = 0;
  }

  processUSB();
  processUart();
}
