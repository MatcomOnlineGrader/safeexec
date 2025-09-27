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

int memusage (pid_t pid, bool use_vmrss)
{
  char a[SIZE], *p, *q;
  int data, stack;
  int n, v, fd;

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
  q = strstr (p, use_vmrss ? "VmRSS:" : "VmData:");
  if (q != NULL)
    {
      sscanf (q, "%*s %d", &data);
      if (!use_vmrss) {
        // If we use VmRSS, we don’t need to add
        // VmStk. That only matters for VmData.
        q = strstr (q, "VmStk:");
        if (q != NULL)
          sscanf (q, "%*s %d\n", &stack);
      }
    }

  return (data + stack);
}

void memusage_close (void)
{
}
