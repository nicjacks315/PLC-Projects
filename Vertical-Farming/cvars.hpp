#ifndef CVARS_HPP
#define CVARS_HPP

#define MAX_CVARS 8

struct ControlVariable {
  ControlVariable() {}
  ControlVariable( float* p, String n, String d ) : pVar(p), id(n), description(d) {}
  String id;
  String description;
  float* pVar;
};



class VariableManager {
public :

  VariableManager() : m_numVars(0) {}

  void addCvar( float* pVar, char* id, char* description ) {
    m_vars[m_numVars] = ControlVariable( pVar, id, description );
    m_numVars++;
  }

  void setCvar( String id, float value ) {
    for( uint8_t i = 0; i < m_numVars; i++ ) {
      if( m_vars[i].id == id ) {
      //if( strcmp( m_vars[i].id, id ) == 0 ) {
        *m_vars[i].pVar = value;
        return;
      }
    }
  }

  float getCvar( String id ) {
    for( uint8_t i = 0; i < MAX_CVARS; i++ ) {
      if( m_vars[i].id == id ) {
      //if( strcmp( m_vars[i].id, id ) == 0 ) {
        return *m_vars[i].pVar;
      }
    }
  }

  String getCvarInfo( uint8_t index ) {
    return String(m_vars[index].id)+" "+String(m_vars[index].description);
  }

  uint8_t getNumVars() {
    return m_numVars;
  }

private :
  uint8_t m_numVars;
  ControlVariable m_vars[MAX_CVARS];
};

#endif
