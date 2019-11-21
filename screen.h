#ifndef __GJ_UTILS_SCREEN_H__
#define __GJ_UTILS_SCREEN_H__

#include <vector>
#include <functional>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>


#define SCROLL_MESSAGE_MAX_SIZE 12
#define SCROLL_MESSAGE_CLOCK 10

namespace gj {

namespace utils{

struct Screen{

  typedef enum {
    UP    = 0,
    DOWN  = 1,
    LEFT  = 2,
    RIGHT = 3,
    CLICK = 4,
    NONE  = 10
  } joystick_event;

  typedef std::function<void (struct Screen *screen, void* param)> canvas_func;

  Screen(Adafruit_SSD1306* display,
        int joystick_sw_pin = -1);

  void render();

  void set_signal_level(uint8_t level);

  void draw_wifi_signal(uint8_t level);

  void set_alert(bool on=true);

  void draw_alert(uint8_t x = 7, uint8_t y = 4);

  void set_bluetooth(bool on=true);

  void draw_bluetooth(uint8_t x = 95, uint8_t y = 0);

  void set_scrolling_message(const std::string& message);

  void draw_scrolling_message(uint8_t x, uint8_t y, uint8_t text_size);

  void set_body_message(const std::string& message);

  void draw_body_message(uint8_t size = 1);

  void add_canvas(canvas_func func, void* param);

  uint8_t signal_to_level(uint8_t signal);

  joystick_event read_joystick();

  Adafruit_SSD1306* get_display(){
    return display;
  }

private :

  Adafruit_SSD1306* display;

  uint8_t signal_level;

  std::string header_message;
  char header_buffer[SCROLL_MESSAGE_MAX_SIZE + 1];

  std::string body_message;

  bool alert_on;
  bool bluetooth;

  //canvas
  uint8_t selected_canvas;
  std::vector<canvas_func> canvas;
  std::vector<void *> canvas_param;

  //joystick variables
  bool joystick;
  int joystick_sw_pin;
  Bounce joystick_bounce;
};

} // namespace utils

} // namespace gj

#endif
