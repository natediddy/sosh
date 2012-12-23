#ifndef SOSH_VAR_H_INCLUDED
#define SOSH_VAR_H_INCLUDED

#include "sosh.h"

typedef enum
{
  SOSH_VAR_TYPE_UNKNOWN,
  SOSH_VAR_TYPE_BOOLEAN,
  SOSH_VAR_TYPE_NUMBER,
  SOSH_VAR_TYPE_STRING,
  SOSH_VAR_TYPE_FILE
} SoshVarType;

typedef struct
{
  char *v;
  SoshVarType type;
} SoshVarValue;

typedef struct
{
  char *key;
  SoshVarValue value;
} SoshVar;

void sosh_var_init_lookup_table (void);
void sosh_var_set (const char *k, const char *v);
void sosh_var_value_of (SoshVarValue *svv, const char *k);
SoshVarType sosh_var_type_of (const char *k);
void sosh_var_obj (SoshVar *sv, const char *k);
bool sosh_var_is_valid (const char *k);

#endif /* SOSH_VAR_H_INCLUDED */

