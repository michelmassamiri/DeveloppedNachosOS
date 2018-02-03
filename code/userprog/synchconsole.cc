#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore* readAvail;
static Semaphore* writeDone;
static Semaphore* mutexConsole;
static void ReadAvailHandler(void* arg)
{
  (void) arg;
  readAvail->V();
}

static void WriteDoneHandler(void* arg)
{
  (void) arg;
  writeDone->V();
}

SynchConsole::SynchConsole(const char* in, const char* out)
{
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);
  mutexConsole = new Semaphore("mutexConsole",1);
}

SynchConsole::~SynchConsole()
{
  delete console;
  delete writeDone;
  delete readAvail;
  delete mutexConsole;
}

void SynchConsole::SynchPutChar(int ch)
{
  mutexConsole->P();
  console->PutChar(ch);
  writeDone->P();
  mutexConsole->V();
}

int SynchConsole::SynchGetChar()
{
  mutexConsole->P();
  readAvail->P();
  int i = console->GetChar();
  mutexConsole->V();
  return i;
}

void SynchConsole::SynchPutString(const char s[])
{
  mutexConsole->P();
  int i = 0 ;
  while (s[i] != '\0') {
    console->PutChar(int(s[i]));//SynchPutChar(int(s[i]));
    writeDone->P();
    ++i;
  }
  mutexConsole->V();
}

void SynchConsole::SynchGetString(char* s, int n)
{
  mutexConsole->P();
  int i = 0;
  while (i < n){
    readAvail->P();
    s[i] =(char) console->GetChar();//SynchGetChar();
    if(s[i] == '\n' || s[i] == '\0'){
        s[i] = '\0';
          break ;
    }
    i++;
  }
  mutexConsole->V();
}

/*void SynchConsole::SynchPutInt(int ch)
{
  char* buffer =(char*) malloc(sizeof(char)*12);//1 caractere pour le signe
  snprintf(buffer, 12, "%d",ch);
  SynchPutString(buffer);
  free(buffer);
 }
*/
#endif // CHANGED
