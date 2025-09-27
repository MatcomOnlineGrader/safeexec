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

int memusage (pid_t pid)
{
  char path[64], line[256];
  FILE *f;
  int rss = -1;

  snprintf(path, sizeof(path), "/proc/%d/status", pid);
  f = fopen(path, "r");
  if (!f)
    return -1;

  while (fgets(line, sizeof(line), f))
  {
    if (strncmp(line, "VmRSS:", 6) == 0)
    {
      sscanf(line + 6, "%d", &rss);
      break;
    }
  }
  fclose(f);
  return rss; /* in kB */
}

void memusage_close (void)
{
}
