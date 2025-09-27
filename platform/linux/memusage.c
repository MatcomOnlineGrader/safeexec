#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
// #include <sys/wait.h>

#include "error.h"
#include "memusage.h"

#define SIZE          8192      /* buffer size for reading /proc/<pid>/status */

void memusage_init (void)
{
}

int memusage (pid_t pid, char mem)
{
  char a[SIZE], *p, *q, *mem_hdr;
  int data, stack;
  int n, v, fd;

  if (mem == MEM_VMDATA){
    mem_hdr = "VmData:";
  }else if (mem == MEM_VMHWM){
    mem_hdr = "VmHWM:";
  }else if (mem == MEM_VMRSS){
    mem_hdr = "VmRSS:";
  }else{
    error ("mem param must be one of: MEM_VMDATA, MEM_VMHWM, MEM_VMRSS");
  }

  p = a;
  sprintf (p, "/proc/%d/status", pid);
  fd = open (p, O_RDONLY);
  if (fd < 0){
    if (errno == ENOENT){
      return 0;
    } else {
      error (NULL);
    }
  }

  do
    n = read (fd, p, SIZE);
  while ((n < 0) && (errno == EINTR));
  if (n < 0)
    error (NULL);
  do
    v = close (fd);
  while ((v < 0) && (errno == EINTR));
  if (v < 0)
    error (NULL);

  data = stack = 0;
  q = strstr (p, mem_hdr);
  if (q != NULL)
    {
      sscanf (q, "%*s %d", &data);
      q = strstr (q, "VmStk:");
      if (q != NULL)
        sscanf (q, "%*s %d\n", &stack);
    }

  return (data + stack);
}

void memusage_close (void)
{
}
