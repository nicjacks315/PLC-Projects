#ifndef AQUAPONICS_HPP
#define AQUAPONICS_HPP

//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of sensor class for DHT22
#include <DHT.h>
class SensorDht22 : public Sensor {
public :
  SensorDht22( unsigned long p, byte i, float* t, float* h ) : Sensor(p), m_pAirTemp(t), m_pAirHum(h), m_dht(i,DHT22) {}
  void init() {
    m_dht.begin();
  }

protected :
  void poll() {
    *m_pAirTemp = m_dht.readTemperature();
    *m_pAirHum = m_dht.readHumidity();
  }
  
private :
  float* m_pAirTemp;
  float* m_pAirHum;
  DHT m_dht;
};

//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of sensor class for QCRobot Infrared Water Contact Sensor
class SensorWaterLevel : public Sensor {
public :
  SensorWaterLevel( unsigned long p, byte i, float* w ) : Sensor(p), m_pWaterContact(w), m_pinIn(i) {}
  void init() {
    pinMode( m_pinIn, INPUT );
  }
  
protected :
  void poll() {
    *m_pWaterContact = digitalRead( m_pinIn );
  }

private :
  float* m_pWaterContact;
  byte m_pinIn;
};

//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of sensor class for Total Dissolved Solids Sensor
class SensorTds : public Sensor {
public :
  SensorTds( unsigned long p, byte i, float* f ) : Sensor(p), m_pPpm(f), m_pinIn(i) {}
  void init() {
    pinMode( m_pinIn, INPUT );
  }
  
protected :
  void poll() {
    *m_pPpm = analogRead( m_pinIn );
  }

private :
  float* m_pPpm;
  byte m_pinIn;
};

//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of sensor class for Total Dissolved Solids Sensor
class SensorPh : public Sensor {
public :
  SensorPh( unsigned long p, byte i, float* f ) : Sensor(p), m_pAcidity(f), m_pinIn(i) {}
  void init() {
    pinMode( m_pinIn, INPUT );
  }
  
protected :
  void poll() {
    *m_pAcidity = analogRead( m_pinIn );
  }

private :
  float* m_pAcidity;
  byte m_pinIn;
};


//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of sensor class for Total Dissolved Solids Sensor
class SensorFlow : public Sensor {
public :

  static volatile unsigned int k_pwmValue;
  static volatile unsigned int k_prevTime;
  static byte k_pinIn;

  SensorFlow( unsigned long p, byte i, float* f ) :
      Sensor(p),
      m_pFlow(f) {
    k_pinIn = i;
  }
      
  void init();
  
protected :
  void poll() {
    *m_pFlow = k_pwmValue>0?(1000000.f/((k_pwmValue)*2.f))/7.5f:0.f;
    k_pwmValue = 0;
  }

private :
  float* m_pFlow;
};

byte SensorFlow::k_pinIn = NULL;
volatile unsigned int SensorFlow::k_pwmValue;
volatile unsigned int SensorFlow::k_prevTime;

//foward declare
static void SensorFlow_Falling();

static void SensorFlow_Rising() {
  SensorFlow::k_prevTime = micros();
  attachInterrupt(digitalPinToInterrupt(SensorFlow::k_pinIn), &SensorFlow_Falling, FALLING);
}

static void SensorFlow_Falling() {
  SensorFlow::k_pwmValue = micros()-SensorFlow::k_prevTime;
  attachInterrupt(digitalPinToInterrupt(SensorFlow::k_pinIn), &SensorFlow_Rising, RISING);

}

static void SensorFlow::init() {
  pinMode(SensorFlow::k_pinIn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SensorFlow::k_pinIn), &SensorFlow_Rising, RISING);
}

#endif
