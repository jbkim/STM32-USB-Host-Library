/**
  @page USBD_MSC  USB Device Mass Storage example
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Device Mass Storage example
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

The Mass Storage example gives a typical example of how to use the STM32F2xx, STM32F4xx
and STM32F105/7 USB OTG Device peripheral to communicate with a PC Host using the bulk
transfer while the microSD card is used as storage media.
This example supports the BOT (bulk only transfer) protocol and all needed SCSI
(small computer system interface) commands, and is compatible with both Windows
XP (SP1/SP2) and Windows 2000 (SP4).
After running the example, the user just has to plug the USB cable into a PC host
and the device is automatically detected. A new removable drive appears in the
system window and write/read/format operations can be performed as with any other
removable drive.

  @note The USB Device library uses the multi-packet feature, this functionality increase 
        the transfer performance when using big amount of data. In   the Mass storage demo, 
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
    - use CN2 connector to connect the board to a PC host
    - Make sure that a microSD card is plugged into CN16 connector

  - STM322xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
    - Make sure that a microSD card is plugged into CN6 connector
          - Make sure that Jumpers JP22 and JP16 are in position 1<->2

  - STM324xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral
    - Make sure that a microSD card is plugged into CN6 connector
          - Make sure that Jumpers JP22 and JP16 are in position 1<->2


@par How to use it ?

 + EWARM
    - Open the usbd_msc.eww workspace.
    - In the workspace toolbar select the project config:
     - STM322xG-EVAL_USBD-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBD-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBD-FS: to configure the project for STM32F105/7 devices  
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the usbd_msc.uvproj project
    - In the build toolbar select the project config:
     - STM322xG-EVAL_USBD-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBD-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBD-FS: to configure the project for STM32F105/7 devices  
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + RIDE
    - Open the usbd_msc.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
     - STM322xG-EVAL_USBD-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBD-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBD-FS: to configure the project for STM32F105/7 devices  
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project: 
     - STM322xG-EVAL_USBD-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBD-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBD-FS: to configure the project for STM32F105/7 devices  
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
     - STM322xG-EVAL_USBD-HS: to configure the project for STM32F2xx devices and use USB OTG HS peripheral
     - STM322xG-EVAL_USBD-FS: to configure the project for STM32F2xx devices and use USB OTG FS peripheral
     - STM324xG-EVAL_USBD-HS: to configure the project for STM32F4xx devices and use USB OTG HS peripheral
     - STM324xG-EVAL_USBD-FS: to configure the project for STM32F4xx devices and use USB OTG FS peripheral
     - STM3210C-EVAL_USBD-FS: to configure the project for STM32F105/7 devices  
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
