#ifndef METRIC_HPP
#define METRIC_HPP

#define MAX_DATAPOINTS 1

template<typename T>
struct Datapoint {
  Datapoint() : value(NULL), interval(0) {}
  T value;
  int interval;
};

//storage class for primitive data types
//  automatically records a history of the value
//  only adds a new data point if the value cahnged
//  stores time between datapoints in intervals
template<typename T>
class Metric {
public :
  Metric() : m_numValues(0), m_intervalCounter(0), m_intervalTotal(0) {}

  Metric<T>& operator=(T newValue);
  operator T();
  T* operator&();
  
  const T& getCurrentValue() {
    return m_value;
  }

  T getByIndex( uint8_t index ) {
    if( index < m_numValues ) {
      return m_history[index].value;
    }
    return NULL;
  }

  T* getVariable() {
    return &m_value;
  }

private :
  uint8_t m_numValues;
  T m_value;
  int m_intervalCounter;
  int m_intervalTotal;
  Datapoint<T> m_history[MAX_DATAPOINTS];

  void setCurrentValue( T newValue ) {
    m_intervalCounter++;
    m_intervalTotal++;
    if( m_value != newValue ) {
      m_value = newValue;
      pushValue( m_intervalCounter, newValue );
      m_intervalCounter = 0;
    }
  }
  
  void pushValue( int interval, T value ) {
    if( m_numValues == MAX_DATAPOINTS ) {
      for( uint8_t i = 0; i < m_numValues-1; i++ ) {
        m_history[i] = m_history[i+1];
      }
    } else {
      m_numValues++;
    }
    Datapoint<T>& d = m_history[m_numValues-1]; 
    d.interval = interval;
    d.value = value;
  }
  
};

template<typename T>
Metric<T>& Metric<T>::operator=(T newValue) {
  setCurrentValue(newValue);
  return *this;
}

template<typename T>
Metric<T>::operator T() {
  return m_value;
}

template<typename T>
T* Metric<T>::operator&() {
  return &m_value;
}

#endif
