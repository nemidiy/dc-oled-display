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
  
  /*
  screen.add_canvas(
      [](struct gj::utils::Screen::Screen* screen, void* param){
        Adafruit_SSD1306* d = screen->get_display();
        d->setCursor(0,18);
        d->setTextSize(2);
        d->println("PH");
        d->setCursor(7,36);
        d->setTextSize(4);
        d->println("10.56");
      },
      NULL);

  screen.add_canvas(
      [](struct gj::utils::Screen::Screen* screen, void* param){
        Adafruit_SSD1306* d = screen->get_display();
        d->setTextSize(2);
        d->setCursor(0,18);
        d->println("EC");
        d->setCursor(103,18);
        d->println("uV");
        d->setCursor(28,36);
        d->setTextSize(4);
        d->println("538");
      },
      NULL);

  screen.add_canvas(
      [](struct gj::utils::Screen::Screen* screen, void* param){
        Adafruit_SSD1306* d = screen->get_display();
        d->setTextSize(2);
        d->setCursor(0,18);
        d->println("TEMP");
        d->setCursor(120,18);
        d->println("C");
        d->setCursor(7,36);
        d->setTextSize(4);
        d->println("27.89");
      },
      NULL);
*/
}

void loop(){
...
}
```
