#ifndef LATCH_HPP
#define LATCH_HPP

#define MAX_LATCHES 4

struct Latch {
  Latch( float* i, float* s, float* r, byte q, byte o ) : pSignal( i ), pSet( s ), pReset( r ), outPin( o ), defState( q ) {}
  byte outPin;
  byte defState;
  float* pSignal;
  float* pSet;
  float* pReset;
};

class LatchManager {
public :
  LatchManager() {}

  void add( Latch* pLatch ) {
    m_pLatches[m_latchCount] = pLatch;
    m_latchCount++;
  }

  void initialize() {
     for( uint8_t i = 0; i < m_latchCount; i++ ) {
      pinMode( m_pLatches[i]->outPin, OUTPUT );
    }
  }

  void check() {
    for( uint8_t i = 0; i < m_latchCount; i++ ) {
      Latch* pLatch = m_pLatches[i];
      if( *pLatch->pSignal <= *pLatch->pSet ) {
        digitalWrite( pLatch->outPin, pLatch->defState );
      } else if( *pLatch->pSignal >= *pLatch->pReset ) {
        digitalWrite( pLatch->outPin, !pLatch->defState );
      }//else maintain state
    }
  }
  
private :
  Latch* m_pLatches[MAX_LATCHES];
  uint8_t m_latchCount = 0;
  
};

#endif
