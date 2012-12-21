#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sosh.h"
#include "sosh-env.h"

extern char **environ;
extern const char *g_exec;

const char *
sosh_getenv (const char *key)
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
sosh_exeabspath (const char *p)
{
  size_t i, j, len, pos, p_len;
  bool complete;
  const char *value;

  value = sosh_getenv ("PATH");
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

