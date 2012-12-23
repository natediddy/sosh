#include <ctype.h>
#include <string.h>

#include "sosh-str.h"

ssize_t
sosh_str_fposof_c (const char *s, const char q)
{
  ssize_t pos;

  if ((!s || !*s) || !q)
    return -1;

  for (pos = 0; s[pos]; ++pos)
    if (s[pos] == q)
      return pos;
  return -1;
}

ssize_t
sosh_str_fposof (const char *s, const char *q)
{
  ssize_t pos;
  size_t q_pos;
  size_t n;
  size_t s_len = strlen (s);
  size_t q_len = strlen (q);

  if (!s_len || !q_len)
    return -1;

  if (q_len == 1)
    return sosh_str_fposof_c (s, q[0]);

  for (pos = 0; pos < s_len; ++pos)
  {
    if (s[pos] == q[0])
    {
      for (n = pos + 1, q_pos = 1; q_pos < q_len; ++n, ++q_pos)
        if (s[n] != q[q_pos])
          break;
      if (pos == (n - q_len))
        return pos;
    }
  }
  return -1;
}

ssize_t
sosh_str_lposof_c (const char *s, const char q)
{
  ssize_t pos;
  size_t s_len = strlen (s);

  if (!s_len || !q)
    return -1;

  for (pos = s_len - 1; pos >= 0; --pos)
    if (s[pos] == q)
      return pos;
  return -1;
}

ssize_t
sosh_str_lposof (const char *s, const char *q)
{
  ssize_t pos;
  ssize_t q_pos;
  ssize_t n;
  size_t s_len = strlen (s);
  size_t q_len = strlen (q);

  if (!s_len || !q_len)
    return -1;

  if (q_len == 1)
    return sosh_str_lposof_c (s, q[0]);

  for (pos == s_len - 1; pos >= 0; --pos)
  {
    if (s[pos] == q[q_len - 1])
    {
      for (n = pos - 1, q_pos = q_len - 2; n >= 0 && q_pos >= 0; --n, --q_pos)
        if (s[n] != q[q_pos])
          break;
      if (pos == (n + q_len))
        return pos;
    }
  }
  return -1;
}

bool
sosh_str_startswith_c (const char *s, const char x)
{
  if (sosh_str_fposof_c (s, x) == 0)
    return true;
  return false;
}

bool
sosh_str_startswith (const char *s, const char *x)
{
  if (sosh_str_fposof (s, x) == 0)
    return true;
  return false;
}

bool
sosh_str_endswith_c (const char *s, const char x)
{
  if (sosh_str_lposof_c (s, x) == (strlen (s) - 1))
    return true;
  return false;
}

bool
sosh_str_endswith (const char *s, const char *x)
{
  ssize_t s_len = strlen (s);
  ssize_t x_len = strlen (x);

  if ((s_len - x_len) < 0)
    return false;
  if (sosh_str_lposof (s, x) == (s_len - x_len))
    return true;
  return false;
}

bool
sosh_str_has_c (const char *s, const char q)
{
  if (sosh_str_fposof_c (s, q) != -1)
    return true;
  return false;
}

bool
sosh_str_has (const char *s, const char *q)
{
  if (sosh_str_fposof (s, q) != -1)
    return true;
  return false;
}

unsigned int
sosh_str_quantity_c (const char *s, const char q)
{
  size_t i;
  unsigned int r = 0;

  if (!s || !q)
    return r;

  for (i = 0; s[i]; ++i)
    if (s[i] == q)
      ++r;
  return r;
}

unsigned int
sosh_str_quantity (const char *s, const char *q)
{
  size_t pos;
  size_t q_pos;
  size_t n;
  size_t s_len = strlen (s);
  size_t q_len = strlen (q);
  unsigned int r = 0;

  if (!s_len || !q_len)
    return r;

  if (q_len == 1)
    return sosh_str_quantity_c (s, q[0]);

  for (pos = 0; pos < s_len; ++pos)
  {
    if (s[pos] == q[0])
    {
      for (n = pos + 1, q_pos = 1; n < s_len && q_pos < q_len; ++n, ++q_pos)
        if (s[n] != q[q_pos])
          break;
      if (pos == (n - q_len))
      {
        ++r;
        pos += q_len - 1;
      }
    }
  }
  return r;
}

char *
sosh_str_format (const char *fmt, ...)
{
  char *r = NULL;
  va_list ap;

  if (!fmt || !*fmt)
    return r;

  va_start (ap, fmt);
  if (vasprintf (&r, fmt, ap) < 0)
    r = NULL;
  va_end (ap);
  return r;
}

void
sosh_str_skip_white_pos (const char *s, size_t *pos)
{
  size_t n = strlen (s);

  while (*pos < n && isblank (s[*pos]))
    *++pos;
}

