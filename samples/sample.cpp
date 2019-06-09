#include <Adafruit_SSD1306.h>

#include <sstream>
#include <string>

#include <screen.h>

// Create display
Adafruit_SSD1306 display(128, 64);
// create the screen
gj::utils::Screen screen(&display);

void setup(){
  screen.set_scrolling_message("SUPER DEVICE FROM HELL");
  screen.set_alert();
}

int loop_count = 0;
uint8_t signal_level = 0;

void loop(){

  if (loop_count == 15){
    screen.set_body_message("Hola");

    int8_t signal = 70;
    signal = (120 - signal) / 120 * 100;
    if(signal < 10){
      signal_level = 0;
    }else if (signal >= 10 && signal < 20){
      signal_level = 1;
    }else if (signal >= 20 && signal < 40){
        signal_level = 2;
    }else if (signal >= 40 && signal < 60){
        signal_level = 3;
    }else if (signal >= 60 && signal < 80){
        signal_level = 4;
    }else if (signal >= 80){
        signal_level = 5;
    }

  }

  screen.set_signal_level(signal_level);
  screen.render();
  ++loop_count;
}  // End of loop
