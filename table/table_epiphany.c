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

#include <stdio.h>
#include <stdlib.h>
#include "disasm_epiphany.h"
#include "table_epiphany.h"

struct _table_epiphany table_epiphany[] = {
  { "beq",   0x0000, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bne",   0x0010, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bgtu",  0x0020, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bgteu", 0x0030, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "blteu", 0x0040, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bltu",  0x0050, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bgt",   0x0060, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bgte",  0x0070, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "blt",   0x0080, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "blte",  0x0090, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bbeq",  0x00a0, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bbne",  0x00b0, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bblt",  0x00c0, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bblte", 0x00d0, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "b",     0x00e0, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "bl",    0x00f0, 0x00ff, OP_BRANCH_16, 16, 0, 0 },
  { "beq",   0x00000008, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bne",   0x00000018, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bgtu",  0x00000028, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bgteu", 0x00000038, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "blteu", 0x00000048, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bltu",  0x00000058, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bgt",   0x00000068, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bgte",  0x00000078, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "blt",   0x00000088, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "blte",  0x00000098, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bbeq",  0x000000a8, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bbne",  0x000000b8, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bblt",  0x000000c8, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bblte", 0x000000d8, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "b",     0x000000e8, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },
  { "bl",    0x000000f8, 0x000000ff, OP_BRANCH_32, 32, 0, 0 },

  { "ldrb", 0x0004, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "ldrh", 0x0024, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "ldr",  0x0044, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "ldrd", 0x0064, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "strb", 0x0014, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "strh", 0x0034, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "str",  0x0054, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "strd", 0x0074, 0x007f, OP_DISP_IMM3_16, 16, 0, 0 },
  { "ldrb", 0x0001, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "ldrh", 0x0021, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "ldr",  0x0041, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "ldrd", 0x0061, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "strb", 0x0011, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "strh", 0x0031, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "str",  0x0051, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "strd", 0x0071, 0x007f, OP_INDEX_16, 16, 0, 0 },
  { "ldrb", 0x0005, 0x007f, OP_POST_MOD_16, 16, 0, 0 },
  { "ldrh", 0x0025, 0x007f, OP_POST_MOD_16, 16, 0, 0 },
  { "ldr",  0x0045, 0x007f, OP_POST_MOD_16, 16, 0, 0 },
  { "ldrd", 0x0065, 0x007f, OP_POST_MOD_16, 16, 0, 0 },
  { "strb", 0x0015, 0x007f, OP_POST_MOD_16, 16, 0, 0 },
  { "strh", 0x0035, 0x007f, OP_POST_MOD_16, 16, 0, 0 },
  { "str",  0x0055, 0x007f, OP_POST_MOD_16, 16, 0, 0 },
  { "strd", 0x0075, 0x007f, OP_POST_MOD_16, 16, 0, 0 },

  { "ldrb", 0x0000000c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "ldrh", 0x0000002c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "ldr",  0x0000004c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "ldrd", 0x0000006c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "strb", 0x0000001c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "strh", 0x0000003c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "str",  0x0000005c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "strd", 0x0000007c, 0x0200007f, OP_DISP_IMM11_32, 32, 0, 0 },
  { "ldrb", 0x0200000c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "ldrh", 0x0200002c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "ldr",  0x0200004c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "ldrd", 0x0200006c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "strb", 0x0000001c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "strh", 0x0200003c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "str",  0x0200005c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "strd", 0x0200007c, 0x0200007f, OP_POST_MOD_DISP_32, 32, 0, 0 },
  { "ldrb", 0x00000009, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "ldrh", 0x00000029, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "ldr",  0x00000049, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "ldrd", 0x00000069, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "strb", 0x00000019, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "strh", 0x00000039, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "str",  0x00000059, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "strd", 0x00000079, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "testsetb", 0x00200001, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "testseth", 0x00200021, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "testset",  0x00200041, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "testsetd", 0x00200061, 0x006f007f, OP_INDEX_32, 32, 0, 0 },
  { "ldrb", 0x0000000d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },
  { "ldrh", 0x0000002d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },
  { "ldr",  0x0000004d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },
  { "ldrd", 0x0000006d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },
  { "strb", 0x0000001d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },
  { "strh", 0x0000003d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },
  { "str",  0x0000005d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },
  { "strd", 0x0000007d, 0x006f007f, OP_POST_MOD_32, 32, 0, 0 },

  { "mov", 0x0003, 0x001f, OP_REG_IMM_16, 16, 0, 0 },
  { "mov",  0x0000000b, 0x100f001f, OP_REG_IMM_32, 32, 0, 0 },
  { "movt", 0x1001000b, 0x100f001f, OP_REG_IMM_32, 32, 0, 0 },
  { "add", 0x0013, 0x007f, OP_REG_2_IMM_16, 16, 0, 0 },
  { "sub", 0x0033, 0x007f, OP_REG_2_IMM_16, 16, 0, 0 },
  { "add",  0x0000001b, 0x0300001f, OP_REG_2_IMM_32, 32, 0, 0 },
  { "sub",  0x0000003b, 0x0300001f, OP_REG_2_IMM_32, 32, 0, 0 },

  { "lsr",  0x0006, 0x001f, OP_REG_2_IMM5_16, 16, 0, 0 },
  { "lsl",  0x0016, 0x001f, OP_REG_2_IMM5_16, 16, 0, 0 },
  { "asr",  0x000e, 0x001f, OP_REG_2_IMM5_16, 16, 0, 0 },
  { "bitr", 0x001e, 0x001f, OP_REG_2_IMM5_16, 16, 0, 0 },
  { "lsr",  0x0006000f, 0x03ff001f, OP_REG_2_IMM5_32, 32, 0, 0 },
  { "lsl",  0x0006001f, 0x03ff001f, OP_REG_2_IMM5_32, 32, 0, 0 },
  { "asr",  0x000e000f, 0x03ff001f, OP_REG_2_IMM5_32, 32, 0, 0 },
  { "bitr", 0x000e001f, 0x03ff001f, OP_REG_2_IMM5_32, 32, 0, 0 },

  { "add", 0x001a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "sub", 0x003a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "and", 0x005a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "orr", 0x007a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "eor", 0x000a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "asr", 0x006a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "lsr", 0x004a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "lsl", 0x002a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "add", 0x000a001f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "sub", 0x000a003f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "and", 0x000a005f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "orr", 0x000a007f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "eor", 0x000a000f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "asr", 0x000a006f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "lsr", 0x000a004f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "lsl", 0x000a002f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },

  { "fadd",  0x0007, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "iadd",  0x0007, 0x007f, OP_REG_3_16, 16, 0, 0 }, // DUP
  { "fsub",  0x0017, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "isub",  0x0017, 0x007f, OP_REG_3_16, 16, 0, 0 }, // DUP
  { "fmul",  0x0027, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "imul",  0x0027, 0x007f, OP_REG_3_16, 16, 0, 0 }, // DUP
  { "fmadd", 0x003a, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "imadd", 0x003a, 0x007f, OP_REG_3_16, 16, 0, 0 }, // DUP
  { "fmsub", 0x0047, 0x007f, OP_REG_3_16, 16, 0, 0 },
  { "imsub", 0x0047, 0x007f, OP_REG_3_16, 16, 0, 0 }, // DUP
  { "float", 0x0057, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "fix",   0x0067, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "fabs",  0x0077, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "fadd",  0x0007000f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "iadd",  0x0007000f, 0x007f007f, OP_REG_3_32, 32, 0, 0 }, // DUP
  { "fsub",  0x0007001f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "isub",  0x0007001f, 0x007f007f, OP_REG_3_32, 32, 0, 0 }, // DUP
  { "fmul",  0x0007002f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "imul",  0x0007002f, 0x007f007f, OP_REG_3_32, 32, 0, 0 }, // DUP
  { "fmadd", 0x0007003f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "imadd", 0x0007003f, 0x007f007f, OP_REG_3_32, 32, 0, 0 }, // DUP
  { "fmsub", 0x0007004f, 0x007f007f, OP_REG_3_32, 32, 0, 0 },
  { "imsub", 0x0007004f, 0x007f007f, OP_REG_3_32, 32, 0, 0 }, // DUP
  { "float", 0x0007005f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "fix",   0x0007006f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "fabs",  0x0007007f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },

  { "moveq",   0x0002, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movne",   0x0012, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movgtu",  0x0022, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movgteu", 0x0032, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movlteu", 0x0042, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movltu",  0x0052, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movgt",   0x0062, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movgte",  0x0072, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movlt",   0x0082, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movlte",  0x0092, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movbeq",  0x00a2, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movbne",  0x00b2, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movblt",  0x00c2, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movblte", 0x00d2, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "mov",     0x00e2, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  { "movl",    0x00f2, 0x03ff, OP_REG_2_16, 16, 0, 0 },

  { "moveq",   0x0002000f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movne",   0x0002001f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movgtu",  0x0002002f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movgteu", 0x0002003f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movlteu", 0x0002004f, 0x030f03ff, OP_REG_2_32, 32, 0, 0 },
  { "movltu",  0x0002005f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movgt",   0x0002006f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movgte",  0x0002007f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movlt",   0x0002008f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movlte",  0x0002009f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movbeq",  0x000200af, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movbne",  0x000200bf, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movblt",  0x000200cf, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movblte", 0x000200df, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "mov",     0x000200ef, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  { "movl",    0x000200ff, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },

  // dafuq?
  //{ "movts",    0x0102, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  //{ "movfs",    0x0112, 0x03ff, OP_REG_2_16, 16, 0, 0 },
  //{ "movts",    0x0002010f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },
  //{ "movfs",    0x0002011f, 0x03ff03ff, OP_REG_2_32, 32, 0, 0 },

  { "jr",    0x0142, 0xe3ff, OP_REG_1_16, 16, 0, 0 },
  { "jalr",  0x0152, 0xe3ff, OP_REG_1_16, 16, 0, 0 },
  { "jr",    0x00020142, 0xe000e3ff, OP_REG_1_32, 32, 0, 0 },
  { "jalr",  0x00020152, 0xe3ffe3ff, OP_REG_1_32, 32, 0, 0 },

  { "gie",    0x0192, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "gid",    0x0392, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "nop",    0x01a2, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "idle",   0x01b2, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "bkpt",   0x01c2, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "mbkpt",  0x03c2, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "sync",   0x01f2, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "rti",    0x01d2, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "wand",   0x0182, 0xffff, OP_NONE_16, 16, 0, 0 },
  { "trap",   0x03e2, 0x03ff, OP_NUM_16, 16, 0, 0 },
  { "unimpl",  0x000f000f, 0xffffffff, OP_NONE_32, 16, 0, 0 },
  { NULL, 0 }
};


