#include "screen.h"

using namespace gj::utils;

Screen::Screen(Adafruit_SSD1306* display){
  this->display = display;
  this->signal_level = 0;
  this->header_message_pos = 0;
  this->header_message_clock = 0;
  this->alert_on = false;

  //let the dislpay initialize
  delay(100);
  display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display->clearDisplay();
  display->setTextColor(WHITE);
  display->setRotation(0);
  display->setTextWrap(false);
  display->dim(0);

}

void Screen::render(){
  display->clearDisplay();  // Clear the display so we can refresh
  //display->drawRect(0, 0, 128, 16, WHITE);
  draw_header_message();
  draw_alert_message();
  draw_body_message();
  draw_wifi_signal(signal_level);
  //display->drawRect(0, 17, 128, 48, WHITE);
  display->display();
}

void Screen::set_signal_level(uint8_t level){
  signal_level = level;
}

void Screen::set_header_message(const std::string& message){
  header_message = message;
  if(header_message.size() > 15)
    header_message += " - ";
  header_message_pos = 0;
  header_message_clock = 0;
  memset(header_buffer, 0, 16);
}

void Screen::set_body_message(const std::string& message){
  body_message = message;
}

void Screen::set_alert(bool on){
  alert_on = on;
}

void Screen::draw_wifi_signal(uint8_t level){
  uint8_t x = 114;
  uint8_t y = 4;
  display->fillRect(x , y, 14, 8, BLACK);
  if (level == 0) {
    display->setCursor(x, y);  // (x,y)
    display->println("...");
  } else if (level == 1) {
    display->fillRect(x,      y + 8 , 2, 2 , WHITE);
  } else if (level == 2) {
    display->fillRect(x,      y + 8 , 2, 2 , WHITE);
    display->fillRect(x + 3,  y + 6 , 2, 4 , WHITE);
  } else if (level == 3) {
    display->fillRect(x,      y + 8 , 2, 2 , WHITE);
    display->fillRect(x + 3,  y + 6 , 2, 4 , WHITE);
    display->fillRect(x + 6,  y + 4 , 2, 6 , WHITE);
  } else if (level == 4) {
    display->fillRect(x,      y + 8 , 2, 2 , WHITE);
    display->fillRect(x + 3,  y + 6 , 2, 4 , WHITE);
    display->fillRect(x + 6,  y + 4 , 2, 6 , WHITE);
    display->fillRect(x + 9,  y + 2 , 2 ,8 , WHITE);
  } else if (level == 5) {
    display->fillRect(x,      y + 8 , 2, 2 , WHITE);
    display->fillRect(x + 3,  y + 6 , 2, 4 , WHITE);
    display->fillRect(x + 6,  y + 4 , 2, 6 , WHITE);
    display->fillRect(x + 9,  y + 2 , 2 ,8 , WHITE);
    display->fillRect(x + 12, y + 0 , 2 ,10, WHITE);
  }
}

void Screen::draw_header_message(){
  memset(header_buffer, 0, HEADER_MESSAGE_MAX_SIZE + 1);
  memcpy(
      header_buffer,
      header_message.c_str() + header_message_pos,
      header_message.size() - header_message_pos > HEADER_MESSAGE_MAX_SIZE ?
          HEADER_MESSAGE_MAX_SIZE : header_message.size() - header_message_pos);

  if (header_message.size() - header_message_pos < HEADER_MESSAGE_MAX_SIZE &&
          header_message.size() > HEADER_MESSAGE_MAX_SIZE){
        memcpy(
          header_buffer + header_message.size() - header_message_pos,
          header_message.c_str(),
          HEADER_MESSAGE_MAX_SIZE - header_message.size() + header_message_pos);
  }

  display->setCursor(0, 6);
  display->println(header_buffer);

  if(header_message.size() > HEADER_MESSAGE_MAX_SIZE &&
        header_message_clock == HEADER_MESSAGE_CLOCK){
    ++header_message_pos;
    if(header_message_pos > header_message.size()){
      header_message_pos = 0;
    }
  }

  ++header_message_clock;
  if(header_message_clock > HEADER_MESSAGE_CLOCK){
    header_message_clock = 0;
  }
}


void Screen::draw_body_message(){
  display->setCursor(0, 22);
  display->println(body_message.c_str());

  /*
  static const unsigned char PROGMEM github [] = {};
  display->drawBitmap(0, 16, github, 128, 64, WHITE);
  */

}

void Screen::draw_alert_message(){
  uint8_t x = 98;
  uint8_t y = 5;
  if(alert_on){
    display->fillRect(x,     y     , 2, 4, WHITE);
    display->fillRect(x,     y + 6 , 2, 2, WHITE);
    display->drawCircle(x,   y + 3 , 7, WHITE);
  }else{
    display->fillRect(x - 7 , y - 5, 14, 14, BLACK);
  }
}
