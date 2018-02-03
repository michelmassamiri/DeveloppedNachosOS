#include "syscall.h"
/*
* In order to execute this test, go to ~/code
* Then run the following command ./userprog/nachos -rs 123 -x test/oneThread
* This test will create 2 process 'userpages0' and 'userpages1'
* 'userpages1' has one thread that prints a String
* The expected output is the total number of the printed string.
* The output is the following :
* "I am thread main\n" for the first process (userpages0)
* "aaa" 10 times and "bbb" 10 times so : 30 times 'a' and 30 times 'b' in total for the second process(userpages1)
*/

int main()
{
  ForkExec("test/userpages0");
  ForkExec("test/userpages1");
}
