#ifndef MEMUSAGE_H
#define MEMUSAGE_H

int memusage (pid_t pid, char use_vmrss);
void memusage_init (void);
void memusage_close (void);

#endif
