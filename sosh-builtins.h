#ifndef SOSH_BUILTINS_H_INCLUDED
#define SOSH_BUILTINS_H_INCLUDED

#define SOSH_BUILTIN_COMMAND_CHAR ':'
#define SOSH_BUILTIN_COMMAND_CHAR_STRING ":"
#define SOSH_BUILTIN_HELP    "help"
#define SOSH_BUILTIN_EXIT    "exit"
#define SOSH_BUILTIN_QUIT    "quit"
#define SOSH_BUILTIN_VERSION "version"

void sosh_builtin_exec (const char *cmd, const char *args);

#endif /* SOSH_BUILTINS_H_INCLUDED */

