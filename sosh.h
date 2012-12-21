/*
 * sosh.h
 * Nathan Forbes
 */

#ifndef SoSH_H_INCLUDED
#define SoSH_H_INCLUDED

#ifndef __cplusplus
typedef unsigned char __SoSH_Bool;
#  define bool __SoSH_Bool
#  define false ((bool) 0)
#  define true ((bool) 1)
#else
#  define bool bool
#  define false false
#  define true true
#endif

#endif /* SoSH_H_INCLUDED */

