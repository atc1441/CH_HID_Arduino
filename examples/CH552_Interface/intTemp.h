#pragma once

//Comment the one you want to use on your sketch

void  mINT0Interrupt( void ) __interrupt (INT_NO_INT0) {}
//void  mTMR0Interrupt( void ) __interrupt (INT_NO_TMR0 ) {}
void  mINT1Interrupt( void ) __interrupt (INT_NO_INT1 ) {}
void  mTMR1Interrupt( void ) __interrupt (INT_NO_TMR1 ) {}
void  mUART0Interrupt( void ) __interrupt (INT_NO_UART0) {}
void  mTMR20Interrupt( void ) __interrupt (INT_NO_TMR2 ) {}
void  mSPI0Interrupt( void ) __interrupt (INT_NO_SPI0 ) {}
//void  mTKEYInterrupt( void ) __interrupt (INT_NO_TKEY ) {}
//void  mUSBInterrupt( void ) __interrupt (INT_NO_USB  ) {}
void  mADCInterrupt( void ) __interrupt (INT_NO_ADC  ) {}
void  mUART1Interrupt( void ) __interrupt (INT_NO_UART1) {}
void  mPWMXInterrupt( void ) __interrupt (INT_NO_PWMX ) {}
void  mGPIOInterrupt( void ) __interrupt (INT_NO_GPIO ) {}
void  mWDOGInterrupt( void ) __interrupt (INT_NO_WDOG ) {}
