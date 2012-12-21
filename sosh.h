/*
 * sosh.h
 * Nathan Forbes
 */

#ifndef SOSH_H_INCLUDED
#define SOSH_H_INCLUDED

#define DEFAULT_PROGRAM_NAME "sosh"
#ifndef SOSH_VERSION
#  define SOSH_VERSION "X.XX-test"
#endif

#ifndef __cplusplus
typedef unsigned char __SOSH_Bool;
#  define bool __SOSH_Bool
#  define false ((bool) 0)
#  define true ((bool) 1)
#else
#  define bool bool
#  define false false
#  define true true
#endif

#endif /* SOSH_H_INCLUDED */

