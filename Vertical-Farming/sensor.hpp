#ifndef SENSOR_HPP
#define SENSOR_HPP

#define MAX_SENSORS 8

class Sensor {
public :
  Sensor( unsigned long p ) : m_timer(0UL), m_period(p) {}
  virtual void init() = 0;
  void update() {
    m_timer += dt;
    if( m_timer >= m_period ) {
      m_timer -= m_period;
      poll();
    }
  }
  
protected :
  virtual void poll() = 0;
  
private :
  unsigned long m_timer;
  unsigned long m_period;
  
};

//-------------------------------------------------------------------------------------------------------------------------------------

class SensorManager {
public :

  void add( Sensor* pSensor ) {
    m_pSensors[m_sensorCount] = pSensor;
    m_sensorCount++;
  }
  
  void initialize() {
     for( uint8_t i = 0; i < m_sensorCount; i++ ) {
      m_pSensors[i]->init();
    }
  }
  
  void update() {
    for( uint8_t i = 0; i < m_sensorCount; i++ ) {
      m_pSensors[i]->update();
    }
  }

private :
  uint8_t m_sensorCount;
  Sensor* m_pSensors[MAX_SENSORS];
  
};

#endif
