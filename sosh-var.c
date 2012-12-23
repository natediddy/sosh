#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sosh-bltn.h"
#include "sosh-var.h"

#define USER_VAR_MAX 256

#define VAR_UNDEFINED(i) \
  (!lookup_table[(i)].key && !lookup_table[(i)].value.v && \
   (lookup_table[(i)].value.type == SOSH_VAR_TYPE_UNKNOWN))

#define GOTO_LAST(i) \
  do \
  { \
    for (i = 0; i < USER_VAR_MAX; ++i) \
      if (VAR_UNDEFINED (lookup_table[i])) \
        break;
  } while (0)

SoshVar lookup_table[USER_VAR_MAX];

static bool
valid_boolean_type (char *v)
{
  if (sosh_expr_boolean_valid (v))
    return true;
  return false;
}

static bool
valid_number_type (char *v)
{
  size_t i;

  for (i = 0; v[i]; ++i)
    if (!isdigit (v[i]))
      return false;
  return true;
}

static SoshVarType
intended_type (char *v)
{
  size_t i;
  SoshVarType type = SOSH_VAR_TYPE_UNKNOWN;

  if (valid_boolean_type (v))
    type = SOSH_VAR_TYPE_BOOLEAN;
  else if (valid_number_type (v))
    type = SOSH_VAR_TYPE_NUMBER;
  else
    type = SOSH_VAR_TYPE_STRING;
  return type;
}

void
sosh_var_init_lookup_table (void)
{
  size_t i;

  for (i = 0; i < USER_VAR_MAX; ++i)
  {
    lookup_table[i].key = NULL;
    lookup_table[i].value.v = NULL;
    lookup_table[i].value.type = SOSH_VAR_TYPE_UNKNOWN;
  }
}

void
sosh_var_set (char *k, char *v)
{
  size_t i;

  GOTO_LAST (i);
  lookup_table[i].key = k;
  lookup_table[i].value.v = v;
  lookup_table[i].value.type = intended_type (v);
}

SoshVarValue *
sosh_var_value_of (const char *k)
{
  size_t i;

  for (i = 0; !VAR_UNDEFINED (i); ++i)
    if (!strcmp (lookup_table[i].key, k))
      return &lookup_table[i].value;
  return NULL;
}

SoshVarType
sosh_var_type_of (const char *k)
{
  size_t i;

  for (i = 0; !VAR_UNDEFINED (i); ++i)
    if (!strcmp (lookup_table[i].key, k))
      return lookup_table[i].value.type;
  return SOSH_VAR_TYPE_UNKNOWN;
}

SoshVar *
sosh_var_obj (const char *k)
{
  size_t i;

  for (i = 0; !VAR_UNDEFINED (i); ++i)
    if (!strcmp (lookup_table[i].key, k))
      return &lookup_table[i];
  return NULL;
}

bool
sosh_var_is_valid (const char *k)
{
  size_t i;

  for (i = 0; !VAR_UNDEFINED (i); ++i)
    if (!strcmp (lookup_table[i].key, k))
      return true;
  return false;
}

