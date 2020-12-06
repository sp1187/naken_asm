/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2020 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "asm/common.h"
#include "asm/propeller.h"
#include "common/assembler.h"
#include "common/tokens.h"
#include "common/eval_expression.h"
#include "table/propeller2.h"

enum
{
  OPERAND_NUMBER,
  OPERAND_IMMEDIATE,
  OPERAND_ABSOLUTE_ADDRESS,
};

struct _operand
{
  int type;
  int value;
  int index;
};

union _flags
{
  uint8_t has_flag;

  struct
  {
    uint8_t wc    : 1;
    uint8_t wz    : 1;
    uint8_t wcz   : 1;
    uint8_t logic : 2;
  };
};

struct _conditions
{
  const char *name;
  uint8_t value;
};

static struct _conditions conditions[] =
{
  { "_ret_",        0x0 },
  { "if_nc_and_nz", 0x1 },
  { "if_nz_and_nc", 0x1 },
  { "if_a",         0x1 },
  { "if_00",        0x1 },
  { "if_nc_and_z",  0x2 },
  { "if_z_and_nc",  0x2 },
  { "if_01",        0x2 },
  { "if_nc",        0x3 },
  { "if_ae",        0x3 },
  { "if_0x",        0x3 },
  { "if_c_and_nz",  0x4 },
  { "if_nz_and_c",  0x4 },
  { "if_10",        0x4 },
  { "if_nz",        0x5 },
  { "if_ne",        0x5 },
  { "if_x0",        0x5 },
  { "if_c_ne_z",    0x6 },
  { "if_z_ne_c",    0x6 },
  { "if_diff",      0x6 },
  { "if_nc_or_nz",  0x7 },
  { "if_nz_or_nc",  0x7 },
  { "if_not_11",    0x7 },
  { "if_c_and_z",   0x8 },
  { "if_z_and_c",   0x8 },
  { "if_11",        0x8 },
  { "if_c_eq_z",    0x9 },
  { "if_z_eq_c",    0x9 },
  { "if_same",      0x9 },
  { "if_z",         0xa },
  { "if_e",         0xa },
  { "if_x1",        0xa },
  { "if_nc_or_z",   0xb },
  { "if_z_or_nc",   0xb },
  { "if_not_10",    0xb },
  { "if_c",         0xc },
  { "if_b",         0xc },
  { "if_1x",        0xc },
  { "if_c_or_nz",   0xd },
  { "if_nz_or_c",   0xd },
  { "if_not_01",    0xd },
  { "if_c_or_z",    0xe },
  { "if_z_or_c",    0xe },
  { "if_be",        0xe },
  { "if_not_00",    0xe },
};

int lookup_flag(const char *token, union _flags *flags)
{
  if (strcasecmp(token, "wc") == 0)
  {
    flags->wc = 1;
  }
    else
  if (strcasecmp(token, "wz") == 0)
  {
    flags->wz = 1;
  }
    else
  if (strcasecmp(token, "wcz") == 0)
  {
    flags->wcz = 1;
  }
    else
  if (strcasecmp(token, "andc") == 0)
  {
    flags->wc = 1;
    flags->logic = LOGIC_AND;
  }
    else
  if (strcasecmp(token, "andz") == 0)
  {
    flags->wz = 1;
    flags->logic = LOGIC_AND;
  }
    else
  if (strcasecmp(token, "orc") == 0)
  {
    flags->wc = 1;
    flags->logic = LOGIC_OR;
  }
    else
  if (strcasecmp(token, "orz") == 0)
  {
    flags->wz = 1;
    flags->logic = LOGIC_OR;
  }
    else
  if (strcasecmp(token, "xorc") == 0)
  {
    flags->wc = 1;
    flags->logic = LOGIC_XOR;
  }
    else
  if (strcasecmp(token, "xorz") == 0)
  {
    flags->wz = 1;
    flags->logic = LOGIC_XOR;
  }

  return flags->has_flag;
}

int parse_instruction_propeller2(struct _asm_context *asm_context, char *instr)
{
  char token[TOKENLEN];
  int token_type;
  char instr_case[TOKENLEN];
  struct _operand operands[3];
  int operand_count;
  uint32_t opcode;
  int n, i;
  int8_t cond = 0xf;
  union _flags flags;
  int found = 0;

  lower_copy(instr_case, instr);

  memset(operands, 0, sizeof(operands));
  operand_count = 0;

  const int len = sizeof(conditions) / sizeof(struct _conditions);

  for (n = 0; n < len; n++)
  {
    if (strcmp(instr_case, conditions[n].name) == 0)
    {
      token_type = tokens_get(asm_context, instr, TOKENLEN);
      lower_copy(instr_case, instr);

      cond = conditions[n].value;
      break;
    }
  }

  while (1)
  {
    token_type = tokens_get(asm_context, token, TOKENLEN);

    if (token_type == TOKEN_EOL || token_type == TOKEN_EOF)
    {
      break;
    }

    if (flags.has_flag != 0)
    {
      print_error_unexp(token, asm_context);
      return -1;
    }

    if (lookup_flag(token, &flags) != 0) { continue; }

    if (token_type == TOKEN_POUND)
    {
      operands[operand_count].type = OPERAND_IMMEDIATE;

      token_type = tokens_get(asm_context, token, TOKENLEN);

      if (IS_TOKEN(token, '\\'))
      {
        operands[operand_count].type = OPERAND_ABSOLUTE_ADDRESS;
      }
        else
      {
        tokens_push(asm_context, token, token_type);
      }
    }
      else
    {
      operands[operand_count].type = OPERAND_NUMBER;
      tokens_push(asm_context, token, token_type);
    }

    if (eval_expression(asm_context, &operands[operand_count].value) != 0)
    {
      if (asm_context->pass == 2)
      {
        print_error_illegal_expression(instr, asm_context);
        return -1;
      }

      eat_operand(asm_context);
      operands[operand_count].value = 0;
    }

    operand_count++;

    token_type = tokens_get(asm_context, token, TOKENLEN);

    if (token_type == TOKEN_EOL || token_type == TOKEN_EOF)
    {
      break;
    }

    if (IS_NOT_TOKEN(token,','))
    {
      print_error_unexp(token, asm_context);
      return -1;
    }
  }

  for (n = 0; table_propeller2[n].instr != NULL; n++)
  {
    if (strcmp(instr_case, table_propeller2[n].instr) == 0)
    {
      found = 1;

      if (operand_count != table_propeller2[n].operand_count) { continue; }
      if (flags.logic != table_propeller2[n].logic) { continue; }

      if ((flags.wc == 1 && table_propeller2[n].wc == 0) ||
          (flags.wz == 1 && table_propeller2[n].wz == 0) ||
          (flags.wcz == 1 && table_propeller2[n].wcz == 0))
      {
        continue;
      }

      opcode = table_propeller2[n].opcode;

      for (i = 0; i < operand_count; i++)
      {
        switch (table_propeller2[n].operands[i])
        {
          case OP_D:
            if (operands[i].type != OPERAND_NUMBER)
            {
              print_error_unknown_operand_combo(instr, asm_context);
              return -1;
            }

            if (check_range(asm_context, "Register", operands[i].value, 0, 511) == -1) { return -1; }
            opcode |= operands[i].value << 9;
            break;
          case OP_NUM_D:
            if (operands[i].type == OPERAND_NUMBER)
            {
              if (check_range(asm_context, "Register", operands[i].value, 0, 511) == -1) { return -1; }
            }
              else
            if (operands[i].type == OPERAND_IMMEDIATE)
            {
              if (check_range(asm_context, "Immediate", operands[i].value, -256, 511) == -1) { return -1; }
            }
              else
            {
              print_error_unknown_operand_combo(instr, asm_context);
              return -1;
            }

            opcode |= (operands[i].value & 0x1ff) << 9;
            break;
          case OP_NUM_S:
            if (operands[i].type == OPERAND_NUMBER)
            {
              if (check_range(asm_context, "Register", operands[i].value, 0, 511) == -1) { return -1; }
            }
              else
            if (operands[i].type == OPERAND_IMMEDIATE)
            {
              if (check_range(asm_context, "Immediate", operands[i].value, -256, 511) == -1) { return -1; }

              opcode |= 1 << 18;
            }
              else
            {
              print_error_unknown_operand_combo(instr, asm_context);
              return -1;
            }

            opcode |= operands[i].value & 0x1ff;
            break;
          case OP_NUM_SP:
          case OP_N_1:
          case OP_N_2:
          case OP_N_3:
          case OP_N_23:
          case OP_A:
          case OP_P:
          case OP_C:
          case OP_Z:
            break;
        }
      }

      if (i != operand_count) { continue; }

      if (flags.wz == 1)  { opcode |= 0x00080000; }
      if (flags.wc == 1)  { opcode |= 0x00100000; }
      if (flags.wcz == 1) { opcode |= 0x00180000; }

      if (operand_count != 0)
      {
        opcode |= cond << 28;
      }

      add_bin32(asm_context, opcode, IS_OPCODE);

      return 4;
    }
  }

  if (found == 1)
  {
    print_error_illegal_operands(instr, asm_context);
  }
    else
  {
    print_error_unknown_instr(instr, asm_context);
  }

  return -1;
}
