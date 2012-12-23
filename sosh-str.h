#ifndef SOSH_STR_H_INCLUDED
#define SOSH_STR_H_INCLUDED

#include "sosh.h"

ssize_t sosh_str_fposof_c (const char *s, const char q);
ssize_t sosh_str_fposof (const char *s, const char *q);
ssize_t sosh_str_lposof_c (const char *s, const char q);
ssize_t sosh_str_lposof (const char *s, const char *q);
bool sosh_str_startswith_c (const char *s, const char x);
bool sosh_str_startswith (const char *s, const char *x);
bool sosh_str_endswith_c (const char *s, const char x);
bool sosh_str_endswith (const char *s, const char *x);
bool sosh_str_has_c (const char *s, const char q);
bool sosh_str_has (const char *s, const char *q);
unsigned int sosh_str_quantity_c (const char *s, const char q);
unsigned int sosh_str_quantity (const char *s, const char *q);
char *sosh_str_format (const char *fmt, ...);
void sosh_str_skip_white_pos (const char *s, size_t *pos);

#endif /* SOSH_STR_H_INCLUDED */

