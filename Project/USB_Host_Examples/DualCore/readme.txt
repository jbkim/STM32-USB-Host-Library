/**
  @page USBH_DualCore  USB Host Dual Core example
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   Description of the USB Host Dual Core example
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

The USB Device Dual Core example uses the STM32F2xx and STM32F4xx multi core support
feature and integrates the Mass Storage and HID examples in the same project. 
The Mass storage device should be connected to the USB HS port while the HID device 
should be connected to the FS port. 

The user has to select the USB operation (MSC or HID) using the embedded joystick
following the menu displayed in the Bottom of the LCD display.

 @note Only the BMP files with the following format are supported : 
         - width  : 320
         - Height : 240
         - BPP    :  16
         - Compression : RGB bitmap with RGB masks  
       To use the Image browser, copy the BMP files from "\Utilities\Binary\Media"
       to the root of the USB flash disk to be used.     


@par Hardware and Software environment 

   - This example runs on STM32F2xx and STM32F4xx devices.
  
   - This example has been tested with STM322xG-EVAL RevB (STM32F2xx) and 
     STM324xG-EVAL RevB (STM32F4xx)    
        
  - STM322xG-EVAL Set-up
    - Use both CN8 and CN9 connectors to connect the USB devices

  - STM324xG-EVAL Set-up
    - Use both CN8 and CN9 connectors to connect the USB devices


@par How to use it ?

 + EWARM
    - Open the USBH_DualCore.eww workspace.
    - In the workspace toolbar select the project config:
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices
     - STM324xG-EVAL_USBH-HS: to configure the project for STM32F4xx devices
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 + MDK-ARM
    - Open the USBH_DualCore.uvproj project
    - In the build toolbar select the project config:
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices
     - STM324xG-EVAL_USBH-HS: to configure the project for STM32F4xx devices
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

 + RIDE
    - Open the USBH_DualCore.rprj project.
    - In the configuration toolbar(Project->properties) select the project config:
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices
     - STM324xG-EVAL_USBH-HS: to configure the project for STM32F4xx devices
    - Rebuild all files: Project->build project
    - Load project image: Debug->start(ctrl+D)
    - Run program: Debug->Run(ctrl+F9)

 + TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to  TASKING workspace directory and select the project: 
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices
     - STM324xG-EVAL_USBH-HS: to configure the project for STM32F4xx devices
   
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
     - STM322xG-EVAL_USBH-HS: to configure the project for STM32F2xx devices
     - STM324xG-EVAL_USBH-HS: to configure the project for STM32F4xx devices 
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
