#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sosh.h"
#include "sosh-builtins.h"
#include "sosh-env.h"
#include "sosh-external.h"

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
    sosh_builtin_exec (buf + 1, input_buffer + i);
  else
    sosh_external_exec (buf, input_buffer + i);
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
    return;
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

