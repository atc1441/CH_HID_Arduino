#include "usb.h"
#include "time.h"
#include "uart.h"

#define THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE

__xdata __at (0x0000) uint8_t  Ep0Buffer[DEFAULT_ENDP0_SIZE];
__xdata __at (0x0008) uint8_t  Ep1Buffer[DEFAULT_ENDP1_SIZE];
__xdata __at (0x0010) uint8_t  Ep2Buffer[2 * MAX_PACKET_SIZE];

uint8_t   SetupReq, SetupLen, USB_Connected, Count, FLAG, UsbConfig;
uint8_t *pDescr;
USB_SETUP_REQ   SetupReqBuf;

uint8_t new_num, data_num;

#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)

#define   THIS_ENDP0_SIZE         DEFAULT_ENDP0_SIZE
#define   BUFFER_SIZE       64
#define   DUAL_BUFFER_SIZE    128
#define   UsbSetupBuf         ((PUSB_SETUP_REQ)Ep0Buffer)

__code uint8_t DevDesc[18] =
{ 0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x08,
  0x3d, 0x41, 0x07, 0x21, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01
};
__code uint8_t CfgDesc[59] =
{
  0x09, 0x02, 0x3b, 0x00, 0x02, 0x01, 0x00, 0xA0, 0x32,
  0x09, 0x04, 0x00, 0x00, 0x01, 0x03, 0x01, 0x01, 0x00,
  0x09, 0x21, 0x11, 0x01, 0x00, 0x01, 0x22, 0x3e, 0x00,
  0x07, 0x05, 0x81, 0x03, 0x08, 0x00, 0x0a,
  0x09, 0x04, 0x01, 0x00, 0x01, 0x03, 0x01, 0x02, 0x00,
  0x09, 0x21, 0x10, 0x01, 0x00, 0x01, 0x22, 0x34, 0x00,
  0x07, 0x05, 0x82, 0x03, 0x04, 0x00, 0x0a
};
__code uint8_t KeyRepDesc[62] =
{
  0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07,
  0x19, 0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25, 0x01,
  0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01,
  0x75, 0x08, 0x81, 0x01, 0x95, 0x03, 0x75, 0x01,
  0x05, 0x08, 0x19, 0x01, 0x29, 0x03, 0x91, 0x02,
  0x95, 0x05, 0x75, 0x01, 0x91, 0x01, 0x95, 0x06,
  0x75, 0x08, 0x26, 0xff, 0x00, 0x05, 0x07, 0x19,
  0x00, 0x29, 0x91, 0x81, 0x00, 0xC0
};
__code uint8_t MouseRepDesc[52] =
{
  0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x09, 0x01,
  0xA1, 0x00, 0x05, 0x09, 0x19, 0x01, 0x29, 0x03,
  0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x03,
  0x81, 0x02, 0x75, 0x05, 0x95, 0x01, 0x81, 0x01,
  0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x09, 0x38,
  0x15, 0x81, 0x25, 0x7f, 0x75, 0x08, 0x95, 0x03,
  0x81, 0x06, 0xC0, 0xC0
};

uint8_t HIDMouse[4] = {0x00, 0x00, 0x00, 0x00};
uint8_t HIDKey[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void init_usb()
{
  USBDeviceInit();
}

void HIDValueHandle()
{
  UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;
  UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;
}

void processUSB() {
  if (USB_Connected)
  {
    HIDValueHandle();
    if (new_num) {
      sendNumMSG(data_num);
      new_num = 0;
    }
  }
}

void USBDeviceInit()
{
  IE_USB = 0;
  USB_CTRL = 0x00;
  UEP2_DMA = (uint16_t)Ep2Buffer;
  UEP2_3_MOD = UEP2_3_MOD & ~bUEP2_BUF_MOD | bUEP2_TX_EN;
  UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
  UEP0_DMA = (uint16_t)Ep0Buffer;
  UEP4_1_MOD &= ~(bUEP4_RX_EN | bUEP4_TX_EN);
  UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
  UEP1_DMA = (uint16_t)Ep1Buffer;
  UEP4_1_MOD = UEP4_1_MOD & ~bUEP1_BUF_MOD | bUEP1_TX_EN;
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;

  USB_DEV_AD = 0x00;
  UDEV_CTRL = bUD_PD_DIS;
  USB_CTRL = bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;
  UDEV_CTRL |= bUD_PORT_EN;
  USB_INT_FG = 0xFF;
  USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
  IE_USB = 1;

  UEP1_T_LEN = 0;
  UEP2_T_LEN = 0;

  FLAG = 0;
  USB_Connected = 0;
}

void CH554USBDevWakeup( )
{
  UDEV_CTRL |= bUD_LOW_SPEED;
  delay(2);
  UDEV_CTRL &= ~bUD_LOW_SPEED;
}

void send_Keyboard_data_to_usb()
{
  memcpy( Ep1Buffer, HIDKey, sizeof(HIDKey));
  UEP1_T_LEN = sizeof(HIDKey);
  UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;
}

void send_Mouse_data_to_usb( )
{
  memcpy( Ep2Buffer, HIDMouse, sizeof(HIDMouse));
  UEP2_T_LEN = sizeof(HIDMouse);
  UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK;
}

void mUSBInterrupt(void) __interrupt (INT_NO_USB)
{
  uint8_t len = 0;
  if (UIF_TRANSFER)
  {
    switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
    {
      case UIS_TOKEN_IN | 2:
        UEP2_T_LEN = 0;
        UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;
        break;
      case UIS_TOKEN_IN | 1:
        UEP1_T_LEN = 0;
        UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;
        FLAG = 1;
        break;
      case UIS_TOKEN_SETUP | 0:
        len = USB_RX_LEN;
        if (len == (sizeof(USB_SETUP_REQ)))
        {
          SetupLen = UsbSetupBuf->wLengthL;
          if (UsbSetupBuf->wLengthH || SetupLen > 0x7F )
          {
            SetupLen = 0x7F;
          }
          len = 0;
          SetupReq = UsbSetupBuf->bRequest;
          if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )
          {
            switch ( SetupReq )
            {
              case 0x01://GetReport
                break;
              case 0x02://GetIdle
                break;
              case 0x03://GetProtocol
                break;
              case 0x09://SetReport
                break;
              case 0x0A://SetIdle
                break;
              case 0x0B://SetProtocol
                break;
              default:
                len = 0xFF;
                break;
            }
          }
          else
          {
            switch (SetupReq)
            {
              case USB_GET_DESCRIPTOR:
                switch (UsbSetupBuf->wValueH)
                {
                  case 1:
                    pDescr = DevDesc;
                    len = sizeof(DevDesc);
                    break;
                  case 2:
                    pDescr = CfgDesc;
                    len = sizeof(CfgDesc);
                    break;
                  case 0x22:
                    if (UsbSetupBuf->wIndexL == 0)
                    {
                      pDescr = KeyRepDesc;
                      len = sizeof(KeyRepDesc);
                    }
                    else if (UsbSetupBuf->wIndexL == 1)
                    {
                      pDescr = MouseRepDesc;
                      len = sizeof(MouseRepDesc);
                      USB_Connected = 1;
                    }
                    else
                    {
                      len = 0xff;
                    }
                    break;
                  default:
                    len = 0xff;
                    break;
                }
                if ( SetupLen > len )
                {
                  SetupLen = len;
                }
                len = SetupLen >= 8 ? 8 : SetupLen;
                memcpy(Ep0Buffer, pDescr, len);
                SetupLen -= len;
                pDescr += len;
                break;
              case USB_SET_ADDRESS:
                SetupLen = UsbSetupBuf->wValueL;
                break;
              case USB_GET_CONFIGURATION:
                Ep0Buffer[0] = UsbConfig;
                if ( SetupLen >= 1 )
                {
                  len = 1;
                }
                break;
              case USB_SET_CONFIGURATION:
                UsbConfig = UsbSetupBuf->wValueL;
                break;
              case 0x0A:
                break;
              case USB_CLEAR_FEATURE:
                if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )
                {
                  switch ( UsbSetupBuf->wIndexL )
                  {
                    case 0x82:
                      UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                      break;
                    case 0x81:
                      UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                      break;
                    case 0x01:
                      UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
                      break;
                    default:
                      len = 0xFF;
                      break;
                  }
                }
                if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_DEVICE )
                {
                  break;
                }
                else
                {
                  len = 0xFF;
                }
                break;
              case USB_SET_FEATURE:
                if ( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x00 )
                {
                  if ( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
                  {
                    if ( CfgDesc[ 7 ] & 0x20 )
                    {
                    }
                    else
                    {
                      len = 0xFF;
                    }
                  }
                  else
                  {
                    len = 0xFF;
                  }
                }
                else if ( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x02 )
                {
                  if ( ( ( ( uint16_t )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
                  {
                    switch ( ( ( uint16_t )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
                    {
                      case 0x82:
                        UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;
                        break;
                      case 0x02:
                        UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;
                        break;
                      case 0x81:
                        UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;
                        break;
                      default:
                        len = 0xFF;
                        break;
                    }
                  }
                  else
                  {
                    len = 0xFF;
                  }
                }
                else
                {
                  len = 0xFF;
                }
                break;
              case USB_GET_STATUS:
                Ep0Buffer[0] = 0x00;
                Ep0Buffer[1] = 0x00;
                if ( SetupLen >= 2 )
                {
                  len = 2;
                }
                else
                {
                  len = SetupLen;
                }
                break;
              default:
                len = 0xff;
                break;
            }
          }
        }
        else
        {
          len = 0xff;
        }
        if (len == 0xff)
        {
          SetupReq = 0xFF;
          UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;
        }
        else if (len)
        {
          UEP0_T_LEN = len;
          UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;
        }
        else
        {
          UEP0_T_LEN = 0;
          UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;
        }
        break;
      case UIS_TOKEN_IN | 0:
        switch (SetupReq)
        {
          case USB_GET_DESCRIPTOR:
            len = SetupLen >= 8 ? 8 : SetupLen;
            memcpy( Ep0Buffer, pDescr, len );
            SetupLen -= len;
            pDescr += len;
            UEP0_T_LEN = len;
            UEP0_CTRL ^= bUEP_T_TOG;
            break;
          case USB_SET_ADDRESS:
            USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
            UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
            break;
          default:
            UEP0_T_LEN = 0;
            UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
            break;
        }
        break;
      case UIS_TOKEN_OUT | 0:
        len = USB_RX_LEN;
        /*NumLock, Rollen, CapsLk*/
        data_num   = Ep0Buffer[0];
        new_num = 1;

        UEP0_CTRL ^= bUEP_R_TOG;
        break;
      default:
        break;
    }
    UIF_TRANSFER = 0;
  }
  if (UIF_BUS_RST)
  {
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
    UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
    USB_DEV_AD = 0x00;
    UIF_SUSPEND = 0;
    UIF_TRANSFER = 0;
    UIF_BUS_RST = 0;
  }
  if (UIF_SUSPEND)
  {
    UIF_SUSPEND = 0;
    if ( USB_MIS_ST & bUMS_SUSPEND )
    {
    }
  }
  else {
    USB_INT_FG = 0xFF;
  }
}
