#ifdef CHANGED
#ifndef USERPROCESSUS_H
#define USERPROCESSUS_H

extern int ForkExecute(const char *filename);
extern int ExitProcessus() ;
extern Semaphore *mutex_cmpProcessus;
#endif //USERTHREAD
#endif //CHANGED
