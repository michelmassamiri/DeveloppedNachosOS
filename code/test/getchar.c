#include "syscall.h"

/*
* In order to execute this test, go to ~/code/userprog
* Then run the following command ./nachos -x ../test/getchar
* The output is the typed character.
* To stop  the test, please hit ctrl-c 
*/

int main()
{
  while (1){
    int ch = GetChar();
    PutChar(ch);
  }
  return 0;
}
