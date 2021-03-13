#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <MemoryFree.h>
#define MEM_CAP 2048

static void echo(int numArgs, String args[MAX_ARGS]) {
  for( uint8_t i = 1; i < min(numArgs,MAX_ARGS); i++ ) {
    serial.print(args[i]);
  }
}

static void info(int numArgs, String args[MAX_ARGS]) {
  serial.pageBreak();

  serial.writeBuffer(TERMINAL_WIDTH/2-4, "MAASyOS");
  serial.printBuffer();

  serial.pageBreak();
  
  serial.writeBuffer( 6, "VER: "+String( __DATE__ ) );
  serial.writeBuffer( 26, "TIM: "+clock.getTimeStr() );
  serial.printBuffer();
  
  serial.writeBuffer( 6, "RUN: "+clock.getUptimeStr() );
  serial.writeBuffer( 26, "MEM: "+String(MEM_CAP-freeMemory())+"/"+MEM_CAP+" B" );
  serial.printBuffer();

  serial.pageBreak();
  
  serial.writeBuffer( 6, "LIT: ON " );
  serial.writeBuffer( 26, "HUM: "+String(humidity)+" %" );
  serial.printBuffer();
  
  serial.writeBuffer( 6, "APM: ON " );
  serial.writeBuffer( 26, "ATP: "+String(airTemp)+" ºC" );
  serial.printBuffer();

  serial.writeBuffer( 6, "WPM: ON " );
  serial.writeBuffer( 26, "FLO: "+String(flowRate)+" Lpm" );
  serial.printBuffer();

  serial.writeBuffer( 6, "ACU: OFF" );
  serial.writeBuffer( 26, "WTP: "+String("00.00")+" ºC" );
  serial.printBuffer();

  serial.writeBuffer( 6, "VLV: OFF" );
  serial.writeBuffer( 26, "TDS: "+String("00.00")+" ppm" );
  serial.printBuffer();

  serial.writeBuffer( 6, "PHU: OFF" );
  serial.writeBuffer( 26, "ACD: "+String("07.05")+" pH" );
  serial.printBuffer();

  serial.writeBuffer( 6, "PHD: OFF" );
  serial.writeBuffer( 26, "LVL: "+String(waterContact==0.f?"OK":"HIGH") );
  serial.printBuffer();
  
  serial.writeBuffer( 6, "FDR: OFF" );
  serial.printBuffer();
  
  serial.pageBreak();
}

static void set(int numArgs, String args[MAX_ARGS]) {
  if( args[1] && args[2] ) {
    String id = args[1];
    id.toUpperCase();
    float value = args[2].toFloat();
    variables.setCvar( id, value );
    serial.print(id+"="+String(value));
  } else {
    serial.print("No such variable!");
  }
}

static void get(int numArgs, String args[MAX_ARGS]) {
  if( args[1] ) {
    String id = args[1];
    id.toUpperCase();
    float value = variables.getCvar( id );
    serial.print(id+"="+String(value));
  } else {
    serial.print("No such variable!");
  }
}

static void vars(int numArgs, String args[MAX_ARGS]) {
  serial.print( "Available variables:" );
  uint8_t numVars = variables.getNumVars();
  for( uint8_t i = 0; i < numVars; i++ ) {
    serial.print( variables.getCvarInfo(i) );
  }
}

static void time(int numArgs, String args[MAX_ARGS]) {
  if( args[1] && args[2] ) {
    clock.setCurrentTime( args[1].toInt(), args[2].toInt() );
    serial.print( "Clock set to: "+clock.getTimeStr() );
  }
}

void CommandConsole::initialize() {
  m_commands[0] = Command("echo", &echo);
  m_commands[1] = Command("info", &info);
  m_commands[2] = Command("vars", &vars);
  m_commands[3] = Command("get", &get);
  m_commands[4] = Command("set", &set);
  m_commands[5] = Command("time", &time);
}

void CommandConsole::listCommands() {
  for( uint8_t i = 0; i < m_numCommands; i++ ) {
    serial.print(m_commands[i].name);
  }
}

#endif
