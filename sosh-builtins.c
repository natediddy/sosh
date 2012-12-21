#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sosh.h"
#include "sosh-builtins.h"

extern const char *g_exec;

void
sosh_builtin_exec (const char *cmd, const char *args)
{
  if (!strcmp (cmd, SOSH_BUILTIN_QUIT) || !strcmp (cmd, SOSH_BUILTIN_EXIT))
  {
    printf ("%s: goodbye...\n", g_exec);
    exit (EXIT_SUCCESS);
  }
  else if (!strcmp (cmd, SOSH_BUILTIN_HELP))
    fputs ("Builtin Commands:\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_HELP "     Display this help message\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_EXIT "     Exit the shell\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_QUIT "     Exit the shell\n"
        "  " SOSH_BUILTIN_COMMAND_CHAR_STRING
        SOSH_BUILTIN_VERSION "  Display version information\n", stdout);
  else if (!strcmp (cmd, SOSH_BUILTIN_VERSION))
    fputs (DEFAULT_PROGRAM_NAME " " SOSH_VERSION "\n"
        "Written by Nathan Forbes (2012)\n", stdout);
}

