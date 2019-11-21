#include "screen.h"

using namespace gj::utils;

Screen::Screen(Adafruit_SSD1306* display,
              int joystick_sw_pin){

  this->display = display;
  this->signal_level = 0;
  this->alert_on = false;
  this->bluetooth = false;


  this->joystick_sw_pin = joystick_sw_pin;
  if (joystick_sw_pin == -1){
    joystick = false;
  }else{
    joystick = true;
    joystick_bounce.attach(joystick_sw_pin, INPUT_PULLUP);
    joystick_bounce.interval(10);
  }

  //let the dislpay initialize
  delay(100);
  display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display->clearDisplay();
  display->setTextColor(WHITE);
  display->setRotation(0);
  display->setTextWrap(false);
  display->dim(0);

  selected_canvas = 0;
}

void Screen::render(){
  //read joystick and move canvas
  joystick_event ev = read_joystick();
  if(ev == CLICK && selected_canvas == canvas.size() - 1){
    selected_canvas = 0;
  }else if(ev == CLICK){
    ++selected_canvas;
  }

  // Clear the display so we can refresh
  display->clearDisplay();
  if(canvas.size()){
    canvas[selected_canvas](this, canvas_param[selected_canvas]);
  }

  if(bluetooth){
    draw_bluetooth();
  }
  draw_alert();
  draw_wifi_signal(signal_level);
  display->display();
}

void Screen::set_signal_level(uint8_t level){
  signal_level = level;
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

void Screen::set_scrolling_message(const std::string& message){
  if(message == header_message ||
        message + " - " == header_message){
    return;
  }
  header_message = message;
  if(header_message.size() > SCROLL_MESSAGE_MAX_SIZE)
    header_message += " - ";
  memset(header_buffer, 0, SCROLL_MESSAGE_MAX_SIZE + 1);
}

void Screen::draw_scrolling_message(uint8_t x, uint8_t y, uint8_t text_size){
  static uint8_t cycles = 0;
  static uint8_t message_pos = 0;
  memset(header_buffer, 0, SCROLL_MESSAGE_MAX_SIZE + 1);
  memcpy(
      header_buffer,
      header_message.c_str() + message_pos,
      header_message.size() - message_pos > SCROLL_MESSAGE_MAX_SIZE ?
          SCROLL_MESSAGE_MAX_SIZE : header_message.size() - message_pos);

  if (header_message.size() - message_pos < SCROLL_MESSAGE_MAX_SIZE &&
          header_message.size() > SCROLL_MESSAGE_MAX_SIZE){
        memcpy(
          header_buffer + header_message.size() - message_pos,
          header_message.c_str(),
          SCROLL_MESSAGE_MAX_SIZE - header_message.size() + message_pos);
  }
  display->setTextSize(text_size);
  display->setCursor(x, y);
  display->println(header_buffer);

  if(header_message.size() > SCROLL_MESSAGE_MAX_SIZE &&
        cycles == SCROLL_MESSAGE_CLOCK){
    ++message_pos;
    if(message_pos > header_message.size()){
      message_pos = 0;
    }
  }

  ++cycles;
  if(cycles > SCROLL_MESSAGE_CLOCK){
    cycles = 0;
  }
}


void Screen::draw_body_message(uint8_t size){
  display->setTextSize(size);
  display->setCursor(0, 22);
  display->println(body_message.c_str());
}

void Screen::draw_alert(uint8_t x, uint8_t y){
  static int cycles = 0;
  static bool blink = true;

  if(alert_on && blink){
    display->fillRect(x,     y     , 2, 4, WHITE);
    display->fillRect(x,     y + 6 , 2, 2, WHITE);
    display->drawCircle(x,   y + 3 , 7, WHITE);
  }else{
    display->fillRect(x - 7 , y - 5, 14, 14, BLACK);
  }

  if(cycles == 15 && blink){
    blink = false;
    cycles = 0;
  }else if(cycles == 10 && !blink){
    blink = true;
    cycles = 0;
  }
  ++cycles;
}


void Screen::set_bluetooth(bool on){
  bluetooth = on;
}

void Screen::draw_bluetooth(uint8_t x, uint8_t y){
  display->drawLine(x    , y      , x    , y + 14, WHITE);
  display->drawLine(x - 5, y + 2  , x + 5, y + 10, WHITE);
  display->drawLine(x    , y + 14 , x + 5, y + 10, WHITE);
  display->drawLine(x - 5, y + 12 , x + 5, y + 2 , WHITE);
  display->drawLine(x    , y      , x + 5, y + 2 , WHITE);
}


void Screen::add_canvas(canvas_func func, void* param){
  canvas.push_back(func);
  canvas_param.push_back(param);
}

uint8_t Screen::signal_to_level(uint8_t signal){
  if(signal < 10){
    return 0;
  }else if (signal >= 10 && signal < 20){
    return 1;
  }else if (signal >= 20 && signal < 40){
    return 2;
  }else if (signal >= 40 && signal < 60){
    return 3;
  }else if (signal >= 60 && signal < 80){
    return 4;
  }else if (signal >= 80){
    return 5;
  }
}

Screen::joystick_event Screen::read_joystick(){
  joystick_event ev = NONE;

  joystick_bounce.update();

  if(joystick_bounce.rose()){
    Serial.println("CLICK DETECTED.");
    ev = CLICK;
  }

  return ev;
}
