#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <SoftwareSerial.h>
#define BAUD_RATE 9600
#define TERMINAL_WIDTH 48

class SerialManager {
public :
  SerialManager(CommandConsole* pConsole, byte rxPin, byte txPin) :
    m_pConsole(pConsole),
    m_radio(rxPin,txPin),
    m_line(true) {
    memset(m_buffer,' ',TERMINAL_WIDTH);
  }

  void pageBreak() {
    for( byte i = 0; i < TERMINAL_WIDTH; i++ ) {
      Serial.print('-');
      m_radio.print('-');
    }
    Serial.println();
    m_radio.println();
  }

  //need to send this into a buffer, then slowly empty that buffer
  void print( char* c ) {
    Serial.println(c);
    m_radio.println(c);
  }

  void print( String s ) {
    Serial.println(s);
    m_radio.println(s);
  }

  void writeBuffer( uint8_t pos, String s ) {
    m_buffer[TERMINAL_WIDTH-1]='\0';
    for( uint8_t i = 0; i < s.length(); i++ ) {
      if( pos+i == TERMINAL_WIDTH ) return;
      m_buffer[pos+i] = s.charAt(i);
    }
  }

  void printBuffer() {
    Serial.println(String(m_buffer));
    m_radio.println(String(m_buffer));
    memset(m_buffer,' ',TERMINAL_WIDTH);
  }

  void initialize() {
    Serial.begin(BAUD_RATE);
    m_radio.begin(BAUD_RATE);
  }

  void update() {

    //process remote commands
    while( m_radio.available() > 0 ){
      String str = m_radio.readString();
      
      if( str.substring(0,2) == "OK" ) {    //the HM-10 is responding to an AT command
        Serial.println( str );
        
      } else {                              //a command is coming in from a remote device
        str = str.substring(0,str.length()-2);//trim the '\r\n'
        
        m_radio.print(">"); m_radio.println(str);
        Serial.print(">"); Serial.println(str);
        
        char input[32];
        str.toCharArray(input,32);
        char* cmd = strtok(input," ");
        String args[4];
        int count = 0;
        while( cmd ) {
          args[count] = String(cmd);
          cmd = strtok(0," ");
          count++;
        }
        m_pConsole->processCommand( count, args );       
      } 
    }

    //send HM-10 AT commands via the arduino serial monitor
    while( Serial.available() ) {
      char c = Serial.read();
          
      if (c != 0x0A && c != 0x0D) {//no carriage returns or line feed to the radio
        m_radio.write(c);
      }
  
      if( m_line ) {//echo the character just sent
        Serial.print("\r\n>");
        m_line = false;
      }
      Serial.write(c);
      if( c == 0x0A ) {
        m_line = true;
      }
    }
  }
  
private :
  char m_buffer[TERMINAL_WIDTH];
  SoftwareSerial m_radio;
  CommandConsole* m_pConsole;
  bool m_line;
};

#endif
