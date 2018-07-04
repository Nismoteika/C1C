# C1C
### Projector remote control (SANYO/EIKI api).
### ***At now project is frozen and using in next project***
This is a easy bridge from wifi to serial COM port, specify to help you control big projector, which not support Wi-Fi, but support old-school COM port.

If you projector location in hard-to-reach place and IR sensor of projector can't get signal.
You need to C1C firmware based on Arduino core and device built on esp8266 with adapter to COM port.
C1C solves problems of IR signal availability, as well as provides a convenient interface and mobility
____

## Content
1. [Installation](#Installation)
2. [How to build up](#How-to-build-up)
3. [Usage](#Usage)
4. [Our build](#Our-build)
5. [Web interface](#Web-interface)

## Installation

### PlatformIO
1. Install PlatformIO convenient way
2. Open project this repository
3. In `src/main.cpp` change `app_pass` from `******` to access point password
3. Upload firmware and fs

### Arduino IDE
1. Install Arduino IDE
2. Create new project
3. Copy contains `src/main.cpp` into your project
4. Copy `data` folder in your project directory
5. Install esp8266, ESPAsyncTCP, ESPAsyncWebServer addons
4. In your project change `app_pass` from `******` to access point password
5. Upload firmware and fs

## How to build up
1. You need esp8266 module and adapter from serial to COM port
2. Connect esp8266 and adapter as shown in picture down
![connect like this](https://github.com/Nismoteika/C1C/raw/master/assets/scheme.png "connect like this")
3. Only the body is left to be made, any suitable in size

## Usage
1. Connect to Wi-Fi named C1C with your password
2. Open webpage `192.168.4.1`
3. You will see the [web interface](#Web-interface).

## Our build
![build](https://github.com/Nismoteika/C1C/raw/master/assets/ourbuild.png "our build")

## Web interface
![interface](https://github.com/Nismoteika/C1C/raw/master/assets/interface.png "web interface")