/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the user application layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_log.h"
#include "usbh_hid_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "usb_conf.h"


/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file is the Header file for usbh_usr.c
* @{
*/ 


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/ 
#define KYBRD_FIRST_COLUMN               (uint16_t)319
#define KYBRD_LAST_COLUMN                (uint16_t)7
#define KYBRD_FIRST_LINE                 (uint8_t)80
#define KYBRD_LAST_LINE                  (uint8_t)180


/**
* @}
*/ 
extern HID_Machine_TypeDef        HID_Machine;
extern  int16_t  x_loc, y_loc; 
extern __IO int16_t  prev_x, prev_y;
extern uint8_t USBFS_EnumDone;
extern __IO uint8_t  DEMO_HID_ShowData;
uint8_t  DEMO_StartHID = 0 ;

uint8_t  Xpos;
uint16_t Ypos;


/** @addtogroup USER
* @{
*/

/** @defgroup USBH_USR 
* @brief    This file includes the user application layer
* @{
*/ 

/** @defgroup USBH_CORE_Exported_Types
* @{
*/ 



/** @defgroup USBH_HID_USR_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_HID_USR_Private_Macros
* @{
*/ 
#define  LCD_UsrLog2(...)                   LCD_LineColor = Yellow;\
                                            printf(__VA_ARGS__);\
/**
* @}
*/ 

/** @defgroup USBH_HID_USR_Private_Variables
* @{
*/
extern USB_OTG_CORE_HANDLE           USB_OTG_FS_Core_dev;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_HID_cb =
{
  USBH_HID_USR_Init,
  USBH_HID_USR_DeInit,
  USBH_HID_USR_DeviceAttached,
  USBH_HID_USR_ResetDevice,
  USBH_HID_USR_DeviceDisconnected,
  USBH_HID_USR_OverCurrentDetected,
  USBH_HID_USR_DeviceSpeedDetected,
  USBH_HID_USR_Device_DescAvailable,
  USBH_HID_USR_DeviceAddressAssigned,
  USBH_HID_USR_Configuration_DescAvailable,
  USBH_HID_USR_Manufacturer_String,
  USBH_HID_USR_Product_String,
  USBH_HID_USR_SerialNum_String,
  USBH_HID_USR_EnumerationDone,
  USBH_HID_USR_UserInput,
  NULL,
  USBH_HID_USR_DeviceNotSupported,
  USBH_HID_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_HID_USR_Private_Constants
* @{
*/ 
/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]          = ">Port1:Host Library Initialized\n";
const uint8_t MSG_HID_DEV_ATTACHED[]       = ">Port1:Device Attached\n";
const uint8_t MSG_HID_DEV_DISCONNECTED[]   = ">Port1:Device Disconnected\n";
const uint8_t MSG_HID_DEV_ENUMERATED[]     = ">Port1:Enumeration completed\n";
const uint8_t MSG_HID_DEV_HIGHSPEED[]      = ">Port1:High speed device detected\n";
const uint8_t MSG_HID_DEV_FULLSPEED[]      = ">Port1:Full speed device detected\n";
const uint8_t MSG_HID_DEV_LOWSPEED[]       = ">Port1:Low speed device detected\n";
const uint8_t MSG_HID_DEV_ERROR[]          = ">Port1:Device fault \n";
const uint8_t MSG_HID_CLASS[]              = ">Port1:HID device connected\n";

const uint8_t USB_HID_MouseStatus[]    = ">Port1:HID Demo Device : Mouse\n";
const uint8_t USB_HID_KeybrdStatus[]   = ">Port1:HID Demo Device : Keyboard\n";
const uint8_t MSG_UNREC_ERROR[]        = ">Port1:UNRECOVERED ERROR STATE\n";
/**
* @}
*/



/** @defgroup USBH_HID_USR_Private_FunctionPrototypes
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_HID_USR_Private_Functions
* @{
*/ 





/**
* @brief  USBH_HID_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_HID_USR_Init(void)
{
  
}

/**
* @brief  USBH_HID_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_HID_USR_DeviceAttached(void)
{  
}


/**
* @brief  USBH_HID_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_HID_USR_UnrecoveredError (void)
{
  LCD_ErrLog((void*)MSG_UNREC_ERROR);
}

/**
* @brief  USBH_HID_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_HID_USR_DeInit(void)
{
  
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_HID_USR_DeviceDisconnected (void)
{
  LCD_ErrLog((void *)MSG_HID_DEV_DISCONNECTED);
  USBFS_EnumDone= 0;
}

/**
* @brief  USBH_HID_USR_ResetUSBDevice 
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_HID_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_HID_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_HID_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    LCD_UsrLog2((void *)MSG_HID_DEV_HIGHSPEED);
  }  
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    LCD_UsrLog2((void *)MSG_HID_DEV_FULLSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    LCD_UsrLog2((void *)MSG_HID_DEV_LOWSPEED);
  }
  else
  {
    LCD_UsrLog2((void *)MSG_HID_DEV_ERROR);
  }
}

/**
* @brief  USBH_HID_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_HID_USR_Device_DescAvailable(void *DeviceDesc)
{

}

/**
* @brief  USBH_HID_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_HID_USR_DeviceAddressAssigned(void)
{
  
}


/**
* @brief  USBH_HID_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_HID_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;
  
  id = itfDesc;  
  
 if((*id).bInterfaceClass  == 0x03)
  {
    LCD_UsrLog2((void *)MSG_HID_CLASS);
  }   
}

/**
* @brief  USBH_HID_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_HID_USR_Manufacturer_String(void *ManufacturerString)
{
  char temp[100];
  sprintf(temp, ">Port1:Manufacturer : %s\n", (char *)ManufacturerString);
  LCD_UsrLog2((void *)temp);
  
}

/**
* @brief  USBH_HID_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_HID_USR_Product_String(void *ProductString)
{
  char temp[100];
  sprintf((char *)temp, ">Port1:Product : %s\n", (char *)ProductString);  
  LCD_UsrLog2((void *)temp);
  
}

/**
* @brief  USBH_HID_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_HID_USR_SerialNum_String(void *SerialNumString)
{
  uint8_t temp[100];
  sprintf((char *)temp, ">Port1:Serial Number : %s\n", (char *)SerialNumString);    
  LCD_UsrLog2((void *)temp);
} 

/**
* @brief  EnumerationDone 
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_HID_USR_EnumerationDone(void)
{
  /* Enumeration complete */
  LCD_UsrLog2((void *)MSG_HID_DEV_ENUMERATED);
  USBFS_EnumDone = 1;

} 

/**
* @brief  USBH_HID_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_HID_USR_DeviceNotSupported(void)
{
  LCD_UsrLog2 (">Port1:Device not supported.\n");              
  
}  


/**
* @brief  USBH_HID_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_HID_USR_UserInput(void)
{
  
  USBH_USR_Status usbh_usr_status;
  
  usbh_usr_status = USBH_USR_NO_RESP;  
  
  /*Key B3 is in polling mode to detect user action */
  if(DEMO_StartHID) 
  {
    
    usbh_usr_status = USBH_USR_RESP_OK;
    
  }
  
  
  return usbh_usr_status;
  
} 

/**
* @brief  USBH_HID_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_HID_USR_OverCurrentDetected (void)
{
  LCD_ErrLog (">Port1:Overcurrent detected.\n");
  
}

/**
* @brief  USR_MOUSE_Init
*         Init Mouse window
* @param  None
* @retval None
*/
void USR_MOUSE_Init	(void)
{
  
  if(DEMO_HID_ShowData == 0)
  {
    LCD_UsrLog((void*)USB_HID_MouseStatus); 
    
    /* Display Mouse Window */
    LCD_DrawRect(MOUSE_WINDOW_X,
                 MOUSE_WINDOW_Y, 
                 MOUSE_WINDOW_HEIGHT,
                 MOUSE_WINDOW_WIDTH);
    
    HID_MOUSE_ButtonReleased(0);
    HID_MOUSE_ButtonReleased(1);
    HID_MOUSE_ButtonReleased(2);
    
    
    LCD_SetTextColor(LCD_COLOR_GREEN);
    LCD_SetBackColor(LCD_COLOR_BLACK);
    
    LCD_DisplayChar(MOUSE_WINDOW_X + 1,
                    MOUSE_WINDOW_Y - 1,
                    'x');
    x_loc  = 0;
    y_loc  = 0; 
    prev_x = 0;
    prev_y = 0;
  }
  
}

/**
* @brief  USR_MOUSE_ProcessData
*         Process Mouse data
* @param  data : Mouse data to be displayed
* @retval None
*/
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{
  
  uint8_t idx = 1;   
  static uint8_t b_state[3] = { 0, 0 , 0};
  char temp[20];

  if(DEMO_HID_ShowData == 0)
  {  
  if ((data->x != 0) && (data->y != 0))
  {
    HID_MOUSE_UpdatePosition(data->x , data->y);
  }
  
  for ( idx = 0 ; idx < 3 ; idx ++)
  {
    
    if(data->button & 1 << idx) 
    {
      if(b_state[idx] == 0)
      {
        HID_MOUSE_ButtonPressed (idx);
        b_state[idx] = 1;
      }
    }
    else
    {
      if(b_state[idx] == 1)
      {
        HID_MOUSE_ButtonReleased (idx);
        b_state[idx] = 0;
      }
    }
  }
  }
  else
  {
    sprintf(temp ,"> %02xh %02xh %02xh %02xh\n" , HID_Machine.buff[0],  
                                         HID_Machine.buff[1],  
                                         HID_Machine.buff[2],  
                                         HID_Machine.buff[3]);
    LCD_DbgLog(temp);
    
  }
  
  
}

/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void  USR_KEYBRD_Init (void)
{
  

  Xpos  = KYBRD_FIRST_LINE;
  Ypos  = KYBRD_FIRST_COLUMN;

  if(DEMO_HID_ShowData == 0)
  {
    LCD_UsrLog((void*)USB_HID_KeybrdStatus); 
    LCD_UsrLog("\n______________ USER TEXT ______________\n"); 
  }  
}


/**
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be displayed
* @retval None
*/
void  USR_KEYBRD_ProcessData (uint8_t data)
{
 
  char temp[20];
    
  if(DEMO_HID_ShowData == 0)
  {
  LCD_SetTextColor(LCD_COLOR_GREEN);
  
  if(data == '\n')
  {
    

    Ypos = KYBRD_FIRST_COLUMN;
    
    /*Increment char X position*/
    Xpos+=SMALL_FONT_LINE_WIDTH;
    
  }
  else if (data == '\r')
  {
    /* Manage deletion of character and update cursor location*/
    if( Ypos == KYBRD_FIRST_COLUMN) 
    {
      /*First character of first line to be deleted*/
      if(Xpos == KYBRD_FIRST_LINE)
      {  
        Ypos =KYBRD_FIRST_COLUMN; 
      }
      else
      {
        Xpos-=SMALL_FONT_LINE_WIDTH;
        Ypos =(KYBRD_LAST_COLUMN+SMALL_FONT_COLUMN_WIDTH); 
      }
    }
    else
    {
      Ypos +=SMALL_FONT_COLUMN_WIDTH;                  
      
    } 
    LCD_DisplayChar(Xpos,Ypos, ' '); 
  }
  else
  {
    LCD_DisplayChar(Xpos,Ypos, data);    
    /* Update the cursor position on LCD */
    
    /*Increment char Y position*/
    Ypos -=SMALL_FONT_COLUMN_WIDTH;
    
    /*Check if the Y position has reached the last column*/
    if(Ypos == KYBRD_LAST_COLUMN)
    {
      
      Ypos = KYBRD_FIRST_COLUMN;
      
      /*Increment char X position*/
      
      
      Xpos+=SMALL_FONT_LINE_WIDTH;
      
            
      if( Xpos > KYBRD_LAST_LINE) 
      {
        Xpos =KYBRD_FIRST_LINE;
      }
      
    }
  }
  }
  else
  {
    sprintf(temp ,"> %02xh %02xh %02xh %02xh\n" , HID_Machine.buff[0],  
                                         HID_Machine.buff[1],  
                                         HID_Machine.buff[2],  
                                         HID_Machine.buff[3]);
    LCD_DbgLog(temp);
  }
}

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

