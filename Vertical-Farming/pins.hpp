#ifndef PINS_HPP
#define PINS_HPP

#define NUM_PINS_UNO 20

enum ePinType { DIGITAL = 0, ANALOG = 1 };

class Pin {
public :
  Pin( byte id, byte type, int value ) : m_id(id), m_type(type), m_value(value), m_fix(-1), m_bChanged(false) {}

  void hold( int value ) {
    m_bChanged = (m_fix!=value);
    m_fix = value;
  }

  void release() {
    m_bChanged = (m_fix!=-1);
    m_fix  = -1;
  }
  
  void write( int value ) {
    m_bChanged = (m_value!=value);
    m_value = value;
  }

  int read() {
    return m_value;
  }

  void setMode( const byte mode ) {
    pinMode( m_id, mode );
  }

  void update() {
  if( m_bChanged ) {
      if( m_fix == -1 ) {
        if( m_type == DIGITAL ) {
          digitalWrite( m_id, m_value );
        } else if( m_type == ANALOG ) {
          analogWrite( m_id, m_value );
        }
      } else {
        digitalWrite( m_fix, m_value );
      }
    }
  }

private :
  byte m_id;
  byte m_type;
  bool m_bChanged;
  int m_value;
  int m_fix;
};

#endif
