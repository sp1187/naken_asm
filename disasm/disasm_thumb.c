/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2013 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disasm_common.h"
#include "disasm_thumb.h"
#include "table_thumb.h"

#define READ_RAM(a) memory_read_m(memory, a)
#define READ_RAM16(a) (memory_read_m(memory, a))|(memory_read_m(memory, a+1)<<8)

int get_cycle_count_thumb(unsigned short int opcode)
{
  return -1;
}

int disasm_thumb(struct _memory *memory, int address, char *instruction, int *cycles_min, int *cycles_max)
{
uint16_t opcode;
int rd,rs,offset;
int n;

  *cycles_min=-1;
  *cycles_max=-1;

  opcode=READ_RAM16(address);

  n=0;
  while(table_thumb[n].instr!=NULL)
  {
    if (table_thumb[n].opcode==(opcode&table_thumb[n].mask))
    {
      switch(table_thumb[n].type)
      {
        case OP_SHIFT:
          rd=opcode&0x7;
          rs=(opcode>>3)&0x7;
          offset=(opcode>>6)&0x1f;
          sprintf(instruction, "%s r%d, r%d, #%d", table_thumb[n].instr, rd, rs, offset);
          return 2;
        default:
          strcpy(instruction, "???");
          return 2;
      }
    }
    n++;
  }

  strcpy(instruction, "???");

  return 2;
}

void list_output_thumb(struct _asm_context *asm_context, int address)
{
int cycles_min,cycles_max;
char instruction[128];
char bytes[10];
int count;
int n;
//unsigned int opcode=memory_read_m(&asm_context->memory, address);

  fprintf(asm_context->list, "\n");
  count=disasm_thumb(&asm_context->memory, address, instruction, &cycles_min, &cycles_max);

  bytes[0]=0;
  for (n=0; n<count; n++)
  {
    char temp[4];
    sprintf(temp, "%02x ", memory_read_m(&asm_context->memory, address+n));
    strcat(bytes, temp);
  }

  fprintf(asm_context->list, "0x%04x: %-9s %-40s cycles: ", address, bytes, instruction);

  if (cycles_min==cycles_max)
  { fprintf(asm_context->list, "%d\n", cycles_min); }
    else
  { fprintf(asm_context->list, "%d-%d\n", cycles_min, cycles_max); }
}

void disasm_range_thumb(struct _memory *memory, int start, int end)
{
char instruction[128];
char bytes[10];
int cycles_min=0,cycles_max=0;
int count;
int n;

  printf("\n");

  printf("%-7s %-5s %-40s Cycles\n", "Addr", "Opcode", "Instruction");
  printf("------- ------ ----------------------------------       ------\n");

  while(start<=end)
  {
    count=disasm_thumb(memory, start, instruction, &cycles_min, &cycles_max);

    bytes[0]=0;
    for (n=0; n<count; n++)
    {
      char temp[4];
      sprintf(temp, "%02x ", READ_RAM(start+n));
      strcat(bytes, temp);
    }

    if (cycles_min<1)
    {
      printf("0x%04x: %-9s %-40s ?\n", start, bytes, instruction);
    }
      else
    if (cycles_min==cycles_max)
    {
      printf("0x%04x: %-9s %-40s %d\n", start, bytes, instruction, cycles_min);
    }
      else
    {
      printf("0x%04x: %-9s %-40s %d-%d\n", start, bytes, instruction, cycles_min, cycles_max);
    }

    start=start+count;
  }
}

