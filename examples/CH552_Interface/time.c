#include "time.h"

#ifndef FREQ_SYS
#define FREQ_SYS  24000000
#endif

void  init_freq()
{
  SAFE_MOD = 0x55;
  SAFE_MOD = 0xAA;

#if FREQ_SYS == 32000000
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x07;  // 32MHz
#elif FREQ_SYS == 24000000
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x06;  // 24MHz
#elif FREQ_SYS == 26000000
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x05;  // 16MHz
#elif FREQ_SYS == 12000000
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x04;  // 12MHz
#elif FREQ_SYS == 6000000
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x03;  // 6MHz
#elif FREQ_SYS == 3000000
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x02;  // 3MHz
#elif FREQ_SYS == 750000
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x01;  // 750KHz
#elif FREQ_SYS == 187500
  CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x00;  // 187.5MHz
#else
#warning FREQ_SYS invalid or not set
#endif

  SAFE_MOD = 0x00;
  delay(5);
}

void  delayUs(uint16_t n)
{
#ifdef  FREQ_SYS
#if   FREQ_SYS <= 6000000
  n >>= 2;
#endif
#if   FREQ_SYS <= 3000000
  n >>= 2;
#endif
#if   FREQ_SYS <= 750000
  n >>= 4;
#endif
#endif
  while ( n ) {  // total = 12~13 Fsys cycles, 1uS @Fsys=12MHz
    ++ SAFE_MOD;  // 2 Fsys cycles, for higher Fsys, add operation here
#ifdef  FREQ_SYS
#if   FREQ_SYS >= 14000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 16000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 18000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 20000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 22000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 24000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 26000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 28000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 30000000
    ++ SAFE_MOD;
#endif
#if   FREQ_SYS >= 32000000
    ++ SAFE_MOD;
#endif
#endif
    -- n;
  }
}

void  delay(uint16_t n)
{
  while ( n ) {
    delayUs( 1000 );
    -- n;
  }
}

uint32_t millis_int;

void init_millis() {
  TMOD = 0x11;
  TH0 = (65536 - 2000) / 256; // fuer Startwert
  TL0 = (65536 - 2000) % 256; // fuer Startwert
  TR0 = 1;    // Timer 0 starten  - TR1 fuer Timer 1
  ET0 = 1;    // Interrupt fuer Timer 0 aktivieren
}

void  mTMR0Interrupt( void ) __interrupt (INT_NO_TMR0)
{
  TH0 = (65536 - 2000) / 256; // Reload
  TL0 = (65536 - 2000) % 256; // Reload
  millis_int++;
}

uint32_t millis(){
  return millis_int;
}
