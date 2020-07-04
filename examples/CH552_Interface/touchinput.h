#pragma once 

#define   BIT0    (0X01)
#define   BIT1    (0X02)
#define   BIT2    (0X04)
#define   BIT3    (0X08)
#define   BIT4    (0X10)
#define   BIT5    (0X20)
#define   BIT6    (0X40)
#define   BIT7    (0X80)

#define   CHX       (0X00)
#define   CH2       (BIT2)
#define   CH3       (BIT3)
#define   CH_FREE     (0x07)

#define   TH_VALUE    (100)
#define   TOUCH_NUM   (0x04)
#define   SAMPLE_TIMES  (0x05)

__xdata uint8_t   TK_Code[TOUCH_NUM] = {
  0x03, 0x04,
};

__xdata uint16_t      Key_FreeBuf[TOUCH_NUM];
__xdata uint8_t volatile      Touch_IN;

uint8_t TK_SelectChannel( uint8_t ch )
{
  if ( ch <= TOUCH_NUM )
  {
    TKEY_CTRL = ( TKEY_CTRL & 0XF8) | TK_Code[ch];
    return 1;
  }

  return  0;
}

uint8_t TK_Init( uint8_t channel)
{

  __xdata uint8_t   i, j;
  __xdata uint16_t  sum;
  __xdata uint16_t  OverTime;

  P1_DIR_PU &= ~channel;
  P1_MOD_OC &= ~channel;
  TKEY_CTRL |= bTKC_2MS ;

  for ( i = 0; i < TOUCH_NUM; i++ )
  {
    sum = 0;
    j = SAMPLE_TIMES;
    TK_SelectChannel( i );
    while ( j-- )
    {
      OverTime = 0;
      while ( ( TKEY_CTRL & bTKC_IF ) == 0 )
      {
        if ( ++OverTime == 0 )
        {
          return 0;
        }
      }
      sum += TKEY_DAT;
    }
    Key_FreeBuf[i] = sum / SAMPLE_TIMES;
  }
  IE_TKEY = 1;
  return 1;
}

void  mTKEYInterrupt( void ) __interrupt (INT_NO_TKEY)
{
  __xdata static uint8_t ch = 0;
  __xdata uint16_t KeyData;
  KeyData = TKEY_DAT;

  if ( KeyData < ( Key_FreeBuf[ch] - TH_VALUE ) )
  {
    Touch_IN |=  1 << ( TK_Code[ch] - 1 );
  }
  if ( ++ch >= TOUCH_NUM )
  {
    ch = 0;
  }
  TK_SelectChannel( ch );
}

void init_touch(){  
  TK_Init( BIT4 + BIT5);
  TK_SelectChannel(0);
}
