/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2014 by Michael Kohn
 *
 */

#ifndef _LPC_PARTS
#define _LPC_PARTS

#include <stdint.h>

// For sake of compile speed and cutting down .o files I'm just including
// this code in lpc.c.

struct _lpc_parts
{
  uint32_t id;
  char *part;
};

struct _lpc_parts lpc_parts[] =
{
  { 0x2524d02b, "LPC1112FHN33/101,LPC1112FHN33/102" },
  { 0x2516d02b, "LPC1111FHN33/101,LPC1111FHN33/102" },
  { 0x0a40902b, "LPC1114FDH28/102,LPC1114FN28/102" },
  { 0x2540902b, "LPC1114FHN33/201,LPC1114FHN33/202" },
  { 0x2540102b, "LPC1114FHN33/301,LPC1114FHN33/302,LPC1114FHI33/302,LPC1114FBD48/301,LPC1114FBD48/302,LPC11D14FBD100/302" },
  { 0x0434502b, "LPC1113FHN33/201" },
  { 0x041e502b, "LPC1111FHN33/101" },
  { 0x0425502b, "LPC1112FHN33/201" },
  { 0x042d502b, "LPC1112FHN33/101" },
  { 0x0444502b, "LPC1114FHN33/201" },
  { 0x0a07102b, "LPC1110FD20" },
  { 0x1a16d02b, "LPC1111FDH20/002" },
  { 0x1430102b, "LPC11C24FBD48/301" },
  { 0x2516902b, "LPC1111FHN33/201,LPC1111FHN33/202" },
  { 0x2524902b, "LPC1112FHN33/201,LPC1112FHN33/202,LPC1112FHN24/202,LPC1112FHI33/202" },
  { 0x00030030, "LPC1113FBD48/303,LPC1113FHN33/303" },
  { 0x1421102b, "LPC11C12FBD48/301" },
  { 0x00030032, "LPC1113FHN33/203" },
  { 0x0a24902b, "LPC1112FD20/102,LPC1112FDH20/102,LPC1112FDH28/102" },
  { 0x2532102b, "LPC1113FHN33/301,LPC1113FHN33/302,LPC1113FBD48/301,LPC1113FBD48/302" },
  { 0x1431102b, "LPC11C22FBD48/301" },
  { 0x0a16d02b, "LPC1111FDH20/002" },
  { 0x00020022, "LPC1112FHN33/203,LPC1112FHI33/203" },
  { 0x00020023, "LPC1112FHN33/103" },
  { 0x1a40902b, "LPC1114FDH28/102,LPC1114FN28/102" },
  { 0x0434102b, "LPC1113FHN33/301,LPC1113FBD48/301" },
  { 0x0416502b, "LPC1111FHN33/201" },
  { 0x1a24902b, "LPC1112FD20/102,LPC1112FDH20/102,LPC1112FDH28/102" },
  { 0x1a07102b, "LPC1110FD20" },
  { 0x00040042, "LPC1114FHN33/203" },
  { 0x00040040, "LPC1114FBD48/303,LPC1114FHN33/303,LPC1114FHI33/303" },
  { 0x00040060, "LPC1114FBD48/323" },
  { 0x1440102b, "LPC11C14FBD48/301" },
  { 0x00050080, "LPC1115FBD48/303" },
  { 0x00040070, "LPC1114FBD48/333,LPC1114FHN33/333" },
  { 0x2532902b, "LPC1113FHN33/201,LPC1113FHN33/202" },
  { 0x0444102b, "LPC1114FHN33/301,LPC1114FBD48/301" },
  { 0x00010012, "LPC1111FHN33/203" },
  { 0x00010013, "LPC1111FHN33/103" },
  { 0, NULL }
};

#endif

