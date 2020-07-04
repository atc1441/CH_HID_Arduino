#pragma once

void jump_to_bootloader()
{
  disable_hardware();
  __asm
  LJMP BOOT_LOAD_ADDR
  __endasm;
  while (1);
}

void CH554SoftReset( )
{
  disable_hardware();
  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;
  GLOBAL_CFG  |= bSW_RESET;
}

void disable_hardware() {
  USB_INT_EN = 0;
  USB_CTRL = 0x06;
  TR0 = 0;
  ET0 = 0;
  UEP1_T_LEN = 0;
  UEP2_T_LEN = 0;
  TR1 = 0;  // Stop Timer1
  REN = 0;  // Disable UART0
  EA  = 0;
  delay(500);
}
