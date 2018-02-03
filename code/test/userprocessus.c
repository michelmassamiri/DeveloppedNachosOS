#include "syscall.h"

int main(){
  ForkExec("test/userpages1");
  ForkExec("test/userpages1");
  ForkExec("test/userpages1");
  ForkExec("test/userpages1");
  ForkExec("test/userpages1");
  ForkExec("test/userpages1");

  ForkExec("test/userpages0");
  ForkExec("test/userpages0");
  ForkExec("test/userpages0");
  ForkExec("test/userpages0");
  ForkExec("test/userpages0");
  ForkExec("test/userpages0");
}
