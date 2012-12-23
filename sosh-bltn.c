#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sosh.h"
#include "sosh-bltn.h"

extern const char *g_exec;

static void
do_quit (void)
{
  printf ("%s: Goodbye...\n", g_exec);
  exit (EXIT_SUCCESS);
}

static void
do_help (void)
{
  fputs ("Builtin Commands:\n"
      "  " SOSH_BUILTIN_HELP_CMD "     Display this help message\n"
      "  " SOSH_BUILTIN_EXIT_CMD "     Exit the shell\n"
      "  " SOSH_BUILTIN_QUIT_CMD "     Exit the shell\n"
      "  " SOSH_BUILTIN_VERSION_CMD "  Display version information\n"
      "  " SOSH_BUILTIN_FILE_CMD " FILENAME\n"
      "                                Create file object from FILENAME\n",
      stdout);
}

static void
do_version (void)
{
  fputs (DEFAULT_PROGRAM_NAME " " SOSH_VERSION "\n"
      "Written by Nathan Forbes (2012)\n", stdout);
}

static void
do_file (const char *args)
{

}

static void
do_unrecognized (const char *c)
{
  fprintf (stderr, "%s: error: unrecognized command: '%c%s'\n",
      g_exec, SOSH_BUILTIN_COMMAND_CHAR, c);
}

void
sosh_builtin_exec (const char *cmd, const char *args)
{
  if (!strcmp (cmd, SOSH_BUILTIN_QUIT) || !strcmp (cmd, SOSH_BUILTIN_EXIT))
    do_quit ();
  else if (!strcmp (cmd, SOSH_BUILTIN_HELP))
    do_help ();
  else if (!strcmp (cmd, SOSH_BUILTIN_VERSION))
    do_version ();
  else if (!strcmp (cmd, SOSH_BUILTIN_FILE))
    do_file (args);
  else
    do_unrecognized (cmd);
}

