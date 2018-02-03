#include "syscall.h"
/*
* In order to execute this test, go to ~/code/userprog
* Then run the following command ./nachos -rs 123 -x ../test/makethreads
* The output is  : 1 for the first thread, 2 for the second thread and 3 for the third .
* The character'a' stands for the main thread.
* In case of one of thre created threads has finished, a new thread will be created.
* In this example, thread 4 and thread 5 would take place of two threads.
*/


void print(void* arg)
{

#if 1
  PutChar((char)arg);
  PutChar('z');
  ThreadExit();
#endif

}

void print2(void* arg)
{
  int volatile i;
  for(i = 0; i < 5; i++){
    PutChar((char)arg);
  }
  PutString("\n");
  ThreadExit();
}


void print3(char c)
{
  int k;
  for(k=0; k<50; k++){
    PutChar(c);
  }
  PutString("\n");
}

int main()
{
  ThreadCreate(print2, '1');
  ThreadCreate(print2, '2');
  ThreadCreate(print2, '3');

  print3('a');

  ThreadCreate(print2, '4');
  ThreadCreate(print2, '5');

  ThreadExit();
}
