/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2012 by Michael Kohn
 *
 * 65xx file by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "asm_65xx.h"
#include "disasm_65xx.h"
#include "simulate_65xx.h"

#include "table_65xx.h"
extern struct _cycles_65xx cycles_65xx[];

#define READ_RAM(a) memory_read_m(simulate->memory, a)
#define WRITE_RAM(a, b) memory_write_m(simulate->memory, a, b)

// status register flags
#define flag_c 0
#define flag_z 1
#define flag_i 2
#define flag_d 3
#define flag_b 4
#define flag_g 5
#define flag_v 6
#define flag_n 7

#define READ_BIT(dst, a) ((dst & (1 << a)) ? 1 : 0)
#define SET_BIT(dst, a) (dst |= (1 << a))
#define CLEAR_BIT(dst, a) (dst &= ~(1 << a))

#define READ_FLAG(a) ((reg_sr & (1 << a)) ? 1 : 0)
#define SET_FLAG(a) (reg_sr |= (1 << a))
#define CLEAR_FLAG(a) (reg_sr &= ~(1 << a))

#define FLAG(condition, flag) if(condition) SET_FLAG(flag); else CLEAR_FLAG(flag)

//FIXME these go in the _simulate_65xx structure
int reg_a, reg_x, reg_y, reg_sr, reg_pc, reg_sp;

// return calculated address for each mode
static inline int calc_address(struct _simulate *simulate, int address, int mode)
{
  switch(mode)
  {
    case 0:
      return READ_RAM(address) + 256 * READ_RAM(address + 1);
    case 1:
      return ((READ_RAM(address) + 256 * READ_RAM(address + 1)) + reg_x) & 0xFFFF;
    case 2:
      return ((READ_RAM(address) + 256 * READ_RAM(address + 1)) + reg_y) & 0xFFFF;
    case 3:
      return address;
    case 4:
      return address;
    case 5:
      return READ_RAM(address) + 256 * (READ_RAM((address + 1) & 0xFFFF));
    case 6:
      return READ_RAM(address + reg_x) & 0xFF;
    case 7:
      return (READ_RAM(address) + reg_y) & 0xFFFF;
    case 8:
      return (address + (READ_RAM(address) - 128) + 1) & 0xFFFF;
    case 9:
      return READ_RAM(address) & 0xFF;
    case 10:
      return (READ_RAM(address) + reg_x) & 0xFF;
    case 11:
      return (READ_RAM(READ_RAM(address)) + 256 * ((READ_RAM(READ_RAM(address) + 1)) & 0xFF) + reg_y) & 0xFF;
    default:
      return -1;
  }
}

static int operand_exe(struct _simulate *simulate, int opcode)
{
  int mode = cycles_65xx[opcode].mode;
  if(mode == -1)
    return -1;

  int address = calc_address(simulate, reg_pc + 1, mode);

  int m = READ_RAM(address);
  int temp;
  int pc_lo, pc_hi;
  int temp_a = reg_a;

  switch(opcode)
  {
    // ADC
    case 0x61:
    case 0x65:
    case 0x69:
    case 0x6D:
    case 0x71:
    case 0x75:
    case 0x79:
    case 0x7D:
      if(READ_FLAG(flag_d))
      {
        int bcd_a = (reg_a & 15) + 10 * (reg_a >> 4);
        int bcd_m = (m & 15) + 10 * (m >> 4);
        int result = bcd_a + bcd_m + READ_FLAG(flag_c);

        FLAG(result > 99, flag_c);
        result %= 100;

        reg_a = (result % 10) + ((result / 10) << 4);
      }
      else
      {
        reg_a += m + READ_FLAG(flag_c);

        FLAG(reg_a > 255, flag_c);
      }

      reg_a &= 0xFF;
      FLAG((temp_a ^ reg_a) & (m ^ reg_a) & 0x80, flag_v);
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);

      break;
    // AND
    case 0x21:
    case 0x25:
    case 0x29:
    case 0x2D:
    case 0x31:
    case 0x35:
    case 0x39:
    case 0x3D:
      reg_a &= m;
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);
      break;
    // ASL
    case 0x06:
    case 0x0A:
    case 0x0E:
    case 0x16:
    case 0x1E:
      if(mode == 4)
      {
        FLAG(READ_BIT(reg_a, 7), flag_c);
        reg_a <<= 1;
        reg_a &= 0xFF;
        FLAG(reg_a > 127, flag_n);
        FLAG(reg_a == 0, flag_z);
      }
      else
      {
        FLAG(READ_BIT(m, 7), flag_c);
        m <<= 1;
        m &= 0xFF;
        FLAG(m > 127, flag_n);
        FLAG(m == 0, flag_z);
        WRITE_RAM(address, m);
      }
      break;
    // BCC
    case 0x90:
      if(READ_FLAG(flag_c) == 0)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BCS
    case 0xB0:
      if(READ_FLAG(flag_c) == 1)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BEQ
    case 0xF0:
      if(READ_FLAG(flag_z) == 1)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BMI
    case 0x30:
      if(READ_FLAG(flag_n) == 1)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BNE
    case 0xD0:
      if(READ_FLAG(flag_z) == 0)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BPL
    case 0x10:
      if(READ_FLAG(flag_n) == 0)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BVC
    case 0x50:
      if(READ_FLAG(flag_v) == 0)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BVS
    case 0x70:
      if(READ_FLAG(flag_v) == 1)
      {
        reg_pc = address;
        return 1;
      }
      break;
    // BRK
    case 0x00:
      // stop simulation
      return -1;
    // BIT
    case 0x24:
    case 0x2C:
      FLAG((reg_a & m) == 0, flag_z);
      FLAG(READ_BIT(m, 6), flag_v);
      FLAG(READ_BIT(m, 7), flag_n);
      break;
    // CLC
    case 0x18:
      CLEAR_FLAG(flag_c);
      break;
    // CLD
    case 0xD8:
      CLEAR_FLAG(flag_d);
      break;
    // CLI
    case 0x58:
      CLEAR_FLAG(flag_i);
      break;
    // CLV
    case 0xB8:
      CLEAR_FLAG(flag_v);
      break;
    // CMP
    case 0xC1:
    case 0xC5:
    case 0xC9:
    case 0xCD:
    case 0xD1:
    case 0xD5:
    case 0xD9:
    case 0xDD:
      temp = (reg_a - m);
      FLAG(temp >= 0, flag_c);
      temp &= 0xFF;
      FLAG(temp > 127, flag_n);
      FLAG(temp == 0, flag_z);
      break;
    // CPX
    case 0xE0:
    case 0xE4:
    case 0xEC:
      temp = (reg_x - m);
      FLAG(temp >= 0, flag_c);
      temp &= 0xFF;
      FLAG(temp > 127, flag_n);
      FLAG(temp == 0, flag_z);
      break;
    // CPY
    case 0xC0:
    case 0xC4:
    case 0xCC:
      temp = (reg_y - m);
      FLAG(temp >= 0, flag_c);
      temp &= 0xFF;
      FLAG(temp > 127, flag_n);
      FLAG(temp == 0, flag_z);
      break;
    // DEC
    case 0xC6:
    case 0xCE:
    case 0xD6:
    case 0xDE:
      temp = (m - 1) & 0xFF;
      WRITE_RAM(address, temp);
      FLAG(temp > 127, flag_n);
      FLAG(temp == 0, flag_z);
      break;
    // DEX
    case 0xCA:
      reg_x = (reg_x - 1) & 0xFF;
      FLAG(reg_x > 127, flag_n);
      FLAG(reg_x == 0, flag_z);
      break;
    // DEY
    case 0x88:
      reg_y = (reg_y - 1) & 0xFF;
      FLAG(reg_y > 127, flag_n);
      FLAG(reg_y == 0, flag_z);
      break;
    // EOR
    case 0x41:
    case 0x45:
    case 0x49:
    case 0x4D:
    case 0x51:
    case 0x55:
    case 0x59:
    case 0x5D:
      reg_a ^= m;
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);
      break;
    // INC
    case 0xE6:
    case 0xEE:
    case 0xF6:
    case 0xFE:
      temp = (m + 1) & 0xFF;
      WRITE_RAM(address, temp);
      FLAG(temp > 127, flag_n);
      FLAG(temp == 0, flag_z);
      break;
    // INX
    case 0xE8:
      reg_x = (reg_x + 1) & 0xFF;
      FLAG(reg_x > 127, flag_n);
      FLAG(reg_x == 0, flag_z);
      break;
    // INY
    case 0xC8:
      reg_y = (reg_y + 1) & 0xFF;
      FLAG(reg_x > 127, flag_n);
      FLAG(reg_x == 0, flag_z);
      break;
    // JMP
    case 0x4C:
    case 0x6C:
      reg_pc = address;
      return 1;
    // JSR
    case 0x20:
      WRITE_RAM(0x100 + reg_sp, (reg_pc + 2) / 256);
      reg_sp--;
      WRITE_RAM(0x100 + reg_sp, (reg_pc + 2) & 0xFF);
      reg_sp--;
      reg_pc = address;
      return 1;
    // LDA
    case 0xA1:
    case 0xA5:
    case 0xA9:
    case 0xAD:
    case 0xB1:
    case 0xB5:
    case 0xB9:
    case 0xBD:
      reg_a = m;
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);
      break; 
    // LDX
    case 0xA2:
    case 0xA6:
    case 0xAE:
    case 0xB6:
    case 0xBE:
      reg_x = m;
      FLAG(reg_x == 0, flag_z);
      FLAG(m > 127, flag_n);
      break;
    // LDY
    case 0xA0:
    case 0xA4:
    case 0xAC:
    case 0xB4:
    case 0xBC:
      reg_y = m;
      FLAG(reg_y == 0, flag_z);
      FLAG(m > 127, flag_n);
      break;
    // LSR
    case 0x46:
    case 0x4A:
    case 0x4E:
    case 0x56:
    case 0x5E:
      if(mode == 4)
      {
        FLAG(READ_BIT(reg_a, 0), flag_c);
        reg_a >>= 1;
        CLEAR_BIT(reg_a, 7);
        FLAG(reg_a > 127, flag_n);
        FLAG(reg_a == 0, flag_z);
      }
      else
      {
        FLAG(READ_BIT(m, 0), flag_c);
        m >>= 1;
        CLEAR_BIT(m, 7);
        FLAG(m > 127, flag_n);
        FLAG(m == 0, flag_z);
        WRITE_RAM(address, m);
      }
      break;
    // NOP
    case 0xEA:
      break;
    // ORA
    case 0x01:
    case 0x05:
    case 0x09:
    case 0x0D:
    case 0x11:
    case 0x15:
    case 0x19:
    case 0x1D:
      reg_a |= m;
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);
      break;
    // PHA
    case 0x48:
      WRITE_RAM(0x100 + reg_sp, reg_a);
      reg_sp--;
      break;
    // PHP
    case 0x08:
      WRITE_RAM(0x100 + reg_sp, reg_sr);
      reg_sp--;
      break;
    // PLA
    case 0x68:
      reg_sp++;
      reg_a = READ_RAM(0x100 + reg_sp);
      break;
    // PLP
    case 0x28:
      reg_sp++;
      reg_sr = READ_RAM(0x100 + reg_sp);
      break;
    // ROL
    case 0x26:
    case 0x2A:
    case 0x2E:
    case 0x36:
    case 0x3E:
      if(mode == 4)
      {
        FLAG(READ_BIT(reg_a, 7), flag_c);
        reg_a <<= 1;
        SET_BIT(reg_a, READ_FLAG(flag_c));
        reg_a &= 0xFF;
        FLAG(reg_a > 127, flag_n);
        FLAG(reg_a == 0, flag_z);
      }
      else
      {
        FLAG(READ_BIT(m, 7), flag_c);
        m <<= 1;
        SET_BIT(m, READ_FLAG(flag_c));
        m &= 0xFF;
        FLAG(m > 127, flag_n);
        FLAG(m == 0, flag_z);
        WRITE_RAM(address, m);
      }
      break;
    // ROR
    case 0x66:
    case 0x6A:
    case 0x6E:
    case 0x76:
    case 0x7E:
      if(mode == 4)
      {
        temp = READ_BIT(reg_a, 0);
        reg_a >>= 1;
        SET_BIT(reg_a, READ_FLAG(flag_c) << 7);
        FLAG(temp, flag_c);
        FLAG(reg_a > 127, flag_n);
        FLAG(reg_a == 0, flag_z);
      }
      else
      {
        temp = READ_BIT(m, 0);
        m >>= 1;
        SET_BIT(m, READ_FLAG(flag_c) << 7);
        FLAG(temp, flag_c);
        FLAG(m > 127, flag_n);
        FLAG(m == 0, flag_z);
        WRITE_RAM(address, m);
      }
      break;
    // RTI
    case 0x40:
      reg_sp++;
      reg_sr = READ_RAM(0x100 + reg_sp);
      reg_sp++;
      pc_lo = READ_RAM(0x100 + reg_sp);
      reg_sp++;
      pc_hi = READ_RAM(0x100 + reg_sp);
      reg_pc = (pc_lo + 256 * pc_hi);
      reg_pc++;
      return 1;
    // RTS
    case 0x60:
      reg_sp++;
      pc_lo = READ_RAM(0x100 + reg_sp);
      reg_sp++;
      pc_hi = READ_RAM(0x100 + reg_sp);
      reg_pc = (pc_lo + 256 * pc_hi);
      reg_pc++;
      return 1;
    // SBC
    case 0xE1:
    case 0xE5:
    case 0xE9:
    case 0xED:
    case 0xF1:
    case 0xF5:
    case 0xF9:
    case 0xFD:
      if(READ_FLAG(flag_d))
      {
        int bcd_a = (reg_a & 15) + 10 * (reg_a >> 4);
        int bcd_m = (m & 15) + 10 * (m >> 4);
        int result = bcd_a - bcd_m - (1 - READ_FLAG(flag_c));

        // clear carry if < 0
        FLAG(result >= 0, flag_c);
        result %= 100;

        reg_a = (result % 10) + ((result / 10) << 4);
      }
      else
      {
        reg_a -= m - (1 - READ_FLAG(flag_c));

        FLAG(reg_a >= 0, flag_c);
      }

      reg_a &= 0xFF;
      FLAG((temp_a ^ reg_a) & (m ^ reg_a) & 0x80, flag_v);
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);

      break;
    // SEC
    case 0x38:
      SET_FLAG(flag_c);
      break;
    // SED
    case 0xF8:
      SET_FLAG(flag_d);
      break;
    // SEI
    case 0x78:
      SET_FLAG(flag_i);
      break;
    // STA
    case 0x81:
    case 0x85:
    case 0x8D:
    case 0x91:
    case 0x95:
    case 0x99:
    case 0x9D:
      WRITE_RAM(address, reg_a);
      break;
    // STX
    case 0x86:
    case 0x8E:
    case 0x96:
      WRITE_RAM(address, reg_x);
      break;
    // STY
    case 0x84:
    case 0x8C:
    case 0x94:
      WRITE_RAM(address, reg_y);
      break;
    // TAX
    case 0xAA:
      reg_x = reg_a;
      FLAG(reg_x == 0, flag_z);
      FLAG(m > 127, flag_n);
      break;
    // TAY
    case 0xA8:
      reg_y = reg_a;
      FLAG(reg_y == 0, flag_z);
      FLAG(m > 127, flag_n);
      break;
    // TSX
    case 0xBA:
      reg_x = reg_sp;
      FLAG(reg_x == 0, flag_z);
      FLAG(m > 127, flag_n);
      break;
    // TXA
    case 0x8A:
      reg_a = reg_x;
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);
      break;
    // TXS
    case 0x9A:
      reg_sp = reg_x;
      break;
    // TYA
    case 0x98:
      reg_a = reg_y;
      FLAG(reg_a > 127, flag_n);
      FLAG(reg_a == 0, flag_z);
      break;
  }

  return 0;
}

static int stop_running=0;

// 65xx registers
int reg_a, reg_x, reg_y, reg_sr, reg_pc, reg_sp;

static void handle_signal(int sig)
{
  stop_running=1;
  signal(SIGINT, SIG_DFL);
}

struct _simulate *simulate_init_65xx(struct _memory *memory)
{
struct _simulate *simulate;

  simulate=(struct _simulate *)malloc(sizeof(struct _simulate_65xx)+sizeof(struct _simulate));

  simulate->simulate_init=simulate_init_65xx;
  simulate->simulate_free=simulate_free_65xx;
  simulate->simulate_push=simulate_push_65xx;
  simulate->simulate_set_reg=simulate_set_reg_65xx;
  simulate->simulate_get_reg=simulate_get_reg_65xx;
  simulate->simulate_reset=simulate_reset_65xx;
  simulate->simulate_dump_registers=simulate_dump_registers_65xx;
  simulate->simulate_run=simulate_run_65xx;

  //memory_init(&simulate->memory, 65536, 0);
  simulate_reset_65xx(simulate);
  simulate->usec=1000000; // 1Hz
  simulate->show=1; // Show simulation
  simulate->step_mode=0;
  simulate->memory=memory;

  reg_a = 0;
  reg_x = 0;
  reg_y = 0;
  reg_sr = 0;
  reg_pc = 0;
  reg_sp = 0xFF;

  return simulate;
}

void simulate_push_65xx(struct _simulate *simulate, unsigned int value)
{
//struct _simulate_65xx *simulate_65xx=(struct _simulate_65xx *)simulate->context;

  WRITE_RAM(0x100 + reg_sp, value & 0xFF);
  reg_sp--;
}

int simulate_set_reg_65xx(struct _simulate *simulate, char *reg_string, unsigned int value)
{
//struct _simulate_65xx *simulate_65xx=(struct _simulate_65xx *)simulate->context;

  // a, x, y, sr, pc, sp

  while(*reg_string==' ') { reg_string++; }

  char *pos = reg_string;

  if(pos[0] == 'a' || pos[0] == 'A')
    reg_a = value & 0xFF;
  else if(pos[0] == 'x' || pos[0] == 'X')
    reg_x = value & 0xFF;
  else if(pos[0] == 'y' || pos[0] == 'Y')
    reg_y = value & 0xFF;
  else if((pos[0] == 's' || pos[0] == 'S') && (pos[1] == 'r' || pos[1] == 'R'))
    reg_sr = value & 0xFF;
  else if((pos[0] == 'p' || pos[0] == 'P') && (pos[1] == 'c' || pos[1] == 'C'))
    reg_pc = value & 0xFFFF;
  else if((pos[0] == 's' || pos[0] == 'S') && (pos[1] == 'p' || pos[1] == 'P'))
    reg_sp = value & 0xFF;
  else
    return -1;

  return 0;
}

unsigned int simulate_get_reg_65xx(struct _simulate *simulate, char *reg_string)
{
//struct _simulate_65xx *simulate_65xx=(struct _simulate_65xx *)simulate->context;

  while(*reg_string==' ') { reg_string++; }

  char *pos = reg_string;

  if(pos[0] == 'a' || pos[0] == 'A')
    return reg_a;
  if(pos[0] == 'x' || pos[0] == 'X')
    return reg_x;
  if(pos[0] == 'y' || pos[0] == 'Y')
    return reg_y;
  if((pos[0] == 's' || pos[0] == 'S') && (pos[1] == 'r' || pos[1] == 'R'))
    return reg_sr;
  if((pos[0] == 'p' || pos[0] == 'P') && (pos[1] == 'c' || pos[1] == 'C'))
    return reg_pc;
  if((pos[0] == 's' || pos[0] == 'S') && (pos[1] == 'p' || pos[1] == 'P'))
    return reg_sp;

  return -1;
}

void simulate_reset_65xx(struct _simulate *simulate)
{
//struct _simulate_65xx *simulate_65xx=(struct _simulate_65xx *)simulate->context;

  simulate->cycle_count=0;
  simulate->ret_count=0;
  reg_a = 0;
  reg_x = 0;
  reg_y = 0;
  reg_sr = 0;
  reg_pc = 0;
  reg_sp = 0xFF;
  //memset(simulate_65xx->reg, 0, sizeof(simulate_65xx->reg));
  //memory_clear(simulate->memory);
  simulate->break_point=-1;
}

void simulate_free_65xx(struct _simulate *simulate)
{
  //memory_free(simulate->memory);
  free(simulate);
}

void simulate_dump_registers_65xx(struct _simulate *simulate)
{
//struct _simulate_65xx *simulate_65xx=(struct _simulate_65xx *)simulate->context;

  printf("\nSimulation Register Dump\n");
  printf("-------------------------------------------------------------------\n");
  printf(" A = %d, %02x\n", reg_a, reg_a);
  printf(" X = %d, %02x\n", reg_x, reg_x);
  printf(" Y = %d, %02x\n", reg_y, reg_y);
  printf("SR = %d, %02x\n", reg_sr, reg_sr);
  printf("PC = %d, %04x\n", reg_pc, reg_pc);
  printf("SP = %d, %02x\n", reg_sp, reg_sp);
  printf("\n");
  printf("n%c ", READ_FLAG(flag_n) ? '*' : ' ');
  printf("v%c ", READ_FLAG(flag_v) ? '*' : ' ');
  printf("-%c ", READ_FLAG(flag_g) ? '*' : ' ');
  printf("b%c ", READ_FLAG(flag_b) ? '*' : ' ');
  printf("d%c ", READ_FLAG(flag_d) ? '*' : ' ');
  printf("i%c ", READ_FLAG(flag_i) ? '*' : ' ');
  printf("z%c ", READ_FLAG(flag_z) ? '*' : ' ');
  printf("c%c ", READ_FLAG(flag_c) ? '*' : ' ');

  printf("\n\n");
  printf("%d clock cycles have passed since last reset.\n\n", simulate->cycle_count);
}

int simulate_run_65xx(struct _simulate *simulate, int max_cycles, int step)
{
//struct _simulate_65xx *simulate_65xx=(struct _simulate_65xx *)simulate->context;

char instruction[128];
 

  stop_running=0;
  signal(SIGINT, handle_signal);

  printf("Running... Press Ctl-C to break.\n");

  while(stop_running==0)
  {
    int pc = reg_pc;
    int cycles_min, cycles_max;
    int opcode = READ_RAM(pc);
    //int c = get_cycle_count_65xx(opcode);

    // draw stuffs
    //system("cls");
    printf("\x1b[1J\x1b[1;1H");
    simulate_dump_registers_65xx(simulate);

    int ret = operand_exe(simulate, opcode);

    // stop simulation on BRK instruction
    if(ret == -1)
      break;

    // only increment if reg_pc not touched
    if(ret == 0)
      reg_pc += disasm_65xx(simulate->memory, pc, instruction, &cycles_min, &cycles_max);

// if simulate->show = 1
    int n = 0;
    while(n < 6)
    {
      int count = disasm_65xx(simulate->memory, pc, instruction, &cycles_min, &cycles_max);

      if(cycles_min == -1) break;

      if(pc == simulate->break_point) { printf("*"); }
      else { printf(" "); }

      if(n == 0)
      { printf("! "); }
        else
      if(pc == reg_pc) { printf("> "); }
        else
      { printf("  "); }

      printf("0x%04x: %-40s %d-%d\n", pc, instruction, cycles_min, cycles_max);
      n += count;
      pc += count;

      count--;
      while(count > 0)
      {
        if(pc == simulate->break_point) { printf("*"); }
        else { printf(" "); }
        printf("  0x%04x: 0x%04x\n", pc, READ_RAM(pc));
        pc += count;
        count--;
      }
    }

    if(ret == -1)
    {  
      printf("Illegal instruction at address 0x%04x\n", pc);
      return -1;
    }

    if(simulate->break_point == reg_pc)
    {
      printf("Breakpoint hit at 0x%04x\n", simulate->break_point);
      break;
    }

    if((simulate->usec == 0) || (step == 1))
    {
      signal(SIGINT, SIG_DFL);
      return 0;
    }

    if(reg_pc == 0xFFFF)
    {
      printf("Function ended.  Total cycles: %d\n", simulate->cycle_count);
      simulate->step_mode = 0;
      reg_pc = READ_RAM(0xFFFC) + READ_RAM(0xFFFD) * 256;
      signal(SIGINT, SIG_DFL);
      return 0;
    }

    usleep(simulate->usec > 999999 ? 999999 : simulate->usec);
  }

  signal(SIGINT, SIG_DFL);
  printf("Stopped.  PC=0x%04x.\n", reg_pc);
  printf("%d clock cycles have passed since last reset.\n", simulate->cycle_count);

  return 0;
}


