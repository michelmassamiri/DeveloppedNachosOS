#include "syscall.h"

/*
* In order to execute this test, go to ~/code/userprog
* Then run the following command ./nachos -x ../test/getstring
* The output is the typed string, if the length of the string is bigger than 20, the output show the first 20 characters of the string.
*/

int main()
{
  char  s[20] ;
  GetString(s, 20);
  PutString(s);
  return 0;
}
