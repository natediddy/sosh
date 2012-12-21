#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "sosh.h"
#include "sosh-env.h"
#include "sosh-external.h"

#define EXECV_ARGV_SIZE_MAX 32
#define ARG_BUFFER_SIZE_MAX 256

extern const char *g_exec;

void
sosh_external_exec (const char *cmd, const char *args)
{
  size_t i, j, x;
  pid_t child, w;
  int status, child_exit_status;
  char *xargv[EXECV_ARGV_SIZE_MAX];
  char buffer[ARG_BUFFER_SIZE_MAX];
  char *exe = NULL;

  if (cmd[0] != '/')
    exe = sosh_exeabspath (cmd);
  else
    exe = strdup (cmd);

  i = 0;
  while (args[i] == ' ' || args[i] == '\t')
    ++i;

  xargv[0] = exe;
  for (j = 0, x = 1; args[i]; ++i)
  {
    if (args[i] != ' ' && args[i] != '\t')
      buffer[j++] = args[i];
    if (!args[i + 1] || args[i + 1] == ' ' || args[i + 1] == '\t')
    {
      buffer[j] = '\0';
      j = 0;
      xargv[x++] = strdup (buffer);
    }
  }

  xargv[x] = NULL;
  child = fork ();

  if (child == -1)
    fprintf (stderr, "%s: error: fork: %s\n", g_exec, strerror (errno));
  else if (child == 0)
  {
    child_exit_status = EXIT_SUCCESS;
    if (execv (exe, xargv) == -1)
    {
      child_exit_status = errno;
      fprintf (stderr, "%s: error: execv: %s\n",
          g_exec, strerror (child_exit_status));
    }
    _exit (child_exit_status);
  }
  else
  {
    do
    {
      w = waitpid (child, &status, WUNTRACED | WCONTINUED);
      if (w == -1)
        fprintf (stderr, "%s: error: waitpid: %s\n",
            g_exec, strerror (errno));
    } while (!WIFEXITED (status) && !WIFSIGNALED (status));
  }

  for (x = 0; xargv[x]; ++x)
  {
    free (xargv[x]);
    xargv[x] = NULL;
  }
}

