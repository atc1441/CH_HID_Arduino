## CH552 USB HID, Mouse and Keyboard Library for Arduino

This library allows an Arduino board with CH552 as interface to act as a USB Mouse and Keyboard.



To prepare the CH552 use the CH55xDuino library, install it by placing this link under your preferences in the Arduino IDE(Windows only for now)
https://raw.githubusercontent.com/atc1441/atc1441.github.io/master/package_ch55xduino_index.json
and install the CH55xDuino library via the Boards Manager.


Use the Zadig tool( https://zadig.akeo.ie/ ) to install the libusb-win32 driver for the CH55x in Bootloader mode to make it accessable via Arduino. 


Then open the "CH552_Interface" Example and click on upload to flash it to the CH55x connected via usb.

### You can support my work via paypal: https://paypal.me/hoverboard1 this keeps projects like this comming.

For more information about this library please visit

https://github.com/atc1441/CH_HID_Arduino

http://www.arduino.cc/en/Reference/Mouse

http://www.arduino.cc/en/Reference/Keyboard

== License from the Keyboard.h and Mouse.h these files have been integrated to work with the CH552 Interface ==

Copyright (c) Arduino LLC. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
