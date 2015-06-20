/**
  ******************************************************************************
  * @file    dual_core_demo.c
  * @author  MCD Application Team
  * @version V2.1.0
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
#include "usb_conf.h"
#include "lcd_log.h"
#include "dual_core_demo.h"
#include "usbh_core.h"
#include "usbh_msc_usr.h"
#include "usbh_hid_usr.h"
#include "usbh_msc_core.h"
#include "usbh_hid_core.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_DUAL_CORE_DEMO_
* @{
*/

/** @defgroup USBH_DUAL_CORE_DEMO_
* @brief    This file includes the  demo implementation
* @{
*/ 

/** @defgroup USBH_USR_Private_Types_Definitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_DUAL_CORE_DEMO_Private_Defines
* @{
*/ 
#define IMAGE_BUFFER_SIZE  512

#define BF_TYPE      0x4D42        /* "MB" */

#define BI_RGB       0             /* No compression - straight BGR data */
#define BI_RLE8      1             /* 8-bit run-length compression */
#define BI_RLE4      2             /* 4-bit run-length compression */
#define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

/**
* @}
*/ 


/** @defgroup USBH_DUAL_CORE_DEMO_Private_Macros
* @{
*/ 

/**
* @}
*/ 


/** @defgroup USBH_DUAL_CORE_DEMO_Private_Variables
* @{
*/ 


extern uint8_t  DEMO_StartHID ;
__IO uint8_t  DEMO_HID_ShowData  = 0;
uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;
uint8_t Image_Buf[IMAGE_BUFFER_SIZE];


uint8_t filenameString[15]  = {0};
FATFS fatfs;
FIL file;
DEMO_StateMachine             demo;
uint8_t line_idx = 0; 
__IO uint8_t wait_user_input = 0; 
uint8_t USBFS_EnumDone = 0;
uint8_t USBHS_EnumDone = 0;
uint8_t HS_DemoDisconnHandled = 0;
uint8_t FS_DemoDisconnHandled = 0;

uint8_t * DEMO_main_menu[] = 
{
  "      1 - Mass Storage demo            ",
  "      2 - Humain Interface demo        ",
  "      3 - Re-Enumerate                 ",  
};

uint8_t * DEMO_MSC_menu[] = 
{
  "      1 - Explore Flash content        ",
  "      2 - Write File to disk           ",
  "      3 - Show BMP file                ",  
  "      4 - Return                       ",  
};

uint8_t * DEMO_HID_menu[] = 
{
  "      1 - Start HID application        ",
  "      2 - Show HID report              ",
  "      3 - Return                       ",  
};

uint8_t writeTextBuff[] = "STM32 Connectivity line Host Demo application using FAT_FS   ";  


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE           USB_OTG_Core __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USBH_HOST                     USB_Host __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE           USB_OTG_FS_Core __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USBH_HOST                     USB_FS_Host __ALIGN_END ;

/**
* @}
*/

/** @defgroup USBH_DUAL_CORE_DEMO_Private_Constants
* @{
*/ 
static void Demo_Application (void);
static void Demo_SelectItem (uint8_t **menu , uint8_t item);
static uint8_t Explore_Disk (char* path , uint8_t recu_level);
static void Demo_HandleDisconnect (void);
static uint8_t Image_Browser (char* path);
static void     Show_Image(void);
static void Toggle_Leds(void);
static uint8_t Check_BMP_file(uint8_t *buf);
/**
* @}
*/



/** @defgroup USBH_DUAL_CORE_DEMO_Private_FunctionPrototypes
* @{
*/
/**
* @}
*/ 


/** @defgroup USBH_DUAL_CORE_DEMO_Private_Functions
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
  if(IOE_Config() == 0)
  {
    IOE_ITConfig(IOE_ITSRC_JOYSTICK ); 
  }
  /* Configure the LEDs */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3); 
  STM_EVAL_LEDInit(LED4); 
  
#if defined (USE_STM322xG_EVAL)
  STM322xG_LCD_Init();
#elif defined(USE_STM324xG_EVAL)
  STM324xG_LCD_Init();
#else
 #error "Missing define: Evaluation board (ie. USE_STM322xG_EVAL)"
#endif
  
  LCD_LOG_Init();
  
  LCD_LOG_SetHeader(" USB OTG DualCore Host");
  Demo_SelectItem (DEMO_main_menu, 0); 
  LCD_UsrLog("Initializing hardware....\n");
  
  /* Init HS Core */
  USBH_Init(&USB_OTG_Core, 
            USB_OTG_HS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb, 
            &USR_MSC_cb);
   LCD_UsrLog("USB Host High speed initialized.\n");
  /* Init FS Core */
  USBH_Init(&USB_OTG_FS_Core, 
            USB_OTG_FS_CORE_ID,
            &USB_FS_Host,
            &HID_cb, 
            &USR_HID_cb);  
  

  LCD_UsrLog("USB Host Full speed initialized.\n");
  USB_OTG_BSP_mDelay(500);
  LCD_LOG_ClearTextZone();
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
  
  /* Host Task handler */
  if((USBFS_EnumDone == 1) || (!HCD_IsDeviceConnected(&USB_OTG_FS_Core)))
  {
    USBH_Process(&USB_OTG_Core, &USB_Host);
  }
  
  USBH_Process(&USB_OTG_FS_Core, &USB_FS_Host); 
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
  
  /*Enumeration ongoing : do not interrupt */
  if (((!USBFS_EnumDone) && HCD_IsDeviceConnected(&USB_OTG_FS_Core)) ||
      ((!USBHS_EnumDone) && HCD_IsDeviceConnected(&USB_OTG_Core)))
  {
    return;
  }
  /*Class MAchine ongoing : do not interrupt */
  if(DEMO_IS_LOCKED())
  {
    return;
  }
  if ((state == JOY_UP)&& (demo.select > 0))
  {
    demo.select--;
  }
  else if (((state == JOY_DOWN) && (demo.select < 2)) ||
           ((demo.state == DEMO_MSC) && (state == JOY_DOWN) && (demo.select < 3)))           
  {
    demo.select++;
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
    Demo_SelectItem (DEMO_main_menu, 0); 
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
          
          if(HCD_IsDeviceConnected(&USB_OTG_Core))
          {
            demo.state = DEMO_MSC;            
            demo.msc_state = DEMO_MSC_IDLE;
          }
          else
          {
            LCD_ErrLog("No Mass storage device connected.\n");
          }
          break;
        case 1:
          
          if(HCD_IsDeviceConnected(&USB_OTG_FS_Core))
          {
            demo.state = DEMO_HID;
            demo.hid_state = DEMO_HID_IDLE;
          }
          else
          {
            LCD_ErrLog("No HID device connected.\n");
          }
          break;
        case 2:
          LCD_UsrLog("Starting Enumeration....\n");          
          if(HCD_IsDeviceConnected(&USB_OTG_FS_Core))
          {          
            
            DEMO_LOCK();
            USBFS_EnumDone = 0;
            USBH_DeInit(&USB_OTG_FS_Core,&USB_FS_Host);
            DEMO_UNLOCK();
          }
          else
          {
            LCD_ErrLog("No HID device connected.\n");
          }
          if(HCD_IsDeviceConnected(&USB_OTG_Core))
          {          
            LCD_UsrLog("Starting Enumeration....\n");
            DEMO_LOCK();
            USBHS_EnumDone = 0;
            USBH_DeInit(&USB_OTG_FS_Core,&USB_Host);
            DEMO_UNLOCK();
          }   
          else
          {
            LCD_ErrLog("No Mass storage device connected.\n");
          }
          break;
        default:
          break;
        }
      }
    }
    break;
  case  DEMO_MSC:  
    switch (demo.msc_state)
    {
    case  DEMO_MSC_IDLE:
      DEMO_LOCK();
      /* Initialises the File System*/
      if ( f_mount( 0, &fatfs ) != FR_OK ) 
      {
        /* efs initialisation fails*/
        LCD_ErrLog("> Cannot initialize File System.\n");
      }
      LCD_UsrLog("> File System initialized.\n");
      LCD_UsrLog("> Disk capacity : %d Bytes\n", 
                 USBH_MSC_Param.MSCapacity * USBH_MSC_Param.MSPageLength);       
      Demo_SelectItem (DEMO_MSC_menu, 0);
      demo.msc_state = DEMO_MSC_WAIT;
      demo.select = 0; 
      DEMO_UNLOCK();      
      break;
    case  DEMO_MSC_WAIT:
      if(demo.select != prev_select)
      {
        prev_select = demo.select ;
        Demo_SelectItem (DEMO_MSC_menu, demo.select & 0x7F);
        
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
            LCD_LOG_SetHeader(" USB OTG DualCore Host");
            LCD_UsrLog("> Slide show application closed.\n");
            break;
            
          case 3:
            demo.state = DEMO_IDLE;
            demo.select = 0;  
            f_mount(0, NULL); 
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
  case  DEMO_HID:
    switch (demo.hid_state)
    {
    case  DEMO_HID_IDLE:
      Demo_SelectItem (DEMO_HID_menu, 0);
      demo.hid_state = DEMO_HID_WAIT;
      demo.select = 0;         
      break;
    case  DEMO_HID_WAIT:
      
      if(demo.select != prev_select)
      {
        prev_select = demo.select ;
        Demo_SelectItem (DEMO_HID_menu, demo.select & 0x7F);
        
        /* Handle select item */
        if(demo.select & 0x80)
        {
          demo.select &= 0x7F;
          switch (demo.select)
          {
          case  0:
            LCD_LOG_ClearTextZone();
            HID_cb.DeInit(&USB_OTG_FS_Core, &USB_FS_Host);
            DEMO_HID_ShowData = 0;
            DEMO_StartHID = 1;
            HID_cb.Init(&USB_OTG_FS_Core, &USB_FS_Host);
            break;
          case 1:
            LCD_LOG_ClearTextZone();
            HID_cb.DeInit(&USB_OTG_FS_Core, &USB_FS_Host);
            DEMO_StartHID = 1;
            HID_cb.Init(&USB_OTG_FS_Core, &USB_FS_Host);
            DEMO_HID_ShowData = 1;
            break;
          case 2:
            demo.state = DEMO_IDLE;
            DEMO_StartHID = 0;
            DEMO_HID_ShowData = 0;
            demo.select = 0;
            LCD_LOG_ClearTextZone();
            HID_cb.DeInit(&USB_OTG_FS_Core, &USB_FS_Host);
            LCD_UsrLog("> HID application closed.\n");
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
  
  if (!(HCD_IsDeviceConnected(&USB_OTG_FS_Core)) && 
      (FS_DemoDisconnHandled == 0))
  { 
    FS_DemoDisconnHandled = 1;
    Demo_HandleDisconnect();
  }
  else if ((HCD_IsDeviceConnected(&USB_OTG_FS_Core)) && 
           (FS_DemoDisconnHandled == 1))
  {
    FS_DemoDisconnHandled = 0;
  }
  if (!(HCD_IsDeviceConnected(&USB_OTG_Core)) && 
      (HS_DemoDisconnHandled == 0))
  { 
    HS_DemoDisconnHandled = 1;
    Demo_HandleDisconnect();
  }  
  else if ((HCD_IsDeviceConnected(&USB_OTG_Core)) && 
           (HS_DemoDisconnHandled == 1))
  {
    HS_DemoDisconnHandled = 0;
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
    for (;;) {
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
static void Demo_HandleDisconnect (void)
{
  LCD_LOG_ClearTextZone();
  USBFS_EnumDone = 0;
  demo.state = DEMO_IDLE;
  demo.hid_state = DEMO_HID_IDLE;
  DEMO_StartHID = 0;
  DEMO_HID_ShowData = 0;
  HID_cb.DeInit(&USB_OTG_FS_Core, &USB_FS_Host);
  DEMO_LOCK();
  USBH_DeInit(&USB_OTG_Core,&USB_Host);
  USBH_DeInit(&USB_OTG_FS_Core,&USB_FS_Host);
  USBFS_EnumDone = 0;
  USBHS_EnumDone = 0;
  DEMO_UNLOCK();
  if (HCD_IsDeviceConnected(&USB_OTG_FS_Core) == 0)
  {
      LCD_ErrLog("Full speed device not connected.\n");
  }
  if (HCD_IsDeviceConnected(&USB_OTG_Core) == 0)
  {
      LCD_ErrLog("High speed device not connected.\n");
  }  
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
    LCD_LOG_SetHeader(" USB OTG DualCore Host");
    LCD_ErrLog("Bad BMP Formatd.\n");
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
