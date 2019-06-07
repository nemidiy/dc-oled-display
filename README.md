# OLED Screen lib for GJ products

This library is meant to be used with platformIO.

On the project using this library edit the platformio.ini file and add the dependency. IE for the d1_mini under lib_deps :

```
[env:d1_mini]
platform = espressif8266
board = d1_mini
framework = arduino
upload_speed = 115200
lib_deps =
   Adafruit GFX Library@1.5.3
   Adafruit SSD1306@1.2.9
   ${sysenv.GREEN_JACUZZI_ROOT}/oled_display

```

Then to build :

```
$ GREEN_JACUZZI_ROOT=/PATH/TO/GJ_LIBS platformio run
```

A sample is inluded :

```c++
#include <Adafruit_SSD1306.h>

// include the library
#include <screen.h>

// Create display
Adafruit_SSD1306 display(128, 64);

// create the screen
gj::utils::Screen screen(&display);

void setup(){
  screen.set_header_message("SUPER DEVICE FROM HELL");
  screen.set_alert();
}

void loop(){
...
}
```
