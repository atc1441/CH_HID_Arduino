/*
   Mouse.cpp and Keyboard.cpp

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett
  Modified by Aaron Christophel - ATCnetz.de

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "CH_HID_Arduino.h"

#define USB_WAITING 60
//================================================================================
//================================================================================
//  CH552 raw stuff

void CH_HID_::begin(int8_t _reset_ch_pin)
{
  if(_reset_ch_pin >= 0){
  pinMode(_reset_ch_pin,OUTPUT);
  digitalWrite(_reset_ch_pin,HIGH);
  delay(400);
  digitalWrite(_reset_ch_pin,LOW);
  delay(500);
  }
  Serial.begin(57600);
  delay(100);
  sync();// send one time sign to discard uart garbage
}

void CH_HID_::sync()
{
  send_sign();
}

void CH_HID_::reset()
{
  Serial.print("Reset");
  send_sign();
}

void CH_HID_::bootloader()
{
  Serial.print("Boot");
  send_sign();
}

void CH_HID_::send_sign() {
  Serial.write('A');// Signature
  Serial.write('T');// Signature
  Serial.write('C');// Signature
  Serial.write('\r');// reset line
  Serial.write('\n');// new line
  delay(5);
}
	
CH_HID_ ch_hid;

//================================================================================
//================================================================================
//  Mouse

void Mouse_::click(uint8_t b)
{
  Serial.write('M');// M for mouse
  Serial.write(0x11);// click and release
  Serial.write(b);// mouse buttons bit whise
  Serial.write(0);// X Axis signed int8
  Serial.write(0);// Y Axis signed int8
  Serial.write(0);// Mouse Wheel signed int8
  Serial.write('A');// Signature
  Serial.write('T');// Signature
  Serial.write('C');// Signature
  Serial.write('\r');// reset line
  Serial.write('\n');// new line
  delay(USB_WAITING);
}

void Mouse_::move(signed char x, signed char y, signed char wheel)
{
  Serial.write('M');// M for mouse
  Serial.write(0x00);// click and hold
  Serial.write(_buttons);// mouse buttons bit whise
  Serial.write(x);// X Axis signed int8
  Serial.write(y);// Y Axis signed int8
  Serial.write(wheel);// Mouse Wheel signed int8
  Serial.write('A');// Signature
  Serial.write('T');// Signature
  Serial.write('C');// Signature
  Serial.write('\r');// reset line
  Serial.write('\n');// new line
  delay(USB_WAITING);
}

void Mouse_::buttons(uint8_t b)
{
  if (b != _buttons)
  {
    _buttons = b;
    move(0,0,0);
  }
}

void Mouse_::press(uint8_t b) 
{
  buttons(_buttons | b);
}

void Mouse_::release(uint8_t b)
{
  buttons(_buttons & ~b);
}

bool Mouse_::isPressed(uint8_t b)
{
  if ((b & _buttons) > 0) 
    return true;
  return false;
}

Mouse_ Mouse;

//================================================================================
//================================================================================
//  Keyboard

void Keyboard_::sendReport(KeyReport* keys, bool with_release)
{
  Serial.write('K');// K for keyboard
  Serial.write((with_release ? 0x11 : 0x00));// click and release or clicked and hold
  Serial.write(keys->modifiers);
  Serial.write(0x00);
  Serial.write(keys->keys[0]);
  Serial.write(keys->keys[1]);
  Serial.write(keys->keys[2]);
  Serial.write(keys->keys[3]);
  Serial.write(keys->keys[4]);
  Serial.write(keys->keys[5]);
  Serial.write('A');// Signature
  Serial.write('T');// Signature
  Serial.write('C');// Signature
  Serial.write('\r');// reset line
  Serial.write('\n');// new line
  delay(USB_WAITING);
}

extern
const uint8_t _asciimap[128] PROGMEM;

#define SHIFT 0x80
const uint8_t _asciimap[128] =
{
  0x00,             // NUL
  0x00,             // SOH
  0x00,             // STX
  0x00,             // ETX
  0x00,             // EOT
  0x00,             // ENQ
  0x00,             // ACK
  0x00,             // BEL
  0x2a,     // BS Backspace
  0x2b,     // TAB  Tab
  0x28,     // LF Enter
  0x00,             // VT
  0x00,             // FF
  0x00,             // CR
  0x00,             // SO
  0x00,             // SI
  0x00,             // DEL
  0x00,             // DC1
  0x00,             // DC2
  0x00,             // DC3
  0x00,             // DC4
  0x00,             // NAK
  0x00,             // SYN
  0x00,             // ETB
  0x00,             // CAN
  0x00,             // EM
  0x00,             // SUB
  0x00,             // ESC
  0x00,             // FS
  0x00,             // GS
  0x00,             // RS
  0x00,             // US

  0x2c,      //  ' '
  0x1e | SHIFT,  // !
  0x34 | SHIFT,  // "
  0x20 | SHIFT,  // #
  0x21 | SHIFT,  // $
  0x22 | SHIFT,  // %
  0x24 | SHIFT,  // &
  0x34,          // '
  0x26 | SHIFT,  // (
  0x27 | SHIFT,  // )
  0x25 | SHIFT,  // *
  0x2e | SHIFT,  // +
  0x36,          // ,
  0x2d,          // -
  0x37,          // .
  0x38,          // /
  0x27,          // 0
  0x1e,          // 1
  0x1f,          // 2
  0x20,          // 3
  0x21,          // 4
  0x22,          // 5
  0x23,          // 6
  0x24,          // 7
  0x25,          // 8
  0x26,          // 9
  0x33 | SHIFT,    // :
  0x33,          // ;
  0x36 | SHIFT,    // <
  0x2e,          // =
  0x37 | SHIFT,    // >
  0x38 | SHIFT,    // ?
  0x1f | SHIFT,    // @
  0x04 | SHIFT,    // A
  0x05 | SHIFT,    // B
  0x06 | SHIFT,    // C
  0x07 | SHIFT,    // D
  0x08 | SHIFT,    // E
  0x09 | SHIFT,    // F
  0x0a | SHIFT,    // G
  0x0b | SHIFT,    // H
  0x0c | SHIFT,    // I
  0x0d | SHIFT,    // J
  0x0e | SHIFT,    // K
  0x0f | SHIFT,    // L
  0x10 | SHIFT,    // M
  0x11 | SHIFT,    // N
  0x12 | SHIFT,    // O
  0x13 | SHIFT,    // P
  0x14 | SHIFT,    // Q
  0x15 | SHIFT,    // R
  0x16 | SHIFT,    // S
  0x17 | SHIFT,    // T
  0x18 | SHIFT,    // U
  0x19 | SHIFT,    // V
  0x1a | SHIFT,    // W
  0x1b | SHIFT,    // X
  0x1c | SHIFT,    // Y
  0x1d | SHIFT,    // Z
  0x2f,          // [
  0x31,          // bslash
  0x30,          // ]
  0x23 | SHIFT,  // ^
  0x2d | SHIFT,  // _
  0x35,          // `
  0x04,          // a
  0x05,          // b
  0x06,          // c
  0x07,          // d
  0x08,          // e
  0x09,          // f
  0x0a,          // g
  0x0b,          // h
  0x0c,          // i
  0x0d,          // j
  0x0e,          // k
  0x0f,          // l
  0x10,          // m
  0x11,          // n
  0x12,          // o
  0x13,          // p
  0x14,          // q
  0x15,          // r
  0x16,          // s
  0x17,          // t
  0x18,          // u
  0x19,          // v
  0x1a,          // w
  0x1b,          // x
  0x1c,          // y
  0x1d,          // z
  0x2f | SHIFT,  // {
  0x31 | SHIFT,  // |
  0x30 | SHIFT,  // }
  0x35 | SHIFT,  // ~
  0       // DEL
};

// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// USB HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t Keyboard_::press(uint8_t k, bool with_release)
{
  uint8_t i;
  if (k >= 136) {     // it's a non-printing key (not a modifier)
    k = k - 136;
  } else if (k >= 128) {  // it's a modifier key
    _keyReport.modifiers |= (1 << (k - 128));
    k = 0;
  } else {        // it's a printing key
    k = pgm_read_byte(_asciimap + k);
    if (!k) {
      setWriteError();
      return 0;
    }
    if (k & 0x80) {           // it's a capital letter or other character reached with shift
      _keyReport.modifiers |= 0x02; // the left shift modifier
      k &= 0x7F;
    }
  }

  // Add k to the key report only if it's not already present
  // and if there is an empty slot.
  if (_keyReport.keys[0] != k && _keyReport.keys[1] != k &&
      _keyReport.keys[2] != k && _keyReport.keys[3] != k &&
      _keyReport.keys[4] != k && _keyReport.keys[5] != k) {

    for (i = 0; i < 6; i++) {
      if (_keyReport.keys[i] == 0x00) {
        _keyReport.keys[i] = k;
        break;
      }
    }
    if (i == 6) {
      setWriteError();
      return 0;
    }
  }
  sendReport(&_keyReport, with_release);
  return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t Keyboard_::release(uint8_t k, bool with_send)
{
  uint8_t i;
  if (k >= 136) {     // it's a non-printing key (not a modifier)
    k = k - 136;
  } else if (k >= 128) {  // it's a modifier key
    _keyReport.modifiers &= ~(1 << (k - 128));
    k = 0;
  } else {        // it's a printing key
    k = pgm_read_byte(_asciimap + k);
    if (!k) {
      return 0;
    }
    if (k & 0x80) {             // it's a capital letter or other character reached with shift
      _keyReport.modifiers &= ~(0x02);  // the left shift modifier
      k &= 0x7F;
    }
  }

  // Test the key report to see if k is present.  Clear it if it exists.
  // Check all positions in case the key is present more than once (which it shouldn't be)
  for (i = 0; i < 6; i++) {
    if (0 != k && _keyReport.keys[i] == k) {
      _keyReport.keys[i] = 0x00;
    }
  }

  if (with_send)sendReport(&_keyReport);
  return 1;
}

void Keyboard_::releaseAll(void)
{
  _keyReport.keys[0] = 0;
  _keyReport.keys[1] = 0;
  _keyReport.keys[2] = 0;
  _keyReport.keys[3] = 0;
  _keyReport.keys[4] = 0;
  _keyReport.keys[5] = 0;
  _keyReport.modifiers = 0;
  sendReport(&_keyReport);
}

size_t Keyboard_::write(uint8_t c)
{
  uint8_t p = press(c, true); // Keydown
  release(c, false);           // Keyup
  return p;              // just return the result of press() since release() almost always returns 1
}

size_t Keyboard_::write(const uint8_t *buffer, size_t size) {
  size_t n = 0;
  while (size--) {
    if (*buffer != '\r') {
      if (write(*buffer)) {
        n++;
      } else {
        break;
      }
    }
    buffer++;
  }
  return n;
}

Keyboard_ Keyboard;
