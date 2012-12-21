#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "sosh.h"

#define DEFAULT_PROGRAM_NAME "sosh"
#ifndef SOSH_VERSION
#  define SOSH_VERSION "X.XX-test"
#endif
#define USER_INPUT_BUFFER_MAX 1024
#define EXECV_ARGV_SIZE_MAX 32
#define DEFAULT_PS1 "$"

#define SOSH_BUILTIN_COMMAND_CHAR ':'
#define SOSH_BUILTIN_COMMAND_CHAR_STRING ":"
#define SOSH_BUILTIN_HELP    "help"
#define SOSH_BUILTIN_EXIT    "exit"
#define SOSH_BUILTIN_QUIT    "quit"
#define SOSH_BUILTIN_VERSION "version"

extern char **environ;

char *g_ps1 = DEFAULT_PS1;
const char *g_exec;

static void
display_prompt (void)
{
  fputs (g_ps1, stdout);
  fputc (' ', stdout);
}

static void
grab_input (char *input_buffer)
{
  size_t pos = 0;
  int c;

  while (true)
  {
    c = fgetc (stdin);
    if (c == '\n' || c == EOF || pos >= USER_INPUT_BUFFER_MAX)
      break;
    input_buffer[pos++] = c;
  }
  input_buffer[pos] = '\0';
}

static const char *
environment_value_from_key (const char *key)
{
  size_t i;
  size_t k_len = strlen (key);
  const char *v;

  for (i = 0; environ[i]; ++i)
  {
    if (!strncmp (environ[i], key, k_len) && environ[i][k_len] == '=')
    {
      v = environ[i] + (k_len + 1);
      return v;
    }
  }
  return NULL;
}

static char *
absolute_program_path (const char *p)
{
  size_t i, j, len, pos, p_len;
  bool complete;
  const char *value;

  value = environment_value_from_key ("PATH");
  if (!value)
  {
    fprintf (stderr, "%s: environment variable 'PATH' not set!\n", g_exec);
    return NULL;
  }

  len = pos = 0;
  p_len = strlen (p);

  for (i = 0; value[i]; ++i)
  {
    if (value[i] == ':' || !value[i + 1])
    {
      if (!pos)
        len = i - 1;
      else if ((i - pos) > len)
        len = (i - pos) - 1;
      pos = i - 1;
    }
  }

  len += p_len + 1;
  pos = 0;
  complete = false;

  char buf[len];

  for (i = 0; value[i]; ++i)
  {
    if (value[i] == ':')
      complete = true;
    else
      buf[pos++] = value[i];
    if (!value[i + 1] && !complete)
      complete = true;
    if (complete)
    {
      buf[pos++] = '/';
      for (j = 0; p[j]; ++j)
        buf[pos++] = p[j];
      buf[pos] = '\0';
      if (!access (buf, X_OK))
        return strdup (buf);
      pos = 0;
      complete = false;
    }
  }
  return NULL;
}

static bool
execute_builtins (const char *cmd, const char *args)
{
  if (!strcmp (cmd, SOSH_BUILTIN_QUIT) || !strcmp (cmd, SOSH_BUILTIN_EXIT))
  {
    printf ("%s: goodbye...\n", g_exec);
    exit (EXIT_SUCCESS);
  }
  else if (!strcmp (cmd, SOSH_BUILTIN_HELP))
  {
    fputs ("Builtin Commands:\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_HELP "     Display this help message\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_EXIT "     Exit the shell\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_QUIT "     Exit the shell\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_VERSION "  Display version information\n", stdout);
    return true;
  }
  else if (!strcmp (cmd, SOSH_BUILTIN_VERSION))
  {
    fputs (DEFAULT_PROGRAM_NAME " " SOSH_VERSION "\n"
        "Written by Nathan Forbes (2012)\n", stdout);
    return true;
  }
  return false;
}

static void
execute_external_program (const char *cmd, const char *args)
{
  size_t i, j, x;
  pid_t child, w;
  int status, child_exit_status;
  char *xargv[EXECV_ARGV_SIZE_MAX];
  char buffer[USER_INPUT_BUFFER_MAX / 4];
  char *exe = NULL;

  if (cmd[0] != '/')
    exe = absolute_program_path (cmd);
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

static void
process_input (char *input_buffer)
{
  size_t i = 0;

  while (input_buffer[i] == ' ' || input_buffer[i] == '\t')
    ++i;
  for (; input_buffer[i] && input_buffer[i] != ' ' &&
      input_buffer[i] != '\t'; ++i)
    ;

  char buf[i + 1];

  strncpy (buf, input_buffer, i);
  buf[i] = '\0';

  if (buf[0] == SOSH_BUILTIN_COMMAND_CHAR)
    execute_builtins (buf + 1, input_buffer + i);
  else
    execute_external_program (buf, input_buffer + i);
}

static void
greet (void)
{
  fputs (DEFAULT_PROGRAM_NAME " " SOSH_VERSION "\n"
      "Type " SOSH_BUILTIN_COMMAND_CHAR_STRING SOSH_BUILTIN_HELP
      " for details on usage.\n", stdout);
}

static void
proper_exec_name (const char *n)
{
  char *l;

  if (n && *n)
  {
    l = strrchr (n, '/');
    if (l && *l)
    {
      g_exec = ++l;
      return;
    }
    g_exec = n;
  }
  g_exec = DEFAULT_PROGRAM_NAME;
}

int
main (int argc, char **argv)
{
  char user_input[USER_INPUT_BUFFER_MAX];

  proper_exec_name (argv[0]);
  greet ();
  while (true)
  {
    display_prompt ();
    grab_input (user_input);
    process_input (user_input);
  }
  return EXIT_SUCCESS;
}

