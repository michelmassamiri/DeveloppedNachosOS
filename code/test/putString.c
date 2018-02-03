#include "syscall.h"

/*
* In order to execute this test, go to ~/code/userprog
* Then run the following command ./nachos -x ../test/putString
* The output is "Bonjour" followed in the next line by "bonjourmichelleenzo".
*/


void print(char* s)
{
#if 1
  PutString(s);
#endif
}

int main()
{
  //cas inferieur a MAX_STRING_SIZE
  print("Bonjour");
  print("\n");
  //cas supérieur a MAX_STRING_SIZE
  print("bonjourmichelleenzo");
  print("\n");
  return 0;
  //Halt();
}
