
#include "CH_HID_Arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial softSerial(/*RX*/4, /*TX*/5);

#define CH_RESET_PIN 6

void setup() {

  softSerial.begin(57600);
  ch_hid.begin(&softSerial, CH_RESET_PIN);//Both args not needed, will default to Serial and no CH55x reset

  Keyboard.print("Hallo");
  
  delay(1000);
  
  Keyboard.press(KEY_LEFT_ARROW);
  Keyboard.release(KEY_LEFT_ARROW);
  //Keyboard.releaseAll();

  //Mouse.click(MOUSE_LEFT); // Click Mouse button
  //Mouse.move(0, -40); // Move mouse X Y
  //Mouse.move(0, 0, 3); // Mouse wheel plus 3
  //Mouse.move(0, 0, -3); // Mouse wheel minus 3

  //ch_hid.reset();
  //ch_hid.bootloader();

}


void loop() {
  //nothing to do here.
}