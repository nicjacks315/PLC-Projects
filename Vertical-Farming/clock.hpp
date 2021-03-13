#ifndef CLOCK_HPP
#define CLOCK_HPP

typedef unsigned long ms;

//global clock delta time
static ms dt = 0UL;
class Clock {
public :
  Clock() :
      m_startTime(0UL),
      m_elapsedTime(0UL),
      m_lastDelta(0UL),
      m_millisecondsUptime(0),
      m_secondsUptime(0),
      m_minutesUptime(0),
      m_hoursUptime(0),
      m_daysUptime(0),
      m_minutesCurrent(0),
      m_hoursCurrent(0),
      m_minutesOffset(0),
      m_hoursOffset(0) {}

  ms tick() {   
    m_lastDelta = millis()-m_startTime;
    m_startTime = millis();
    calculateUptime();
    calculateCurrentTime();
    return m_lastDelta;
  }

  void initialize() {
    char* tokens[3];
    tokenize(__TIME__,":",tokens);
    setCurrentTime( atoi(tokens[0]), atoi(tokens[1]) );
  }

  ms getElapsed() {
    return millis();
  }
  
  String getUptimeStr() {
    return leadingZero(m_daysUptime)+":"+leadingZero(m_hoursUptime)+":"+leadingZero(m_minutesUptime)+":"+leadingZero(m_secondsUptime);
  }

  String getTimeStr() {
    return leadingZero(m_hoursCurrent)+":"+leadingZero(m_minutesCurrent)+(m_am?" AM":" PM");
  }

  void setCurrentTime( int hour, int minute ) {
    m_hoursOffset = hour-m_hoursUptime;
    m_minutesOffset = minute-m_minutesUptime;
    calculateCurrentTime();
  }

private :
  ms m_startTime;
  ms m_elapsedTime;
  ms m_lastDelta;

  void calculateUptime() {
    m_millisecondsUptime += (int)m_lastDelta;
    if( m_millisecondsUptime >= 1000 ) {
      m_secondsUptime++;
      m_millisecondsUptime -= 1000;
      if( m_secondsUptime >= 60 ) {
        m_minutesUptime++;
        m_secondsUptime -= 60;
        if( m_minutesUptime >= 60 ) {
          m_hoursUptime++;
          m_minutesUptime -= 60;
          if( m_hoursUptime >= 24 ) {
            m_daysUptime++;
            m_hoursUptime -= 24;
          }
        }
      }
    }
  }

  void calculateCurrentTime() {
    int minutes = m_minutesUptime+m_minutesOffset;
    m_minutesCurrent = minutes>=60?minutes-60:minutes;
    
    int hours = m_hoursUptime+m_hoursOffset;
    m_hoursCurrent = hours>12?hours-12:hours;

    m_am = hours<12?true:false;
  }

  int m_millisecondsUptime;
  int m_secondsUptime;
  int m_minutesUptime;
  int m_hoursUptime;
  int m_daysUptime;

  int m_minutesCurrent;
  int m_hoursCurrent;
  int m_minutesOffset;
  int m_hoursOffset;
  bool m_am;
};

#endif
