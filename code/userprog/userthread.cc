#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"
#include "synchconsole.h"
#include "userprocessus.h"

Semaphore* sem_counter = new Semaphore("thread counter",1);

static void StartUserThread(void *schmurtz)
{
  DEBUG('x', "mon debug %d\n", ((int *)schmurtz)[1]);
  int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    machine->WriteRegister(4, ((int *)schmurtz)[1]);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, ((int *)schmurtz)[0]);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg,currentThread->space->AllocateUserStack(((int *)schmurtz)[2]));
    //DEBUG ('a', "Initializing stack register to 0x%x\n",
    //   currentThread->space->AllocateUserStack());
    delete (int *)schmurtz;
    machine->Run();
}

int do_ThreadCreate(int f, int arg)
{

  int *schmurtz = new int(3);
  schmurtz[0] = f;
  schmurtz[1] = arg;

  int which = currentThread->space->GetBitMap()->Find();
  if( which != -1){

    Thread* newThread = new Thread("newThread");

    schmurtz[2] = which;
    newThread->SetWhichThread(which);

    sem_counter->P();
    int threadCompteur = currentThread->space->GetThreadCompteur()+1 ;
    currentThread->space->SetThreadCompteur(threadCompteur);
    sem_counter->V();

    newThread->Start(StartUserThread, schmurtz);
    return 0;
  }

  return -1;
}

int do_ThreadExit()
{
  //decrease compteur of one and if compteur is equal to 0 exit all thread
  sem_counter->P();
  int threadCompteur = currentThread->space->GetThreadCompteur()-1 ;
  currentThread->space->SetThreadCompteur(threadCompteur);
  if( currentThread->space->GetThreadCompteur() == 0)
    interrupt->Halt();
  sem_counter->V();

  //free bit of bitmap where the thread was stored
  int which = currentThread->GetWhichThread();
  currentThread->space->GetBitMap()->Clear(which);
  currentThread->Finish();

  return 0;
}



#endif //CHANGED
