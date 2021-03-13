#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <LiquidCrystal.h>

#define BUFFER_WIDTH 16
#define BUFFER_HEIGHT 12
#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 2

//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
#define PIN_LCD_RS 13
#define PIN_LCD_EN 12
#define PIN_LCD_D4 11
#define PIN_LCD_D5 10
#define PIN_LCD_D6 9
#define PIN_LCD_D7 8

class Display {
public :
  Display() : m_lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7) {}

  void init() {
    m_lcd.begin(SCREEN_WIDTH, SCREEN_HEIGHT);
  }

  void writeToBuffer( uint8_t line, String text ) {
    m_buffer[line] = text;
  }

  void updateScreen() {
    
  }

  void draw() {
    m_lcd.setCursor(0,0);
    for( uint8_t i = 0; i < SCREEN_WIDTH; i++ ) {
      if( i < m_buffer[m_line].length() ) {
        m_lcd.print( m_buffer[m_line].charAt(i) );
      } else {
        m_lcd.print(' ');
      }
    } 
    m_lcd.setCursor(0,1);
    uint8_t nextLine = (m_line+1==BUFFER_HEIGHT?0:m_line+1);
    for( uint8_t i = 0; i < SCREEN_WIDTH; i++ ) {
      if( i < m_buffer[nextLine].length() ) {
        m_lcd.print( m_buffer[nextLine].charAt(i) );
      } else {
        m_lcd.print(' ');
      }
    }
  }

  void nextLine() {
    m_line++;
    if( m_line == BUFFER_HEIGHT ) {
      m_line = 0;
    }
  }

private :
  LiquidCrystal m_lcd;
  uint8_t m_line;
  String m_buffer[BUFFER_HEIGHT];

};


#endif
