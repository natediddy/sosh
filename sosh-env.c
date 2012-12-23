#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sosh.h"
#include "sosh-env.h"

extern char **environ;
extern const char *g_exec;

const char *
sosh_env_getvar (const char *key)
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

char *
sosh_env_executable_abspath (const char *name)
{
  size_t i;
  size_t j;
  size_t pos;
  size_t len;
  size_t n_len;
  bool complete;
  const char *env_paths;

  env_paths = sosh_env_getvar ("PATH");
  if (!env_paths)
  {
    fprintf (stderr, "%s: environment variable 'PATH' not set!\n", g_exec);
    return NULL;
  }

  len = pos = 0;
  n_len = strlen (name);

  for (i = 0; env_paths[i]; ++i)
  {
    if (env_paths[i] == ':' || !env_paths[i + 1])
    {
      if (!pos)
        len = i - 1;
      else if ((i - pos) > len)
        len = (i - pos) - 1;
      pos = i - 1;
    }
  }

  len += n_len + 1;
  pos = 0;
  complete = false;

  char buf[len];

  for (i = 0; env_paths[i]; ++i)
  {
    if (env_paths[i] == ':')
      complete = true;
    else
      buf[pos++] = env_paths[i];
    if (!env_paths[i + 1] && !complete)
      complete = true;
    if (complete)
    {
      buf[pos++] = '/';
      for (j = 0; p[j]; ++j)
        buf[pos++] = p[j];
      buf[pos] = '\0';
      if (!access (buf, X_OK))
        return strndup (buf, pos);
      pos = 0;
      complete = false;
    }
  }
  return NULL;
}

