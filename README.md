# AMIS
AMIS : an open-source Autonomous MultiWell Imaging Station

<img src="https://github.com/botabotlab/AMWIS/blob/main/img/main.jpg" width="100%">

# Build instructions

## Server

The following software must be installed for the server to work properly. The installation has been tested on different machines with ARM or X86 architecture based on Ubuntu or Debian distributions:


-   **[Mosquitto](https://github.com/eclipse/mosquitto)** : allows devices to communicate with each other using the MQTT protocol)
-   **[NodeJS 16](https://github.com/nodejs/Release)** (recommended version) : required runtime for the automation software
-   **[Node-RED](https://github.com/node-red/node-red)** : (automation server and graphical interface
-   **[Motion](https://github.com/Motion-Project/motion)** (not mandatory) : allows the creation of an IP camera to remotely retrieve the video stream from the microscope.
-   **[v4l2loopback](https://github.com/umlaeute/v4l2loopback)** : creation of virtual cameras that can be used for autofocus, image analysis, image display on screen or as an IP camera.
-   **[ffmpeg](https://github.com/FFmpeg/FFmpeg)** : duplicate the video stream of the USB camera in the virtual cameras
-   **[OpenCV-python](https://github.com/opencv/opencv-python)** : Image analysis library (python3 required)
    
Once Node-RED is installed and started, the user can access its graphical interface directly from the single board computer by entering http://127.0.0.1:1880 in the web browser or http://local.ip.of.the.SBC:1880 (if the interface is accessed from another computer). The "**[node-red-dashboard](https://github.com/node-red/node-red-dashboard)**" palette must be installed and the flow can be imported.

## Microcontroller

### Dependencies 

The microcontrollers code compilation and uploading can be done using the Arduino IDE. However, the Arduino IDE does not natively work with the ESP8266. The support for these boards must therefore be manually added by going to **File > Preference > Additional Boards Manager URLs** and pasting the following URL: "http://arduino.esp8266.com/stable/package_esp8266com_index.json" and installed by selecting ESP8266 in **Tools > Board > Boards Manager.**

3 libraries are required to run the code and can be found in the Arduino IDE library manager:

-   [PubSubClient](https://github.com/knolleary/pubsubclient)
-   [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
-   [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
    
### SPIFFS 

To facilitate the setting of WiFi and restore the previous state of the LEDs even in case of the microcontroller restart, JSON files (wifi.json and state.json) are stored in the SPIFFS memory. The use of SPIFFS memory allows to modify these parameters without having to recompile and upload the code and the use of JSON files makes the editing of these files faster and more intuitive than modifying the source code. Access to SPIFF memory is also not integrated by default in the IDE. The ESP8266 file uploader plugin can be downloaded at the following address:

https://github.com/esp8266/arduino-esp8266fs-plugin and unzipped in a **"tools"** folder created in the working directory of the arduino IDE.

The folder structure should look like this : **< home_dir>/Arduino-<version> /tools/ESP8266FS/tool/esp8266fs.jar)**. The arduino IDE can then be restarted and the tool should appear in **Tool > ESP8266 Sketch Data Upload.**

## Microscope

### Camera support

The camera support has been 3D printed in translucent ABS using a Prusa I3 MK3 (nozzle : 250°C, Bed : 100°C, nozzle 0.4mm, layer height : 0.2mm, filling 100%), however the parts can be printed in any type of plastic (PLA, PETG,...) using printers with lower tolerances and precision. The distance between the lens and the sensor is not critical, but the lens must be perpendicular to the sensor as well as the camera which must be perpendicular to the sample to avoid optical aberrations on the edge of the images. A manual adjustment of the parts using sandpaper may be necessary to compensate for small printing imperfections. The camera is held on the X axis of the printer with 3 neodymium magnets of 10x3mm.

<img src="https://github.com/botabotlab/AMWIS/blob/main/img/CamHolder.png" width="50%">

Several digital camera models with different prices, sizes and resolutions were tested to realize the microscope. The following sensors (with prices ranging from 10€ to 50€) were tested : Omnivision OV2640, OV3660, OV4689 and OV5647 or Sony IMX179 and all gave similar results. We have also modified webcams that are easily found on the market such as Logitech c270 or c920 which work perfectly. However, we recommend the use of camera modules with an M12 mount compared to webcams ; the lens placement is easier and the generally square PCB (versus rectangular/oval for webcams) makes it easier to install the camera on the magnetic mount. The resolution has little importance on the captured image quality, although a higher resolution allows a greater digital zoom. The limiting system factor is not the sensor, but the optics. A resolution of 720p is thus largely sufficient and allows simplifying the image analysis, but a 1080p or 4k resolution can be used in case the user would like to perform a digital zoom in addition to the optical zoom. 

### Light

The lighting system is built by soldering 24 addressable LED SK6812 (Data OUT of one LED should be soldered on the Data In of the next one) the LED matrix thus created will be driven by an ESP8266 by soldering the VCC and GND of the LEDs on pin 5V and GND of the microcontroller and by connecting the Din of the first led on pin D5 (GPIO 14) of the ESP. The system will then be controllable in MQTT by the Node-RED server. 

<img src="https://github.com/botabotlab/AMWIS/blob/main/img/LEDMatrix.jpg" width="50%">

The LED employed offer 4 light channels ; red, green, blue and cold white. We recommend using RGBW LED rather than RGB LED such as WS2812B. Although it is possible to obtain a relatively white light by combining the 3 RGB channels the positioning in 3 diodes makes that the light of each channel does not enter the medium with exactly the same angle of incidence and thus does form chromatic aberrations  These aberrations are not critical as the colored background will be removed by the thresholding adaptation step of the image analysis algorithm but it may affect the measurements if a quantification of the color of the algae is desired. The presence of a white channel mitigates this problem while offering a higher light intensity and preserving the accuracy of the colors (we recommend choosing SK6812 Neutral white and locking the white balance of the camera) . The color channels can be used to expose the sample to monochromatic or polychromatic light in order to improve the image contrast or to induce certain biological processes.

<img src="https://github.com/botabotlab/AMWIS/blob/main/img/rgbVSrgbw.jpg" width="50%">

*Chromatic aberration caused by the RGB light compared to a sample illuminated by a white light.*

## Pump

The system use a pump system described in a  [previous publication](https://www.sciencedirect.com/science/article/pii/S2468067221000286) . For the microscope, only one pump is needed and can be connected to a culture vessel (Erlenmeyer flask, photobioreactor) using a 7mm diameter autoclavable silicone tube. The tubing can be inserted into a micropipette tip for P1000 (blue tip). The pump can be primed until the liquid reaches the tip. The tip can then be placed on the magnetic tool holder and held in place with a 3D printed holder.

[Repository of the pump system](https://doi.org/10.17605/OSF.IO/ZP2S6) 

## 3D printer 

Our goal is to make a simple and universal system that can work with virtually any 3D printer model. A custom PCB would be complex, expensive and not very scalable. We therefore decided to exploit a common feature shared by many 3D printers. Indeed, these printers are generally controllable with standardized commands called GCODE. This set of instructions allows to control the machine movements on its 3 axes and to activate tools (plastic extruder, heating plate, fan, etc.).

Examples of GCode commands :
-   G91 : to set the machine in relative displacement
-   G0 X50 (move 50 mm along the x axis)

 [More info about GCode](https://reprap.org/wiki/G-code)

The commands can be sent via USB or placed in a text file on an SD card (or USB key) that will be read by the machine. 

A 3D printer only applies a simple sequence of commands and has no preconceived notions about their functions. It is therefore totally possible to remove the plastic extruder and replace it with a microscope and still be able to drive the machine using exactly the same commands. This is very interesting as it means that it is possible to transform a printer into a microscope, regardless of its brand, with almost no software or hardware modifications and to restore the original function of the machine if necessary.

### 3D printer hack

We chose an Anet A8 printer because its design is very simple, this model is easy to find on the internet and there are a lot of replacement parts and upgrades available. We kept most of the original parts (chassis, motors, print bed, motherboard). Instead of installing the extruder on the X axis we placed a 3D printed part with magnets to make a quick attachment for the camera or the sampling system. This part has been custom made for the Anet A8 and can not be used for another brand. However this modification is trivial and can be done on any 3D printer of the Cartesian or delta type. The only requirement is to have the camera perpendicular to the print bed (usually the Y axis).

<img src="https://github.com/botabotlab/AMWIS/blob/main/img/Render.png" width="50%">

To automate and control the device remotely, we connected an ESP8266 on the serial port of the motherboard. This WiFI microcontroller, allows to send Gcode commands using the MQTT communication protocol. The communication is bidirectional, and it is therefore possible to control the machine but also to obtain information on its status such as the current position of the camera (an interesting feature to control the machine remotely). The microcontroller is removable connected to the motherboard using an FC10P  connector and can easily be removed to restore the printer original functions.

<img src="https://github.com/botabotlab/AMWIS/blob/main/img/AnetHack.png" width="50%">

*Modification of the Anet A8 motherboard to be able to drive it in MQTT via its serial port. The motherboard has been set in "Serial" mode by bridging two pads (A) and then an ESP8266 (C ) has been connected to the VCC, GND, TX, RX of the motherboard (B). The serial connection between the microcontroller must be "crossed", meaning the TX of the ESP8266 connects to the RX of the motherboard and the RX connects to the TX.*

### Remote control using MQTT

MQTT (Message Queuing Telemetry Transport) is a publish/subscribe machine to machine communication protocol invented by IBM. Initially designed to retrieve information from remote locations from resource constrained devices (like as microcontroller) over an unreliable, high latency and low bandwidth connection (satellite connection for instance) (REF 29) this protocol is nowadays mainly used for IOT (Internet Of Things) applications. This widely used protocol offers numerous advantages for our application. It its simple to use, secure (message can be encrypted using TLS) , reliable, lightweight (in term of computing resources and bandwidth usage), compatible with virtually any programming languages (python, C, C#, C++, java, javascript, lua, ruby, rust, …) and highly scalable.

A typical MQTT communication requires at least 4 components:
-   **Publisher** :  the device that will send information or commands (a computer, a smartphone, a microcontroller)
-   **Subscriber** : the device that will receive the command (in our case a microcontroller connected to the robot)
-   **Broker** : the server that links the different devices
-   **Internet router**
    
*A publisher can also subscribe to one or more topics and that a bidirectional communication between the devices is possible*.

In most universities and corporate networks, some ports may be blocked (even locally) which will prevent MQTT (running on port 1883) from working. We recommend using a router over which the user has full control to create a local subnet to which devices can connect without restriction. The use of an ESP32/8266, a smartphone or a computer in access point mode can also be a solution to create a local network. An internet connection is not necessary as long as all devices are on the same network and port 1883 is not blocked. Our system is able to work locally without depending on a cloud, but an internet connection can be useful for remote control using a VPN (Virtual Private Network) or to host the broker in a VPS (Virtual Private Server).

<img src="https://github.com/botabotlab/AMWIS/blob/main/img/MQTT.png" width="50%">

*Representation of the MQTT communication, allowing to control the microscope remotely. The graphical control interface accessible from a computer or a smartphone (A) connected to the laboratory router (B) allows formatting and send GCODE commands in WiFi to the MQTT broker installed on the Jeston Nano (C ) . The message is then connected to the ESP8266 (D) subscribed to the right topic and which will finally transmit the GCODE command in serial to the printer motherboard (E) allowing to move the camera (F).*

To send a command to the microscope, the user must publish the payload containing the command (e.g. G0 X50) with the topic corresponding to the microscope (microscope/room01/cmd/01) while being subscribed to the debug topic (microscope/room01/debug/01) to receive confirmation that the command is correct and has been applied. For our device, the microscope, the LED lighting system and the manipulation system are all 3 controllable by MQTT.

If the user does not want to use a MQTT connection, our microscope can be completely controlled using a serial communication. Simply connect the signle board computer to the 3D printer using an usb cable and replace the MQTT node with a Serial out node (by installing the "node-red-node-serialport" node) and select the com port (e.g. /dev/ttyUSB0) and baud rate (115200). Since the commands sent have the same structure as GCODE commands, no software modification is necessary. The same applies to the LED array, which can be connected to the Jetson nano via USB and driven via serial.


# Control

The control of the machine can be done in three different ways:

-   Manual remote control by sending messages in MQTT using the software/programming language chosen by the user
-   Manual control from the Node-RED graphical interface
-   Automatic control using a JSON config file

## MQTT Custom control

The two following examples of topics "**microscope/01/move**" and "**microscope/01/light**" manage respectively the microscope and the lighting system thanks to the MQTT Custom control.

The commands sent to the topic "microscope/01/move" must have exactly the same structure as GCODE commands. 

|CMD|Description  |
|--|--|
| M17 | Enable stepper motors |
|M18 | Disable stepper motors|
|M112 | Emergency stop|
|M114 | Get current position|
|G28 | Home|
|G90 | Absolute positioning|
|G91 | Relative positioning|
|G1Axis[X/Y/Z] Position(mm) Feed(optional) | Movement (Ex : G1 X10 Y20 Z30)|
    
*Frequently used GCODE commands*

## Graphical user interface

Once the Node-RED server is launched and the provided flow is deployed, the user can go to the address **http://server.ip:1880/ui** using a smartphone or computer and to access the Node-RED GUI.

*Graphical control interface via Node-RED composed of 5 panels (debug cam, light, control, plate and cam). The first panel (A) allows access to the security camera (Cam) to remotely view the machine to ensure it is working properly or to access the microscope image (Microscope) to observe the samples. The second panel allows to control the light either by choosing a color with the color picker (B) or by manually entering the RGB values (C). The control panel (D) allows you to manually move the camera on its 3 axes and returns the current position of the camera. The flat menu (E) allows you to move the camera to pre-programmed positions or to choose which well to light. The "SP" (safe position) button sends the camera to the upper right corner of the robot and advances the plate towards the user, thus facilitating the installation of multi-well plates, while the "LP" (last position) button returns the camera to its previous position (in case the user accidentally moves the camera). The panel (F) allows you to name the images. If several images are captured, the name will automatically be changed by adding the number of the photo (Iter). Finally, panel (G) allows you to perform autofocus and also to program the taking of multiple images separated by a defined delay.*

The camera can be started on the "**Developper camera**" menu. 

*The "Create virtual Cam" button allows to use “v4l2loopback” to create virtual cameras that will be used by different capture, image analysis, autofocus or debugging programs in HDMI or via IP stream. The use of virtual cameras is necessary because several programs cannot access the same camera at the same time. The second button allows to launch motion which will create an IP stream allowing to access the images remotely in the graphical interface. The button "copy usb webcam" will launch “ffmpeg” which will clone the physical camera in the different virtual cameras. The buttons CAM LCD ON and OFF allow the microscope image display or hiding through HDMI. Analyze ON and OFF display the image analysis (counting and size measurement) in real time on the HDMI output . Finally, the WB lock button and the WB slider allow to lock the white balance to a fixed value defined by the user. When the system is functional the 3 menus should be green.*

This graphical interface allows a quick and manual observation, capture and analysis of one or more samples. A typical use would be to place different samples in multi-well plates, turn on the microscope, initialize the cameras, send the camera to "**Safepos**", place the plates on the bed, choose the illumination color and observe the desired samples. This system is faster than a regular microscope using a slide/coverslip system. 

## Automation

Many situations may require the capture of a large number of images (capture of several plates in a row) or need to be done over a long period of time (real time monitoring of a culture). As manually preparing and analyzing the samples is not feasible in this case, we have developed an automation system. This system allows to automate all the actions available on the graphical interface based on a JSON structure that is both intuitive and flexible. The user can thus define the sequence name, the wells to fill (if filling is desired) and the need to purge the circuit or not (to avoid contamination if the time between two samples is high), the wells to capture (if capturing is desired) and the light (color and intensity) corresponding to each well and the time between two samplings or captures.

JSON structure :

	{  
	"name" : "Name of the experiment",  
	"pos" : [ [X1 (0-5) ,Y1(0-3)] ],  
	"light": [ [R(0-255) , G(0-255) , B(0-255), W(0-255)] ],  
	"delay":[(s)],  
	"capture":true-false,  
	"sampling":true-false,  
	"volume":[(ml)]  
	"trash":true-false  
	}

The order of the wells can be defined by the user and not all wells have to be used ; it is possible to fill a complete plate or just a column. The light can be different between each well. To do this it is imperative that the "light" array has exactly the same length as the "pos" array. If the same color intensity should be used for all wells it is possible to enter only one light value in "light" and the system will automatically apply the light to all wells.

It is not mandatory to capture images ; the machine can for example be used to fill multi-well plates with samples. To do so, you just have to set the "capture" key to false. The following JSON allows to fill 1 multiwell plate with a volume of 2ml (this procedure takes about 2 minutes for 12 wells). Capturing images alone without taking a sample is also possible ; the "capture" property must be set to true.

Demo script to fill the plate 1 with samples of 1ml : 

	{
	"name" : "Fill plate 1",
	"pos":[
	[0,0],[1,0],[2,0],
	[0,1],[1,1],[2,1],
	[0,2],[1,2],[2,2],
	[0,3],[1,3],[2,3]
	],
	"sampling":true,
	"volume" : [1]
	} 

Demo script to capture the plate 1 with a white light of 100% :

	{
	"name" : "Capture plate 1",
	"pos":[
	[0,0],[1,0],[2,0],
	[0,1],[1,1],[2,1],
	[0,2],[1,2],[2,2],
	[0,3],[1,3],[2,3]
	],
	"capture":true,
	"light":[[0,0,0,255]]
	}

### Example

The following example describes the most complex measurement sequence currently achievable with the machine consisting in reading three samples with different parameters. The robot will automatically fill the wells with three different volumes (1ml 2ml and 3ml) while purging the silicone tube between two samples. Each sample will be observed with a different color (red, green and blue) and intensity (100%, 50% and 25%) and the time between two measurements is set to 30 seconds for the first sample sample and 1 minute for the second one : 

	{
	"name" : "DemoSeq", 
	"pos" : [[0,0],[2,3],[3,0]], 
	"light" : [[255,0,0,0],[0,255,0,0],[0,0,255,0]], 
	"delay" : [30,60,0] , 
	"sampling" : true, 
	"volume" : [1,2,3], 
	"trash" : true, 
	"capture" : true
	}

## Safety

- The system uses a 220V power supply so the connections must be properly isolated. 
- The motherboard as well as the single board computer and the lighting system must be kept away from humidity or any conductive elements that could cause a short circuit. 
- The 3 axes of movement must not be obstructed as most 3D printers do not have built-in stall detection (meaning that the motors can be forced even if they are blocked) and the limit switches must be accessible during calibration. 
- The user must always check that the magnets are well aligned when a tool is placed on the machine, taking care not to get his fingers caught when installing the tools as the magnets are relatively powerful.







