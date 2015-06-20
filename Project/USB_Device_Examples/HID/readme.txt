/**
  @page USBD_HID  USB Device HID example
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Device HID (Joystick) example
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

This example provides a description of how to use the USB OTG Device peripheral 
on the STM32F2xx, STM32F4xx and STM32F105/7 devices.
The STM32 device is enumerated as an USB Device Joystick Mouse, that uses the
native PC Host HID driver.

The Joystick mounted on the STM322xG-EVAL, STM324xG-EVAL and STM3210C-EVAL boards
is used to emulate the Mouse directions.

This example supports remote wakeup (which is the ability of a USB device to bring
a suspended bus back to the active condition), and the key push-button is used as
the remote wakeup source.    
By default, in Windows powered PC the Power Management feature of USB mouse devices
is turned off. This setting is different from classic PS/2 computer functionality. 
Therefore, to enable the Wake from standby option, you must manually turn on the
Power Management feature for the USB mouse.
To manually enable the Wake from standby option for the USB mouse, 
  - start "Device Manager",
  - select "Mice and other pointing devices",
  - select the "HID-compliant mouse" device (make sure that PID &VID are equal to 0x5710 & 0x0483 respectively) 
  - right click and select "Properties", 
  - select "Power Management" tab,
  - finally click to select "Allow this device to wake the system from standby" check box.

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

  - STM322xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral

  - STM324xG-EVAL Set-up
    - Use CN8 connector to connect the board to a PC host when using USB OTG FS peripheral
    - Use CN9 connector to connect the board to a PC host when using USB OTG HS peripheral

  
@par How to use it ?

 + EWARM
    - Open the usbd_hid.eww workspace.
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
    - Open the usbd_hid.uvproj project
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
    - Open the usbd_hid.rprj project.
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
