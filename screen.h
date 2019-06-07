#ifndef __GJ_UTILS_SCREEN_H__
#define __GJ_UTILS_SCREEN_H__

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define HEADER_MESSAGE_MAX_SIZE 13
#define HEADER_MESSAGE_CLOCK 10

namespace gj {

namespace utils{

struct Screen{

  Screen(Adafruit_SSD1306* display);

  void render();

  void set_signal_level(uint8_t level);

  void set_header_message(const std::string& message);

  void set_body_message(const std::string& message);

  void set_alert(bool on=true);

  void draw_wifi_signal(uint8_t level);

  void draw_header_message();

  void draw_body_message();

  void draw_alert_message();

private :

  Adafruit_SSD1306* display;

  uint8_t signal_level;

  int clock;

  std::string header_message;
  uint8_t header_message_pos;
  uint8_t header_message_clock;
  char header_buffer[HEADER_MESSAGE_MAX_SIZE + 1];

  std::string body_message;

  bool alert_on;
};

} // namespace utils

} // namespace gj

#endif
