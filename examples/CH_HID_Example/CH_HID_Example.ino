#include "CH_HID_Arduino.h"

void setup() {

  ch_hid.begin(/*Reset CH*/6);//Reset pin is not needed and can be left out, this is to put the CH in an known state.

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
