#include "CH_HID_Arduino.h"

void setup() {
	
  ch_hid.begin();
  
  Keyboard.print("Hallo");
  
  //Mouse.click(MOUSE_LEFT);  
  //Mouse.move(0, -40);
  
  //ch_hid.reset();
  //ch_hid.bootloader();
  
}


void loop() {
	//nothing to do here.
}