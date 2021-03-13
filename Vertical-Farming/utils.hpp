#ifndef UTILS_HPP
#define UTILS_HPP

static String leadingZero( uint8_t n ) {
  if( (n-(n%10))/10 == 0 ) {
    return "0" + String(n);
  } else {
    return String(n);
  }
}

static void overlayStr( char* arr, String str ) {
  for( uint8_t i = 0; i < 16; i++ ) {
    if( i >= str.length() ) {
      arr[i]=' ';
    } else {
      arr[i]=str.charAt(i);
    }
  }
}

static void tokenize( char* array, const char* delimiters, char* strings[] ) {
  //char* strings[10];
  char* ptr = NULL;
  byte index = 0;
  ptr = strtok(array, delimiters);  // takes a list of delimiters
  while(ptr != NULL) {
      strings[index] = ptr;
      index++;
      ptr = strtok(NULL, delimiters);  // takes a list of delimiters
  }
  return strings;
}
#endif
