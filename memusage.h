#ifndef MEMUSAGE_H
#define MEMUSAGE_H

#define MEM_VMDATA  0
#define MEM_VMRSS   1
#define MEM_VMHWM   2

int memusage (pid_t pid, char which);
void memusage_init (void);
void memusage_close (void);

#endif
