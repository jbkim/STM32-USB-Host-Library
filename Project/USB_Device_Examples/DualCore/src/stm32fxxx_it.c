/**
  ******************************************************************************
  * @file    stm32fxxx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and peripherals interrupt
  *          service routine.
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
#include "stm32fxxx_it.h"

#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_hid_core.h"
#include "usb_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CURSOR_STEP     10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
extern USB_OTG_CORE_HANDLE           USB_OTG_FS_dev;
extern USB_OTG_CORE_HANDLE           USB_OTG_HS_dev;
static uint8_t *USBD_HID_GetPos (void);
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
extern uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    
  uint8_t *buf;
  
  buf = USBD_HID_GetPos();
  if((buf[1] != 0) ||(buf[2] != 0))
  {
    USBD_HID_SendReport (&USB_OTG_FS_dev, 
                         buf,
                         4);
  }
}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_HS_dev);
}

void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_FS_dev);
}

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
/**
  * @brief  This function handles EP1_IN Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
  USBD_OTG_EP1IN_ISR_Handler (&USB_OTG_HS_dev);
}

/**
  * @brief  This function handles EP1_OUT Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
  USBD_OTG_EP1OUT_ISR_Handler (&USB_OTG_HS_dev);
}
#endif

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles EXTI15_10_IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
  {
 
    USB_OTG_ActiveRemoteWakeup(&USB_OTG_FS_dev);
    /* Clear the EXTI line pending bit */
    EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
  }
}

/**
  * @brief  This function handles EXTI15_10_IRQ Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_FS  
void OTG_FS_WKUP_IRQHandler(void)
{
#ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT  
  *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ; 
  SystemInit();
  USB_OTG_UngateClock(&USB_OTG_FS_dev);
#endif  
  EXTI_ClearITPendingBit(EXTI_Line18);
}
#endif

/**
* @brief  USBD_HID_GetPos
* @param  None
* @retval Pointer to report
*/
static uint8_t *USBD_HID_GetPos (void)
{
  int8_t  x = 0 , y = 0 ;
  static uint8_t HID_Buffer [4];
  
  switch (IOE_JoyStickGetState())
  {
  case JOY_LEFT:
    x -= CURSOR_STEP;    
    break;
  case JOY_RIGHT:
    x += CURSOR_STEP;    
    break;
  case JOY_UP:
    y -= CURSOR_STEP;    
    break;
  case JOY_DOWN:
    y += CURSOR_STEP;    
 
    break;
  }
  
  HID_Buffer[0] = 0;
  HID_Buffer[1] = x;
  HID_Buffer[2] = y;
  HID_Buffer[3] = 0;
  
  return HID_Buffer;
}
/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
