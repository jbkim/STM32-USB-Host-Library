/**
  ******************************************************************************
  * @file    dual_func_demo.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file contain the demo implementation
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
#include <string.h>
#include "lcd_log.h"
#include "dual_func_demo.h"
#include "usbh_core.h"
#include "usbh_msc_usr.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbh_msc_core.h"
#include "usbd_msc_core.h"

#include "usb_conf.h"


/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_DUAL_FUNCT_DEMO
* @{
*/

/** @defgroup USBH_DUAL_FUNCT_DEMO 
* @brief    This file includes the usb host stack user callbacks
* @{
*/ 

/** @defgroup USBH_DUAL_FUNCT_DEMO_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_DUAL_FUNCT_DEMO_Private_Defines
* @{
*/ 
#  define IMAGE_BUFFER_SIZE  512

#  define BF_TYPE 0x4D42             /* "MB" */

#  define BI_RGB       0             /* No compression - straight BGR data */
#  define BI_RLE8      1             /* 8-bit run-length compression */
#  define BI_RLE4      2             /* 4-bit run-length compression */
#  define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

/**
* @}
*/ 


/** @defgroup USBH_DUAL_FUNCT_DEMO_Private_Macros
* @{
*/ 

/**
* @}
*/ 


/** @defgroup USBH_DUAL_FUNCT_DEMO_Private_Variables
* @{
*/ 


uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;
uint8_t Image_Buf[IMAGE_BUFFER_SIZE];

uint8_t filenameString[15]  = {0};
FATFS fatfs;
FIL file;
DEMO_StateMachine             demo;
uint8_t line_idx = 0; 
__IO uint8_t wait_user_input = 0; 
uint8_t Enum_Done      = 0;   
uint8_t * DEMO_main_menu[] = 
{
  "      1 - Host Demo                    ",
  "      2 - Device Demo                  ",
  "      3 - Credits                      ",  
};

uint8_t * DEMO_HOST_menu[] = 
{
  "      1 - Explore Flash content        ",
  "      2 - Write File to disk           ",
  "      3 - Show BMP file                ",  
  "      4 - Return                       ",  
};

uint8_t * DEMO_DEVICE_menu[] = 
{
  "      1 - Return                       ",  
  "                                       ",  
  "                                       ",
};

uint8_t writeTextBuff[] = "STM32 Connectivity line Host Demo application using FAT_FS   ";  

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_Core __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USBH_HOST               USB_Host __ALIGN_END ;

/**
* @}
*/

/** @defgroup USBH_DUAL_FUNCT_DEMO_Private_Constants
* @{
*/ 
static void Demo_Application (void);
static void Demo_SelectItem (uint8_t **menu , uint8_t item);
static uint8_t Explore_Disk (char* path , uint8_t recu_level);
static uint8_t Image_Browser (char* path);
static void     Show_Image(void);
static void Toggle_Leds(void);
static uint8_t Check_BMP_file(uint8_t *buf);
/**
* @}
*/



/** @defgroup USBH_DUAL_FUNCT_DEMO_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/ 


/** @defgroup USBH_DUAL_FUNCT_DEMO_Private_Functions
* @{
*/ 

/**
* @brief  Demo_Init 
*         Demo initialization
* @param  None
* @retval None
*/

void Demo_Init (void)
{
  
#if defined (USE_STM322xG_EVAL)
  STM322xG_LCD_Init();
#elif defined(USE_STM324xG_EVAL)
  STM324xG_LCD_Init();
#elif defined (USE_STM3210C_EVAL)
  STM3210C_LCD_Init();
#else
 #error "Missing define: Evaluation board (ie. USE_STM322xG_EVAL)"
#endif
  
  LCD_LOG_Init();
  LCD_LOG_SetHeader(" USB Manual DRD demo");
  LCD_UsrLog("> Initializing demo....\n");
  if(IOE_Config() == 0)
  {
    IOE_ITConfig(IOE_ITSRC_JOYSTICK ); 
  }
  else
  {
    LCD_ErrLog("Joystick cannot be initilialized.\n"); 
  }
  /* Configure the LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3); 
  STM_EVAL_LEDInit(LED4); 
    
  Demo_SelectItem (DEMO_main_menu, 0); 
  LCD_UsrLog("> Demo Initialized\n");
  LCD_UsrLog("> Use Joystick to Select demo.\n");
  
  DEMO_UNLOCK();  
}

/**
* @brief  Demo_Application 
*         Demo background task
* @param  None
* @retval None
*/

void Demo_Process (void)
{
  
  if(demo.state == DEMO_HOST)
  {
    if(HCD_IsDeviceConnected(&USB_OTG_Core))
    {
      USBH_Process(&USB_OTG_Core, &USB_Host); 
    }
  }
  Demo_Application();
  
}
/**
* @brief  Demo_ProbeKey 
*         Probe the joystick state
* @param  state : joystick state
* @retval None
*/

void Demo_ProbeKey (JOYState_TypeDef state)
{
  
  /*Explorer running */
  if((wait_user_input == 1)&&(state != JOY_NONE))
  {
    wait_user_input = 2;
    return;
  }
  
  /*Critical process ongoing : do not interrupt */
  if(DEMO_IS_LOCKED())
  {
    return;
  }

  if ((state == JOY_UP)&& (demo.select > 0))    
  {
    demo.select--;
  }
  else if (((state == JOY_DOWN) && (demo.select < 2)) || 
           ((demo.state == DEMO_HOST) && (state == JOY_DOWN) &&(demo.select < 3)))
  {
    
    if (!((demo.state == DEMO_DEVICE) && demo.select == 0))
    {
      demo.select++;
    }
  }
  else if (state == JOY_SEL)
  {
    demo.select  |= 0x80;
  }  
}

/**
* @brief  Demo_SelectItem 
*         manage the menu on the screen
* @param  menu : menu table
*         item : selected item to be highlighted
* @retval None
*/

static void Demo_SelectItem (uint8_t **menu , uint8_t item)
{
  
  
  LCD_SetTextColor(White);
  
  switch (item)
  {
  case 0 : 
    LCD_SetBackColor(Red);
    LCD_DisplayStringLine( LINE_01 ,menu [0]);
    LCD_SetBackColor(Blue);    
    LCD_DisplayStringLine( LINE_02, menu [1]);
    LCD_DisplayStringLine( LINE_03, menu [2]);
    
    break;
  case 1 : 
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine( LINE_01 ,menu [0]);
    LCD_SetBackColor(Red);    
    LCD_DisplayStringLine( LINE_02, menu [1]);
    LCD_SetBackColor(Blue);  
    LCD_DisplayStringLine( LINE_03, menu [2]); 
    
    break;
  case 2 : 
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine( LINE_01 ,menu [0]);
    LCD_SetBackColor(Blue);    
    LCD_DisplayStringLine( LINE_02, menu [1]);
    LCD_SetBackColor(Red);  
    LCD_DisplayStringLine( LINE_03, menu [2]); 
    
    break;
  case 3 : 
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine( LINE_01 ,menu [1]);
    LCD_SetBackColor(Blue);    
    LCD_DisplayStringLine( LINE_02, menu [2]);
    LCD_SetBackColor(Red);  
    LCD_DisplayStringLine( LINE_03, menu [3]); 
    
    break;    
  default:
    break;
  }
  LCD_SetBackColor(Black); 
  
}

/**
* @brief  Demo_Application 
*         Demo state machine
* @param  None
* @retval None
*/

static void Demo_Application (void)
{
  static uint8_t prev_select = 0;
  uint16_t bytesWritten, bytesToWrite;
  FRESULT res;
  
  switch (demo.state)
  {
  case  DEMO_IDLE:
    __disable_irq();
    Demo_SelectItem (DEMO_main_menu, 0); 
    __enable_irq();
    demo.state = DEMO_WAIT;
    demo.select = 0;
    break;    
  case  DEMO_WAIT:
    
    
    if(demo.select != prev_select)
    {
      prev_select = demo.select ;
      Demo_SelectItem (DEMO_main_menu, demo.select & 0x7F);
      
      /* Handle select item */
      if(demo.select & 0x80)
      {
        demo.select &= 0x7F;
        
        switch (demo.select)
        {
        case  0:
          LCD_LOG_ClearTextZone();
          demo.state = DEMO_HOST;  
          demo.Host_state = DEMO_HOST_IDLE;
          break;
          
        case 1:
          LCD_LOG_ClearTextZone ();          
          demo.state = DEMO_DEVICE;            
          demo.Device_state = DEMO_DEVICE_IDLE;          
          break;
          
        case 2:
          LCD_LOG_ClearTextZone();
          LCD_DbgLog("\nSystem Information :\n");  
          LCD_DbgLog("_________________________\n\n");  

#if defined (USE_STM322xG_EVAL)
          LCD_UsrLog("Board : STM322xG-Eval.\n");  
          LCD_UsrLog("Device: STM32F2xx.\n");  
          LCD_UsrLog("USB Host Library v2.1.0.\n");  
          LCD_UsrLog("USB Device Library v1.1.0.\n"); 
          LCD_UsrLog("USB OTG Driver v2.1.0\n");  
          LCD_UsrLog("STM32 Std Library v1.1.1.\n");          
#elif defined (USE_STM324xG_EVAL)
          LCD_UsrLog("Board : STM324xG-Eval.\n");  
          LCD_UsrLog("Device: STM32F4xx.\n");  
          LCD_UsrLog("USB Host Library v2.1.0.\n");  
          LCD_UsrLog("USB Device Library v1.1.0.\n"); 
          LCD_UsrLog("USB OTG Driver v2.1.0\n");  
          LCD_UsrLog("STM32 Std Library v1.0.1.\n"); 
#elif defined (USE_STM3210C_EVAL)
          LCD_UsrLog("Board : STM3210C-Eval.\n");  
          LCD_UsrLog("Device: STM32F10x.\n");  
          LCD_UsrLog("USB Host Library v2.1.0.\n");  
          LCD_UsrLog("USB Device Library v1.1.0.\n"); 
          LCD_UsrLog("USB OTG Driver v2.1.0\n");            
          LCD_UsrLog("STM32 Std Library V3.6.0.\n");  
#else
 #error "Missing define: Evaluation board (ie. USE_STM322xG_EVAL)"
#endif  /* USE_STM322xG_EVAL */
        
          demo.state = DEMO_IDLE;  
          break;
        default:
          break;
        }
      }
    }
    break;
  case  DEMO_HOST:  
    switch (demo.Host_state)
    {
    case  DEMO_HOST_IDLE:
      DEMO_LOCK();
      
      /* Init HS Core  : Demo start in host mode*/
#ifdef USE_USB_OTG_HS
      LCD_DbgLog("> Initializing USB Host High speed...\n");    
#else
      LCD_DbgLog("> Initializing USB Host Full speed...\n");         
#endif
      USBH_Init(&USB_OTG_Core, 
#ifdef USE_USB_OTG_FS
                USB_OTG_FS_CORE_ID,
#elif defined USE_USB_OTG_HS
                USB_OTG_HS_CORE_ID,
#endif                
                &USB_Host,
                &USBH_MSC_cb, 
                &USR_USBH_MSC_cb);
      
      demo.Host_state = DEMO_HOST_WAIT;
      DEMO_UNLOCK();      
      break;

    case  DEMO_HOST_WAIT:
     
      if (!HCD_IsDeviceConnected(&USB_OTG_Core))
      {
        Demo_HandleDisconnect();
        LCD_ErrLog("Please, connect a device and try again.\n");
      }

      if(Enum_Done == 1)
      {
#ifdef USE_USB_OTG_HS
        LCD_DbgLog("> USB Host High speed initialized.\n");
#else
        LCD_DbgLog("> USB Host Full speed initialized.\n");        
#endif        
        
        /* Initialises the File System*/
        if ( f_mount( 0, &fatfs ) != FR_OK ) 
        {
          /* efs initialisation fails*/
          LCD_ErrLog("> Cannot initialize File System.\n");
        }
        
        LCD_UsrLog("> File System initialized.\n");
        LCD_UsrLog("> Disk capacity : %d Bytes\n", 
                   USBH_MSC_Param.MSCapacity * USBH_MSC_Param.MSPageLength);       
        Demo_SelectItem (DEMO_HOST_menu, 0);
        demo.select = 0; 
        Enum_Done = 2;
      }
      
      if(Enum_Done == 2)
      {
        if(demo.select != prev_select)
        {
          prev_select = demo.select ;
          USB_OTG_DisableGlobalInt(&USB_OTG_Core);
          Demo_SelectItem (DEMO_HOST_menu, demo.select & 0x7F);
          USB_OTG_EnableGlobalInt(&USB_OTG_Core);
          
          /* Handle select item */
          if(demo.select & 0x80)
          {
            demo.select &= 0x7F;
            switch (demo.select)
            {
            case  0:
              DEMO_LOCK();
              Explore_Disk("0:/", 1);
              line_idx = 0;  
              DEMO_UNLOCK();
              break;
            case 1:
              /* Writes a text file, STM32.TXT in the disk*/
              LCD_UsrLog("> Writing File to disk flash ...\n");
              if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
              {
                
                LCD_ErrLog ( "> Disk flash is write protected \n");
                USBH_USR_ApplicationState = USH_USR_FS_DRAW;
                break;
              }
              DEMO_LOCK();
              /* Register work area for logical drives */
              f_mount(0, &fatfs);
              
              if(f_open(&file, "0:STM32.TXT",FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
              { 
                /* Write buffer to file */
                bytesToWrite = sizeof(writeTextBuff); 
                res= f_write (&file, writeTextBuff, bytesToWrite, (void *)&bytesWritten);   
                
                if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/
                {
                  LCD_ErrLog("> STM32.TXT CANNOT be writen.\n");
                }
                else
                {
                  LCD_UsrLog("> 'STM32.TXT' file created\n");
                }
                
                /*close file and filesystem*/
                f_close(&file);
              }
              DEMO_UNLOCK();
              break;
            case  2:
              if (f_mount( 0, &fatfs ) != FR_OK ) 
              {
                /* fat_fs initialisation fails*/
                break;
              }
              Image_Browser("0:/");
              
              /*Clear windows */
              LCD_LOG_ClearTextZone();            
              LCD_LOG_SetHeader(" USB Manual DRD demo");
              LCD_UsrLog("> Slide show application closed.\n");
              break;
              
            case 3:
              Demo_HandleDisconnect();
              f_mount(0, NULL); 
              USB_OTG_StopHost(&USB_OTG_Core);
              
              /* Manage User disconnect operations*/
              USB_Host.usr_cb->DeviceDisconnected();
              
              /* Re-Initilaize Host for new Enumeration */
              USBH_DeInit(&USB_OTG_Core, &USB_Host);
              USB_Host.usr_cb->DeInit();
              USB_Host.class_cb->DeInit(&USB_OTG_Core, &USB_Host.device_prop);
              LCD_UsrLog("> Use Joystick to Select demo.\n");
              break;
              
            default:
              break;
            }
          }
        }
      }
      break;        
    default:
      break;
    }    
    break; 
  case  DEMO_DEVICE:
    
    
    switch (demo.Device_state)
    {
    case  DEMO_DEVICE_IDLE:
      DEMO_LOCK();
      USBD_Init(&USB_OTG_Core,
#ifdef USE_USB_OTG_FS
                USB_OTG_FS_CORE_ID,
#elif defined USE_USB_OTG_HS
                USB_OTG_HS_CORE_ID,
#endif 
                &USR_desc,
                &USBD_MSC_cb, 
                &USR_cb);
      
      
      demo.Device_state = DEMO_DEVICE_WAIT;  
      demo.select = 0;
      DEMO_UNLOCK();
      break;
      
    case  DEMO_DEVICE_WAIT:
      
      if(demo.select != prev_select)
      {
        prev_select = demo.select ;
        __disable_irq();
        Demo_SelectItem (DEMO_DEVICE_menu, demo.select & 0x7F);
        __enable_irq();
        
        /* Handle select item */
        if(demo.select & 0x80)
        {
          demo.select &= 0x7F;
          switch (demo.select)
          {
          case  0:
             
             __disable_irq();
            demo.state = DEMO_IDLE;
            demo.select = 0;
            LCD_LOG_ClearTextZone();
            LCD_UsrLog("> Device application closed.\n");
            __enable_irq();
            DCD_DevDisconnect (&USB_OTG_Core);
            USB_OTG_StopDevice(&USB_OTG_Core);               
            break;
          default:
            break;
          }
        }
      } 
      break;      
      
    default:
      break;
    }     
    break;      
  default:
    break;
  }
  
}


/**
* @brief  Explore_Disk 
*         Displays disk content
* @param  path: pointer to root path
* @retval None
*/
static uint8_t Explore_Disk (char* path , uint8_t recu_level)
{
  
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[14];
  
  
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    while(HCD_IsDeviceConnected(&USB_OTG_Core)) 
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) 
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }
      fn = fno.fname;      
      strcpy(tmp, fn);
      line_idx++;
      if(line_idx > 12)
      {
        line_idx = 0;
        wait_user_input = 1;
        LCD_SetTextColor(Green);
        LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 48, "Press any key to continue...");
        LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
        
        while((HCD_IsDeviceConnected(&USB_OTG_Core)) && (wait_user_input != 2))
        {
          Toggle_Leds();
        }
        STM_EVAL_LEDOff(LED1);
        STM_EVAL_LEDOff(LED2);
        STM_EVAL_LEDOff(LED3);
        STM_EVAL_LEDOff(LED4);
        LCD_ClearLine( LCD_PIXEL_HEIGHT - 48);
      } 
      wait_user_input = 0; 
      
      if(recu_level == 1)
      {
        LCD_DbgLog("   |__");
      }
      else if(recu_level == 2)
      {
        LCD_DbgLog("   |   |__");
      }
      if((fno.fattrib & AM_MASK) == AM_DIR)
      {
        strcat(tmp, "\n");
        LCD_UsrLog((void *)tmp);
      }
      else
      {
        strcat(tmp, "\n");        
        LCD_DbgLog((void *)tmp);
      }
      
      if(((fno.fattrib & AM_MASK) == AM_DIR)&&(recu_level == 1))
      {
        Explore_Disk(fn, 2);
      }
    }
  }
  return res;
}

/**
* @brief  Demo_HandleDisconnect
*         deinit demo and astart again the enumeration
* @param  None
* @retval None
*/
void Demo_HandleDisconnect (void)
{
  LCD_LOG_ClearTextZone();
  demo.state = DEMO_IDLE;
  USBH_DeInit(&USB_OTG_Core, &USB_Host);
  Enum_Done = 0;
  DEMO_UNLOCK();
}
/**
* @brief  Toggle_Leds
*         Toggle leds to shows user input state
* @param  None
* @retval None
*/
static void Toggle_Leds(void)
{
  static uint32_t i = 0;
  static uint8_t j = 0;
  
  if (i++ == 0x50000)
  {
    
    switch (j++)
    {
      
    case 0 : 
      STM_EVAL_LEDOn(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      break;
      
    case 1 : 
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOn(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      break;
      
    case 2 : 
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
      break;
      
    case 3 : 
      STM_EVAL_LEDOff(LED1);
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOn(LED4);
      j=0xff;
      break;
    }
    i = 0;
  }  
}

/**
* @brief  Show_Image 
*         Displays BMP image
* @param  None
* @retval None
*/
static void Show_Image(void)
{
  
  uint16_t i = 0;
  uint16_t numOfReadBytes = 0;
  FRESULT res; 
  
  LCD_SetDisplayWindow(239, 319, 240, 320);
  LCD_WriteReg(R3, 0x1008);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  
  
  if(Check_BMP_file(Image_Buf) > 0)
  {
    LCD_LOG_ClearTextZone();            
    LCD_LOG_SetHeader(" USB Manual DRD demo");
    LCD_ErrLog("Bad BMP Format.\n");
    return;
  }
  
  while (HCD_IsDeviceConnected(&USB_OTG_Core))
  {
    res = f_read(&file, Image_Buf, IMAGE_BUFFER_SIZE, (void *)&numOfReadBytes);
    if((numOfReadBytes == 0) || (res != FR_OK)) /*EOF or Error*/
    {
      break; 
    }
    for(i = 0 ; i < IMAGE_BUFFER_SIZE; i+= 2)
    {
      LCD_WriteRAM(Image_Buf[i+1] << 8 | Image_Buf[i]); 
    } 
  }
  
}


/**
* @brief  Show_Image 
*         launch the Image browser
* @param  path string
* @retval status
*/
static uint8_t Image_Browser (char* path)
{
  FRESULT res;
  uint8_t ret = 1;
  FILINFO fno;
  DIR dir;
  char *fn;
  
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) 
      {
        wait_user_input = 0;
        break;
      }  
      
      if (fno.fname[0] == '.') continue;

      fn = fno.fname;

      if (fno.fattrib & AM_DIR) 
      {
        continue;
      } 
      else 
      {
        if((strstr(fn, "bmp")) || (strstr(fn, "BMP")))
        {
          
          res = f_open(&file, fn, FA_OPEN_EXISTING | FA_READ);
          Show_Image();
          wait_user_input = 1;
          LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 12, "Press any key to continue...             ");
          LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
          
          while((HCD_IsDeviceConnected(&USB_OTG_Core)) && (wait_user_input != 2))
          {
            Toggle_Leds();
          }
          STM_EVAL_LEDOff(LED1);
          STM_EVAL_LEDOff(LED2);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED4);
          LCD_ClearLine( LCD_PIXEL_HEIGHT - 12);
          f_close(&file);
          wait_user_input = 0; 
          
        }
      }
    }  
  }
  return ret;
}

/**
* @brief  Check_BMP_file 
*         Displays BMP image
* @param  None
* @retval None
*/
static uint8_t Check_BMP_file(uint8_t *buf)
{
  uint16_t             numOfReadBytes = 0;
  uint16_t  Type;
  uint32_t  Width;  
  uint32_t  Height;
  uint16_t  Bpp;
  uint32_t  Compression ; 
  
  if(f_read(&file, buf, 54, (void *)&numOfReadBytes) != FR_OK) /*Error*/
  {
    return 1;
  }
  
  Type        = *(__packed uint16_t *)(buf + 0 );
  Width       = *(__packed uint32_t *)(buf + 18 );
  Height      = *(__packed uint32_t *)(buf + 22 );
  Bpp         = *(__packed uint16_t *)(buf + 28 );
  Compression = *(__packed uint32_t *)(buf + 30 );
  
  if(( Type != BF_TYPE)||
     ( Width != 320)||
       ( Height != 240)||
         ( Bpp != 16)||
           ( Compression != BI_BITFIELDS))
  {
    return 1;    
  }
  
  return 0;
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
