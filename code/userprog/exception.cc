// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "userthread.h"
#include "userprocessus.h"
#include <iostream>
#endif
//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

/*
* Function to Copy a string from a virtual memory.
* Param : from : the virtual address
*         to   : the variable that contains the string
*         size : the size of 'to'
*/
int copyStringFromMachine(int from, char* to, unsigned size) {
  unsigned i = 0 ;
  int * ch = (int*)malloc(sizeof(int*));
  machine->ReadMem(from, 1, ch);

  while(i < size) {
    if((char)*ch == '\0') {
      to[i] = (char)*ch;
      break;
    }
    to[i] = (int)* ch;
    ++i; from++ ;
    machine->ReadMem(from, 1, ch);
  }
  to[i] = '\0' ;
  free(ch);
  return i;
}


int copyStringToMachine(int to, char* from, unsigned size){
  unsigned i = 0 ;
  int tmp ;
  while(i < size) {

    if(from[i] == '\n' || from[i] == '\0') {
      tmp = (int)'\0' ;
      machine->WriteMem(to, 1, tmp);
      break;
    }

    tmp = (int)from[i] ;
    machine->WriteMem(to, 1, tmp);
    ++i; to++ ;

  }
  return i;
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------


void
ExceptionHandler (ExceptionType which)
{
  int type = machine->ReadRegister (2);

  switch (which)
    {
    case SyscallException:
      {
	switch (type)
	  {
	  case SC_Halt:
	    {
	      DEBUG ('s', "Shutdown, initiated by user program.\n");
	      interrupt->Halt ();
	      break;
	    }
#ifdef CHANGED
	  case SC_PutChar:
	    {
	      DEBUG('s',"PutChar\n");
	      int ch = machine->ReadRegister(4);
	      synchconsole->SynchPutChar(ch);
	      break;
	    }
	  case SC_PutString:
	    {
	      DEBUG('s',"PutString\n");
	      char *to = (char*)malloc(sizeof(char)*MAX_STRING_SIZE);
	      int from = machine->ReadRegister(4);
	      int i = MAX_STRING_SIZE ;
	      while(i == MAX_STRING_SIZE){
		i = copyStringFromMachine(from, to, MAX_STRING_SIZE);
		synchconsole->SynchPutString(to);
		from = from +i ;
	      }
	      free(to);
	      break;
	    }
	    /*case SC_PutInt:
	    {
	     DEBUG('s',"PutInt\n");
	      int ch = machine->ReadRegister(4);
	      synchconsole->SynchPutInt(ch);
	      break;
	      }*/
	  case SC_GetChar:
	    {
	      DEBUG('s',"GetChar\n");
	      int ch = synchconsole->SynchGetChar();
	      machine->WriteRegister(2, ch);
	      break;
	    }
	  case SC_GetString:
	    {
	      DEBUG('s',"GetString\n");
	      int to = machine->ReadRegister(4);
	      int size = machine->ReadRegister(5);
	      char* s;
	      if (size < MAX_STRING_SIZE){
		s =(char*) malloc(sizeof(char)*size);
		synchconsole->SynchGetString(s, size);
		copyStringToMachine(to, s, size);
	      }
	      else{
		s =(char*) malloc(sizeof(char)*MAX_STRING_SIZE);
	        int i=0;
		int buffer_size;
		do{
		  if(size < MAX_STRING_SIZE)
		    buffer_size = size;
		  else
		    buffer_size = MAX_STRING_SIZE;
		  
		  synchconsole->SynchGetString(s, buffer_size);
		  i = copyStringToMachine(to+i, s, buffer_size);
		  size = size -i;
		}while(i == MAX_STRING_SIZE);
	      }
	      free(s);
	      break;
	    }
	  case SC_ThreadCreate:
	    {
	      DEBUG('s',"ThreadCreate\n");
	      
	      int res = do_ThreadCreate( machine->ReadRegister(4), machine->ReadRegister(5));
	      if(res)
		printf("Thread's stack overflow\n");
	      break;
	    }
	  case SC_ThreadExit:
	    {
	      DEBUG('s',"ThreadExit\n");
	      
	      do_ThreadExit();
	      break;
	    }
	  case SC_ForkExec:
	    {
	      DEBUG('s',"ForkExec\n");

	      char *to = (char*)malloc(sizeof(char)*MAX_STRING_SIZE);
	      char* buf =  (char*)malloc(sizeof(char)*MAX_STRING_SIZE);	
	      int from = machine->ReadRegister(4);
	      
	      copyStringFromMachine(from, to, MAX_STRING_SIZE);
	     
	      ForkExecute(to);
	      break;
	    }
	  case SC_Exit:
	    {
	      DEBUG ('s', "Shutdown, initiated by user program.\n");
	      //do_ThreadExit();
	      ExitProcessus() ;
	      break;
	    }
#endif //CHANDED
	  default:
	    {
	      printf("Unimplemented system call %d\n", type);
	      ASSERT(FALSE);
	    }
	  }

	// Do not forget to increment the pc before returning!
	UpdatePC ();
	break;
      }

    case PageFaultException:
      if (!type) {
	printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	ASSERT (FALSE);
      } else {
	printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
	ASSERT (FALSE);	// For now
      }

    default:
      printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
      ASSERT (FALSE);
    }
}
