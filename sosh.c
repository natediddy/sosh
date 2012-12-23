#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sosh.h"
#include "sosh-bltn.h"
#include "sosh-env.h"
#include "sosh-exec.h"

#define USER_INPUT_BUFFER_MAX 1024
#define DEFAULT_PS1 "$"

char *g_ps1 = DEFAULT_PS1;
const char *g_exec;

static void
display_prompt (void)
{
  fputs (g_ps1, stdout);
  fputc (' ', stdout);
}

static void
grab_input (char *input)
{
  size_t pos = 0;
  int c;

  while (true)
  {
    c = fgetc (stdin);
    if (c == '\n' || c == EOF || pos >= USER_INPUT_BUFFER_MAX)
      break;
    input[pos++] = c;
  }
  input[pos] = '\0';
}

static void
process_input (char *input)
{
  size_t i = 0;

  sosh_str_skip_white_pos (input, &i);
  for (; input[i] && input[i] != ' ' && input[i] != '\t'; ++i)
    ;

  char cmd[i + 1];

  strncpy (cmd, input, i);
  cmd[i] = '\0';

  if (cmd[0] == SOSH_BUILTIN_COMMAND_CHAR && i > 1)
    sosh_builtin_exec (cmd + 1, input + i);
  else
    sosh_external_exec (cmd, input + i);
}

static void
greet (void)
{
  fputs (DEFAULT_PROGRAM_NAME " " SOSH_VERSION "\n"
      "Type " SOSH_BUILTIN_HELP_CMD " for details on usage.\n", stdout);
}

static void
proper_exec_name (const char *n)
{
  if (n && *n)
  {
    char *s = strrchr (n, '/');
    if (s && *s)
    {
      g_exec = ++s;
      return;
    }
    g_exec = n;
    return;
  }
  g_exec = DEFAULT_PROGRAM_NAME;
}

int
main (int argc, char **argv)
{
  char input[USER_INPUT_BUFFER_MAX];

  proper_exec_name (argv[0]);
  greet ();

  while (true)
  {
    display_prompt ();
    grab_input (input);
    process_input (input);
  }
  return EXIT_SUCCESS;
}

