/**
  @page USB_HOST_DEVICE  USB Manual Dual role device example
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Manual Dual role device example
  ******************************************************************************
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
  * limitations under the Licens
  *   
  ******************************************************************************
   @endverbatim

   
@par Example Description 

This example provides a description of how to use the USB OTG Device/Host peripheral 
on the STM32F2xx, STM32F4xx and STM32F105/7 devices.
In device mode The STM32 is enumerated as an USB Mass storage Device, that uses the
the embedded microSD as storage media.
In Host mode, the STM32 behave as a mass storage Host that can enumerate, show content
and display the supported BMP image in the attached USB flash disk. 
The user has to select the USB mode (Host or Device) using the embedded joystick
following the menu displayed in the Bottom of the LCD display.

 @note Only the BMP files with the following format are supported : 
         - width  : 320
         - Height : 240
         - BPP    :  16
         - Compression : RGB bitmap with RGB masks  
       To use the Image browser, copy the BMP files from "\Utilities\Binary\Media"
       to the root of the USB flash disk to be used.   

 @note The USB Device library uses the multi-packet feature, this functionality increase 
       the transfer performance when using big amount of data. In the Mass storage demo, 
       an intermediary buffer is used to receive/send data from/to USB  and used also to 
       write/read from the physical media (microSD). The bigger is the buffer, the faster
       is the IO operations in USB and also in media; in order to use a balanced buffer size
       with the project footprint; the buffer size was defined in the usbd_conf.h file to 4K,
       but it can be modified by the user from 512 Bytes to 64KBytes.  

This example works
  - in high speed (HS) when the STM322xG-EVAL or the STM324xG-EVAL board and the
    USB OTG HS peripheral are used
  - in full speed (FS) when the STM322xG-EVAL or the STM324xG-EVAL board and the
    USB OTG FS peripheral are used, or when using the STM3210C-EVAL board.
    

@par Hardware and Software environment 

   - This example runs on STM32F105/7 Connectivity line, STM32F2xx and STM32F4xx devices.
  
   - This example has been tested with STM3210C-EVAL RevB (STM32F105/7 devices), 
     STM322xG-EVAL RevB (STM32F2xx) and STM324xG-EVAL RevB (STM32F4xx)    

  - STM3210C-EVAL Set-up 
    - use CN2 connector to connect the board to a PC host or USB device

  - STM322xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host or USB device when using USB OTG FS peripheral.
    - Use CN9 connector to connect the board to a PC host or USB device when using USB OTG HS peripheral.

  - STM324xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
      
@par How to use it ?

 + EWARM
    - Open the USB_Host_device.eww workspace.
    - In the workspace toolbar select the project config:
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBH-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBH-FS: to configure the project for STM32F105/7 devices  
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the USB_Host_device.uvproj project
    - In the build toolbar select the project config:
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBH-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBH-FS: to configure the project for STM32F105/7 devices  
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + RIDE
    - Open the USB_Host_device.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBH-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBH-FS: to configure the project for STM32F105/7 devices  
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project: 
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBH-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBH-FS: to configure the project for STM32F105/7 devices  
    
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 + TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project:  
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBH-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBH-FS: to configure the project for STM32F105/7 devices  
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)
    
@note Known Limitations
      This example retargets the C library printf() function to the EVAL board’s LCD
      screen (C library I/O redirected to LCD) to display some Library and user debug
      messages. TrueSTUDIO Lite version does not support I/O redirection, and instead
      have do-nothing stubs compiled into the C runtime library. 
      As consequence, when using this toolchain no debug messages will be displayed
      on the LCD (only some control messages in green will be displayed in bottom of
      the LCD). To use printf() with TrueSTUDIO Professional version, just include the
      TrueSTUDIO Minimal System calls file "syscalls.c" provided within the toolchain.
      It contains additional code to support printf() redirection.
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
