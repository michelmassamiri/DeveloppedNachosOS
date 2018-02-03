#include "syscall.h"
/*
* In order to execute this test, go to ~/code/userprog
* Then run the following command ./nachos -rs 123 -x ../test/StringThreads
* The output is  : 1 for the first thread, 2 for the second thread and 3 for the third .
* The "I am the main thread" stands for the main thread.
* In case of one of these created threads has finished, a new thread will be created.
* In this example, thread 4 and thread 5 would cause an error "Thread's Stack overflow".
* In case of "Thread's Stack overflow", thread 4 and 5 will not be created .
*/

void printString(void* arg) {
  PutString((char*)arg);
  ThreadExit();
}

int main()
{
  ThreadCreate(printString, "I am thread 1\n");
  ThreadCreate(printString, "I am thread 2\n");
  ThreadCreate(printString, "I am thread 3\n");

  PutString("I am the main thread\n") ;

  ThreadCreate(printString, "I am thread 4\n");
  ThreadCreate(printString, "I am thread 5\n");

  ThreadExit();
}
