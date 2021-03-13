#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#define MAX_ARGS 8
#define MAX_COMMANDS 8

typedef String arg_list[MAX_ARGS];
typedef void (*Callback)(int,arg_list);
struct Command {
  Command() {}
  Command( String n, Callback p ) : name(n), pFunc(p) {}
  String name;
  Callback pFunc;
};

class CommandConsole {
public :
  CommandConsole() : m_numCommands(0) {

  }

  void initialize();

  void listCommands();

  void processCommand( int numArgs, arg_list args ) {
    for( int i = 0; i < MAX_COMMANDS; i++ ) {
      if( args[0].equals(m_commands[i].name) ) {
        (*m_commands[i].pFunc)(numArgs,args);
      }
    }
  }

private :
  int m_numCommands;
  Command m_commands[MAX_COMMANDS];
  
};

#endif
