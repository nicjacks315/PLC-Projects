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

//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of pH sensor class
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
//implementation of sensor class for pwm flowmeter
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

//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of sensor class for tds and ppm

#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point


class SensorTds : public Sensor {
public :
  SensorTds( unsigned long p, byte i, float* f ) : Sensor(p), m_pTds(f), m_pinIn(i) {}
  void init() {
    pinMode( m_pinIn, INPUT );
  }
  
protected :
  void poll() {

    m_analogBuffer[m_analogBufferIndex] = analogRead(m_pinIn); //read the analog value and store into the buffer
    m_analogBufferIndex++;
    if( m_analogBufferIndex == SCOUNT ) {
      m_analogBufferIndex = 0;
    }
      
    for( m_copyIndex = 0; m_copyIndex < SCOUNT; m_copyIndex++ ) {
      m_analogBufferTemp[m_copyIndex] = m_analogBuffer[m_copyIndex];
    }
      
    m_averageVoltage = getMedianNum(m_analogBufferTemp,SCOUNT) * (float)VREF/ 1024.0;       // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient=1.0+0.02*(m_temperature-25.0);                          //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge=m_averageVoltage/compensationCoefficient;                   //temperature compensation
    *m_pTds=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
  }

private :
  float* m_pTds;
  byte m_pinIn;

  int m_analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
  int m_analogBufferTemp[SCOUNT];
  int m_analogBufferIndex = 0;
  int m_copyIndex = 0;
  float m_averageVoltage = 0;
  float m_temperature = 25;
};

//-------------------------------------------------------------------------------------------------------------------------------------
//implementation of sensor class for water temperature sensor

#include <OneWire.h> 
#include <DallasTemperature.h>

class SensorWaterTemp : public Sensor {
public :
  SensorWaterTemp( unsigned long p, byte i, float* f ) : Sensor(p), m_pWaterTemp(f), m_pinIn(i), m_oneWire(i), m_sensors(&m_oneWire) {}
  void init() {
    m_sensors.begin();
  }
  
protected :
  void poll() {
    m_sensors.requestTemperatures();
    *m_pWaterTemp = m_sensors.getTempCByIndex(0);
  }

private :
  float* m_pWaterTemp;
  byte m_pinIn;
  OneWire m_oneWire;
  DallasTemperature m_sensors;

};


//-------------------------------------------------------------------------------------------------------------------------------------


#endif
