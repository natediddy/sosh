#include "sosh-str.h"

void
sosh_str_skip_white_pos (char *s, size_t *pos)
{
  size_t n = strlen (s);

  while (*pos < n && (s[*pos] == ' ' || s[*pos] == '\t'))
    *++pos;
}

