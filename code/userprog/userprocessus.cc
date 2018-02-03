#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"

Semaphore* mutex_cmpProcessus = new Semaphore("mutex compter processus",1);

void start(void* arg){

  currentThread->space->InitRegisters ();	// set the initial register values
  currentThread->space->RestoreState ();	// load page table register

  machine->Run ();		// jump to the user progam
  ASSERT (FALSE);
  // machine->Run never returns;
  // the address space exits
  // by doing the syscall "exit"
}

int ForkExecute(const char *filename){
  OpenFile *executable = fileSystem->Open (filename);
  AddrSpace *space;

  if (executable == NULL)
    {
      printf ("Unable to open file %s\n", filename);
      return 1;
    }
  space = new AddrSpace (executable);

  Thread* threadProcessus = new Thread("thread_processus");
  threadProcessus->space = space;
  mutex_cmpProcessus->P();
  cmpProcessus++;
  mutex_cmpProcessus->V();
  
  delete executable;		// close file

  threadProcessus->Start(start, space);

  return 0;
}

int ExitProcessus() {
  mutex_cmpProcessus->P();
  cmpProcessus--;
  if(cmpProcessus == 0) 
    interrupt->Halt() ;
  
  mutex_cmpProcessus->V();
  delete(currentThread->space);
  currentThread->Finish() ;

  

  return 0 ;

}
#endif //CHANGED
