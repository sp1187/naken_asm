/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2015 by Michael Kohn, Joe Davisson
 *
 * 65816 by Joe Davisson
 *
 */

#include "table/65816.h"

struct _table_65816 table_65816[] =
{
  { "adc", M65816_ADC, OP_NONE },
  { "and", M65816_AND, OP_NONE },
  { "asl", M65816_ASL, OP_NONE },
  { "bcc", M65816_BCC, OP_RELATIVE },
  { "bcs", M65816_BCS, OP_RELATIVE },
  { "beq", M65816_BEQ, OP_RELATIVE },
  { "bit", M65816_BIT, OP_NONE },
  { "bmi", M65816_BMI, OP_RELATIVE },
  { "bne", M65816_BNE, OP_RELATIVE },
  { "bpl", M65816_BPL, OP_RELATIVE },
  { "bra", M65816_BRA, OP_RELATIVE },
  { "brk", M65816_BRK, OP_NONE },
  { "brl", M65816_BRL, OP_RELATIVE_LONG },
  { "bvc", M65816_BVC, OP_RELATIVE },
  { "bvs", M65816_BVS, OP_RELATIVE },
  { "clc", M65816_CLC, OP_NONE },
  { "cld", M65816_CLD, OP_NONE },
  { "cli", M65816_CLI, OP_NONE },
  { "clv", M65816_CLV, OP_NONE },
  { "cmp", M65816_CMP, OP_NONE },
  { "cop", M65816_COP, OP_NONE },
  { "cpx", M65816_CPX, OP_NONE },
  { "cpy", M65816_CPY, OP_NONE },
  { "dec", M65816_DEC, OP_NONE },
  { "dex", M65816_DEX, OP_NONE },
  { "dey", M65816_DEY, OP_NONE },
  { "eor", M65816_EOR, OP_NONE },
  { "inc", M65816_INC, OP_NONE },
  { "inx", M65816_INX, OP_NONE },
  { "iny", M65816_INY, OP_NONE },
  { "jml", M65816_JML, OP_NONE },
  { "jmp", M65816_JMP, OP_NONE },
  { "jsl", M65816_JSL, OP_NONE },
  { "jsr", M65816_JSR, OP_NONE },
  { "lda", M65816_LDA, OP_NONE },
  { "ldx", M65816_LDX, OP_NONE },
  { "ldy", M65816_LDY, OP_NONE },
  { "lsr", M65816_LSR, OP_NONE },
  { "mvn", M65816_MVN, OP_BLOCK_MOVE },
  { "mvp", M65816_MVP, OP_BLOCK_MOVE },
  { "nop", M65816_NOP, OP_NONE },
  { "ora", M65816_ORA, OP_NONE },
  { "pea", M65816_PEA, OP_NONE },
  { "pei", M65816_PEI, OP_NONE },
  { "per", M65816_PER, OP_RELATIVE_LONG },
  { "pha", M65816_PHA, OP_NONE },
  { "phb", M65816_PHB, OP_NONE },
  { "phd", M65816_PHD, OP_NONE },
  { "phk", M65816_PHK, OP_NONE },
  { "php", M65816_PHP, OP_NONE },
  { "phx", M65816_PHX, OP_NONE },
  { "phy", M65816_PHY, OP_NONE },
  { "pla", M65816_PLA, OP_NONE },
  { "plb", M65816_PLB, OP_NONE },
  { "pld", M65816_PLD, OP_NONE },
  { "plp", M65816_PLP, OP_NONE },
  { "plx", M65816_PLX, OP_NONE },
  { "ply", M65816_PLY, OP_NONE },
  { "rep", M65816_REP, OP_IMMEDIATE8 },
  { "rol", M65816_ROL, OP_NONE },
  { "ror", M65816_ROR, OP_NONE },
  { "rti", M65816_RTI, OP_NONE },
  { "rtl", M65816_RTL, OP_NONE },
  { "rts", M65816_RTS, OP_NONE },
  { "sbc", M65816_SBC, OP_NONE },
  { "sep", M65816_SEP, OP_NONE },
  { "sec", M65816_SEC, OP_NONE },
  { "sed", M65816_SED, OP_NONE },
  { "sei", M65816_SEI, OP_NONE },
  { "sta", M65816_STA, OP_NONE },
  { "stp", M65816_STP, OP_NONE },
  { "stx", M65816_STX, OP_NONE },
  { "sty", M65816_STY, OP_NONE },
  { "stz", M65816_STZ, OP_NONE },
  { "tax", M65816_TAX, OP_NONE },
  { "tay", M65816_TAY, OP_NONE },
  { "tcd", M65816_TCD, OP_NONE },
  { "tcs", M65816_TCS, OP_NONE },
  { "tdc", M65816_TDC, OP_NONE },
  { "trb", M65816_TRB, OP_NONE },
  { "tsb", M65816_TSB, OP_NONE },
  { "tsc", M65816_TSC, OP_NONE },
  { "tsx", M65816_TSX, OP_NONE },
  { "txa", M65816_TXA, OP_NONE },
  { "txs", M65816_TXS, OP_NONE },
  { "txy", M65816_TXY, OP_NONE },
  { "tya", M65816_TYA, OP_NONE },
  { "tyx", M65816_TYX, OP_NONE },
  { "wai", M65816_WAI, OP_NONE },
  { "wdm", M65816_WDM, OP_NONE },
  { "xba", M65816_XBA, OP_NONE },
  { "xce", M65816_XCE, OP_NONE },
};

struct _table_65816_opcodes table_65816_opcodes[] =
{
  /* 0x00 */ { M65816_BRK, OP_NONE, 0, 0 },
  /* 0x01 */ { M65816_ORA, OP_X_INDIRECT8, 0, 0 },
  /* 0x02 */ { M65816_COP, OP_NONE, 0, 0 },
  /* 0x03 */ { M65816_ORA, OP_SP_RELATIVE, 0, 0 },
  /* 0x04 */ { M65816_TSB, OP_ADDRESS8, 0, 0 },
  /* 0x05 */ { M65816_ORA, OP_ADDRESS8, 0, 0 },
  /* 0x06 */ { M65816_ASL, OP_ADDRESS8, 0, 0 },
  /* 0x07 */ { M65816_ORA, OP_INDIRECT8_LONG, 0, 0 },
  /* 0x08 */ { M65816_PHP, OP_NONE, 0, 0 },
  /* 0x09 */ { M65816_ORA, OP_IMMEDIATE16, 0, 0 },
  /* 0x0A */ { M65816_ASL, OP_NONE, 0, 0 },
  /* 0x0B */ { M65816_PHD, OP_NONE, 0, 0 },
  /* 0x0C */ { M65816_TSB, OP_ADDRESS16, 0, 0 },
  /* 0x0D */ { M65816_ORA, OP_ADDRESS16, 0, 0 },
  /* 0x0E */ { M65816_ASL, OP_ADDRESS16, 0, 0 },
  /* 0x0F */ { M65816_ORA, OP_ADDRESS24, 0, 0 },

  /* 0x10 */ { M65816_BPL, OP_RELATIVE, 0, 0 },
  /* 0x11 */ { M65816_ORA, OP_INDIRECT8_Y, 0, 0 },
  /* 0x12 */ { M65816_ORA, OP_INDIRECT8, 0, 0 },
  /* 0x13 */ { M65816_ORA, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0x14 */ { M65816_TRB, OP_ADDRESS8, 0, 0 },
  /* 0x15 */ { M65816_ORA, OP_INDEXED8_X, 0, 0 },
  /* 0x16 */ { M65816_ASL, OP_INDEXED8_X, 0, 0 },
  /* 0x17 */ { M65816_ORA, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0x18 */ { M65816_CLC, OP_NONE, 0, 0 },
  /* 0x19 */ { M65816_ORA, OP_INDEXED16_Y, 0, 0 },
  /* 0x1A */ { M65816_INC, OP_NONE, 0, 0 },
  /* 0x1B */ { M65816_TCS, OP_NONE, 0, 0 },
  /* 0x1C */ { M65816_TRB, OP_ADDRESS16, 0, 0 },
  /* 0x1D */ { M65816_ORA, OP_INDEXED16_X, 0, 0 },
  /* 0x1E */ { M65816_ASL, OP_INDEXED16_X, 0, 0 },
  /* 0x1F */ { M65816_ORA, OP_INDEXED24_X, 0, 0 },

  /* 0x20 */ { M65816_JSR, OP_ADDRESS16, 0, 0 },
  /* 0x21 */ { M65816_AND, OP_X_INDIRECT8, 0, 0 },
  /* 0x22 */ { M65816_JSR, OP_ADDRESS24, 0, 0 },
  /* 0x23 */ { M65816_AND, OP_SP_RELATIVE, 0, 0 },
  /* 0x24 */ { M65816_BIT, OP_ADDRESS8, 0, 0 },
  /* 0x25 */ { M65816_AND, OP_ADDRESS8, 0, 0 },
  /* 0x26 */ { M65816_ROL, OP_ADDRESS8, 0, 0 },
  /* 0x27 */ { M65816_AND, OP_INDIRECT8_LONG, 0, 0 },
  /* 0x28 */ { M65816_PLP, OP_NONE, 0, 0 },
  /* 0x29 */ { M65816_AND, OP_IMMEDIATE16, 0, 0 },
  /* 0x2A */ { M65816_ROL, OP_NONE, 0, 0 },
  /* 0x2B */ { M65816_PLD, OP_NONE, 0, 0 },
  /* 0x2C */ { M65816_BIT, OP_ADDRESS16, 0, 0 },
  /* 0x2D */ { M65816_AND, OP_ADDRESS16, 0, 0 },
  /* 0x2E */ { M65816_ROL, OP_ADDRESS16, 0, 0 },
  /* 0x2F */ { M65816_AND, OP_ADDRESS24, 0, 0 },

  /* 0x30 */ { M65816_BMI, OP_RELATIVE, 0, 0 },
  /* 0x31 */ { M65816_AND, OP_INDIRECT8_Y, 0, 0 },
  /* 0x32 */ { M65816_AND, OP_INDIRECT8, 0, 0 },
  /* 0x33 */ { M65816_AND, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0x34 */ { M65816_BIT, OP_INDEXED8_X, 0, 0 },
  /* 0x35 */ { M65816_AND, OP_INDEXED8_X, 0, 0 },
  /* 0x36 */ { M65816_ROL, OP_INDEXED8_X, 0, 0 },
  /* 0x37 */ { M65816_AND, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0x38 */ { M65816_SEC, OP_NONE, 0, 0 },
  /* 0x39 */ { M65816_AND, OP_INDEXED16_Y, 0, 0 },
  /* 0x3A */ { M65816_DEC, OP_NONE, 0, 0 },
  /* 0x3B */ { M65816_TSC, OP_NONE, 0, 0 },
  /* 0x3C */ { M65816_BIT, OP_INDEXED16_X, 0, 0 },
  /* 0x3D */ { M65816_AND, OP_INDEXED16_X, 0, 0 },
  /* 0x3E */ { M65816_ROL, OP_INDEXED16_X, 0, 0 },
  /* 0x3F */ { M65816_AND, OP_INDEXED24_X, 0, 0 },

  /* 0x40 */ { M65816_RTI, OP_NONE, 0, 0 },
  /* 0x41 */ { M65816_EOR, OP_X_INDIRECT8, 0, 0 },
  /* 0x42 */ { M65816_WDM, OP_NONE, 0, 0 },
  /* 0x43 */ { M65816_EOR, OP_SP_RELATIVE, 0, 0 },
  /* 0x44 */ { M65816_MVP, OP_BLOCK_MOVE, 0, 0 },
  /* 0x45 */ { M65816_EOR, OP_ADDRESS8, 0, 0 },
  /* 0x46 */ { M65816_LSR, OP_ADDRESS8, 0, 0 },
  /* 0x47 */ { M65816_EOR, OP_INDIRECT8_LONG, 0, 0 },
  /* 0x48 */ { M65816_PHA, OP_NONE, 0, 0 },
  /* 0x49 */ { M65816_EOR, OP_IMMEDIATE16, 0, 0 },
  /* 0x4A */ { M65816_LSR, OP_NONE, 0, 0 },
  /* 0x4B */ { M65816_PHK, OP_NONE, 0, 0 },
  /* 0x4C */ { M65816_JMP, OP_ADDRESS16, 0, 0 },
  /* 0x4D */ { M65816_EOR, OP_ADDRESS16, 0, 0 },
  /* 0x4E */ { M65816_LSR, OP_ADDRESS16, 0, 0 },
  /* 0x4F */ { M65816_EOR, OP_ADDRESS24, 0, 0 },

  /* 0x50 */ { M65816_BVC, OP_RELATIVE, 0, 0 },
  /* 0x51 */ { M65816_EOR, OP_INDIRECT8_Y, 0, 0 },
  /* 0x52 */ { M65816_EOR, OP_INDIRECT8, 0, 0 },
  /* 0x53 */ { M65816_EOR, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0x54 */ { M65816_MVN, OP_BLOCK_MOVE, 0, 0 },
  /* 0x55 */ { M65816_EOR, OP_INDEXED8_X, 0, 0 },
  /* 0x56 */ { M65816_LSR, OP_INDEXED8_X, 0, 0 },
  /* 0x57 */ { M65816_EOR, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0x58 */ { M65816_CLI, OP_NONE, 0, 0 },
  /* 0x59 */ { M65816_EOR, OP_INDEXED16_Y, 0, 0 },
  /* 0x5A */ { M65816_PHY, OP_NONE, 0, 0 },
  /* 0x5B */ { M65816_TCD, OP_NONE, 0, 0 },
  /* 0x5C */ { M65816_JMP, OP_ADDRESS24, 0, 0 },
  /* 0x5D */ { M65816_EOR, OP_INDEXED16_X, 0, 0 },
  /* 0x5E */ { M65816_LSR, OP_INDEXED16_X, 0, 0 },
  /* 0x5F */ { M65816_EOR, OP_INDEXED24_X, 0, 0 },

  /* 0x60 */ { M65816_RTS, OP_NONE, 0, 0 },
  /* 0x61 */ { M65816_ADC, OP_X_INDIRECT8, 0, 0 },
  /* 0x62 */ { M65816_PER, OP_RELATIVE_LONG, 0, 0 },
  /* 0x63 */ { M65816_ADC, OP_SP_RELATIVE, 0, 0 },
  /* 0x64 */ { M65816_STZ, OP_ADDRESS8, 0, 0 },
  /* 0x65 */ { M65816_ADC, OP_ADDRESS8, 0, 0 },
  /* 0x66 */ { M65816_ROR, OP_ADDRESS8, 0, 0 },
  /* 0x67 */ { M65816_ADC, OP_INDIRECT8_LONG, 0, 0 },
  /* 0x68 */ { M65816_PLA, OP_NONE, 0, 0 },
  /* 0x69 */ { M65816_ADC, OP_IMMEDIATE16, 0, 0 },
  /* 0x6A */ { M65816_ROR, OP_NONE, 0, 0 },
  /* 0x6B */ { M65816_RTL, OP_NONE, 0, 0 },
  /* 0x6C */ { M65816_JMP, OP_INDIRECT16, 0, 0 },
  /* 0x6D */ { M65816_ADC, OP_ADDRESS16, 0, 0 },
  /* 0x6E */ { M65816_ROR, OP_ADDRESS16, 0, 0 },
  /* 0x6F */ { M65816_ADC, OP_ADDRESS24, 0, 0 },

  /* 0x70 */ { M65816_BVS, OP_RELATIVE, 0, 0 },
  /* 0x71 */ { M65816_ADC, OP_INDIRECT8_Y, 0, 0 },
  /* 0x72 */ { M65816_ADC, OP_INDIRECT8, 0, 0 },
  /* 0x73 */ { M65816_ADC, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0x74 */ { M65816_STZ, OP_INDEXED8_X, 0, 0 },
  /* 0x75 */ { M65816_ADC, OP_INDEXED8_X, 0, 0 },
  /* 0x76 */ { M65816_ROR, OP_INDEXED8_X, 0, 0 },
  /* 0x77 */ { M65816_ADC, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0x78 */ { M65816_SEI, OP_NONE, 0, 0 },
  /* 0x79 */ { M65816_ADC, OP_INDEXED16_Y, 0, 0 },
  /* 0x7A */ { M65816_PLY, OP_NONE, 0, 0 },
  /* 0x7B */ { M65816_TDC, OP_NONE, 0, 0 },
  /* 0x7C */ { M65816_JMP, OP_X_INDIRECT16, 0, 0 },
  /* 0x7D */ { M65816_ADC, OP_INDEXED16_X, 0, 0 },
  /* 0x7E */ { M65816_ROR, OP_INDEXED16_X, 0, 0 },
  /* 0x7F */ { M65816_ADC, OP_INDEXED24_X, 0, 0 },

  /* 0x80 */ { M65816_BRA, OP_RELATIVE, 0, 0 },
  /* 0x81 */ { M65816_STA, OP_X_INDIRECT8, 0, 0 },
  /* 0x82 */ { M65816_BRL, OP_RELATIVE_LONG, 0, 0 },
  /* 0x83 */ { M65816_STA, OP_SP_RELATIVE, 0, 0 },
  /* 0x84 */ { M65816_STY, OP_ADDRESS8, 0, 0 },
  /* 0x85 */ { M65816_STA, OP_ADDRESS8, 0, 0 },
  /* 0x86 */ { M65816_STX, OP_ADDRESS8, 0, 0 },
  /* 0x87 */ { M65816_STA, OP_INDIRECT8_LONG, 0, 0 },
  /* 0x88 */ { M65816_DEY, OP_NONE, 0, 0 },
  /* 0x89 */ { M65816_BIT, OP_IMMEDIATE16, 0, 0 },
  /* 0x8A */ { M65816_TXA, OP_NONE, 0, 0 },
  /* 0x8B */ { M65816_PHB, OP_NONE, 0, 0 },
  /* 0x8C */ { M65816_STY, OP_ADDRESS16, 0, 0 },
  /* 0x8D */ { M65816_STA, OP_ADDRESS16, 0, 0 },
  /* 0x8E */ { M65816_STX, OP_ADDRESS16, 0, 0 },
  /* 0x8F */ { M65816_STA, OP_ADDRESS24, 0, 0 },

  /* 0x90 */ { M65816_BCC, OP_RELATIVE, 0, 0 },
  /* 0x91 */ { M65816_STA, OP_INDIRECT8_Y, 0, 0 },
  /* 0x92 */ { M65816_STA, OP_INDIRECT8, 0, 0 },
  /* 0x93 */ { M65816_STA, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0x94 */ { M65816_STY, OP_INDEXED8_X, 0, 0 },
  /* 0x95 */ { M65816_STA, OP_INDEXED8_X, 0, 0 },
  /* 0x96 */ { M65816_STX, OP_INDEXED8_Y, 0, 0 },
  /* 0x97 */ { M65816_STA, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0x98 */ { M65816_TYA, OP_NONE, 0, 0 },
  /* 0x99 */ { M65816_STA, OP_INDEXED16_Y, 0, 0 },
  /* 0x9A */ { M65816_TXS, OP_NONE, 0, 0 },
  /* 0x9B */ { M65816_TXY, OP_NONE, 0, 0 },
  /* 0x9C */ { M65816_STZ, OP_ADDRESS16, 0, 0 },
  /* 0x9D */ { M65816_STA, OP_INDEXED16_X, 0, 0 },
  /* 0x9E */ { M65816_STZ, OP_INDEXED16_X, 0, 0 },
  /* 0x9F */ { M65816_STA, OP_INDEXED24_X, 0, 0 },

  /* 0xA0 */ { M65816_LDY, OP_IMMEDIATE16, 0, 0 },
  /* 0xA1 */ { M65816_LDA, OP_X_INDIRECT8, 0, 0 },
  /* 0xA2 */ { M65816_LDX, OP_IMMEDIATE16, 0, 0 },
  /* 0xA3 */ { M65816_LDA, OP_SP_RELATIVE, 0, 0 },
  /* 0xA4 */ { M65816_LDY, OP_ADDRESS8, 0, 0 },
  /* 0xA5 */ { M65816_LDA, OP_ADDRESS8, 0, 0 },
  /* 0xA6 */ { M65816_LDX, OP_ADDRESS8, 0, 0 },
  /* 0xA7 */ { M65816_LDA, OP_INDIRECT8_LONG, 0, 0 },
  /* 0xA8 */ { M65816_TAY, OP_NONE, 0, 0 },
  /* 0xA9 */ { M65816_LDA, OP_IMMEDIATE16, 0, 0 },
  /* 0xAA */ { M65816_TAX, OP_NONE, 0, 0 },
  /* 0xAB */ { M65816_PLB, OP_NONE, 0, 0 },
  /* 0xAC */ { M65816_LDY, OP_ADDRESS16, 0, 0 },
  /* 0xAD */ { M65816_LDA, OP_ADDRESS16, 0, 0 },
  /* 0xAE */ { M65816_LDX, OP_ADDRESS16, 0, 0 },
  /* 0xAF */ { M65816_LDA, OP_ADDRESS24, 0, 0 },

  /* 0xB0 */ { M65816_BCS, OP_RELATIVE, 0, 0 },
  /* 0xB1 */ { M65816_LDA, OP_INDIRECT8_Y, 0, 0 },
  /* 0xB2 */ { M65816_LDA, OP_INDIRECT8, 0, 0 },
  /* 0xB3 */ { M65816_LDA, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0xB4 */ { M65816_LDY, OP_INDEXED8_X, 0, 0 },
  /* 0xB5 */ { M65816_LDA, OP_INDEXED8_X, 0, 0 },
  /* 0xB6 */ { M65816_LDX, OP_INDEXED8_Y, 0, 0 },
  /* 0xB7 */ { M65816_LDA, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0xB8 */ { M65816_CLV, OP_NONE, 0, 0 },
  /* 0xB9 */ { M65816_LDA, OP_INDEXED16_Y, 0, 0 },
  /* 0xBA */ { M65816_TSX, OP_NONE, 0, 0 },
  /* 0xBB */ { M65816_TYX, OP_NONE, 0, 0 },
  /* 0xBC */ { M65816_LDY, OP_INDEXED16_X, 0, 0 },
  /* 0xBD */ { M65816_LDA, OP_INDEXED16_X, 0, 0 },
  /* 0xBE */ { M65816_LDX, OP_INDEXED16_Y, 0, 0 },
  /* 0xBF */ { M65816_LDA, OP_INDEXED24_X, 0, 0 },

  /* 0xC0 */ { M65816_CPY, OP_IMMEDIATE16, 0, 0 },
  /* 0xC1 */ { M65816_CMP, OP_X_INDIRECT8, 0, 0 },
  /* 0xC2 */ { M65816_REP, OP_IMMEDIATE8, 0, 0 },
  /* 0xC3 */ { M65816_CMP, OP_SP_RELATIVE, 0, 0 },
  /* 0xC4 */ { M65816_CPY, OP_ADDRESS8, 0, 0 },
  /* 0xC5 */ { M65816_CMP, OP_ADDRESS8, 0, 0 },
  /* 0xC6 */ { M65816_DEC, OP_ADDRESS8, 0, 0 },
  /* 0xC7 */ { M65816_CMP, OP_INDIRECT8_LONG, 0, 0 },
  /* 0xC8 */ { M65816_INY, OP_NONE, 0, 0 },
  /* 0xC9 */ { M65816_CMP, OP_IMMEDIATE16, 0, 0 },
  /* 0xCA */ { M65816_DEX, OP_NONE, 0, 0 },
  /* 0xCB */ { M65816_WAI, OP_NONE, 0, 0 },
  /* 0xCC */ { M65816_CPY, OP_ADDRESS16, 0, 0 },
  /* 0xCD */ { M65816_CMP, OP_ADDRESS16, 0, 0 },
  /* 0xCE */ { M65816_DEC, OP_ADDRESS16, 0, 0 },
  /* 0xCF */ { M65816_CMP, OP_ADDRESS24, 0, 0 },

  /* 0xD0 */ { M65816_BNE, OP_RELATIVE, 0, 0 },
  /* 0xD1 */ { M65816_CMP, OP_INDIRECT8_Y, 0, 0 },
  /* 0xD2 */ { M65816_CMP, OP_INDIRECT8, 0, 0 },
  /* 0xD3 */ { M65816_CMP, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0xD4 */ { M65816_PEI, OP_INDIRECT8, 0, 0 },
  /* 0xD5 */ { M65816_CMP, OP_INDEXED8_X, 0, 0 },
  /* 0xD6 */ { M65816_DEC, OP_INDEXED8_X, 0, 0 },
  /* 0xD7 */ { M65816_CMP, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0xD8 */ { M65816_CLD, OP_NONE, 0, 0 },
  /* 0xD9 */ { M65816_CMP, OP_INDEXED16_Y, 0, 0 },
  /* 0xDA */ { M65816_PHX, OP_NONE, 0, 0 },
  /* 0xDB */ { M65816_STP, OP_NONE, 0, 0 },
  /* 0xDC */ { M65816_JMP, OP_INDIRECT16_LONG, 0, 0 },
  /* 0xDD */ { M65816_CMP, OP_INDEXED16_X, 0, 0 },
  /* 0xDE */ { M65816_DEC, OP_INDEXED16_X, 0, 0 },
  /* 0xDF */ { M65816_CMP, OP_INDEXED24_X, 0, 0 },

  /* 0xE0 */ { M65816_CPX, OP_IMMEDIATE16, 0, 0 },
  /* 0xE1 */ { M65816_SBC, OP_X_INDIRECT8, 0, 0 },
  /* 0xE2 */ { M65816_SEP, OP_IMMEDIATE16, 0, 0 },
  /* 0xE3 */ { M65816_SBC, OP_SP_RELATIVE, 0, 0 },
  /* 0xE4 */ { M65816_CPX, OP_ADDRESS8, 0, 0 },
  /* 0xE5 */ { M65816_SBC, OP_ADDRESS8, 0, 0 },
  /* 0xE6 */ { M65816_INC, OP_ADDRESS8, 0, 0 },
  /* 0xE7 */ { M65816_SBC, OP_INDIRECT8_LONG, 0, 0 },
  /* 0xE8 */ { M65816_INX, OP_NONE, 0, 0 },
  /* 0xE9 */ { M65816_SBC, OP_IMMEDIATE16, 0, 0 },
  /* 0xEA */ { M65816_NOP, OP_NONE, 0, 0 },
  /* 0xEB */ { M65816_XBA, OP_NONE, 0, 0 },
  /* 0xEC */ { M65816_CPX, OP_ADDRESS16, 0, 0 },
  /* 0xED */ { M65816_SBC, OP_ADDRESS16, 0, 0 },
  /* 0xEE */ { M65816_INC, OP_ADDRESS16, 0, 0 },
  /* 0xEF */ { M65816_SBC, OP_ADDRESS24, 0, 0 },

  /* 0xF0 */ { M65816_BEQ, OP_RELATIVE, 0, 0 },
  /* 0xF1 */ { M65816_SBC, OP_INDIRECT8_Y, 0, 0 },
  /* 0xF2 */ { M65816_SBC, OP_INDIRECT8, 0, 0 },
  /* 0xF3 */ { M65816_SBC, OP_SP_INDIRECT_Y, 0, 0 },
  /* 0xF4 */ { M65816_PEA, OP_ADDRESS16, 0, 0 },
  /* 0xF5 */ { M65816_SBC, OP_INDEXED8_X, 0, 0 },
  /* 0xF6 */ { M65816_INC, OP_INDEXED8_X, 0, 0 },
  /* 0xF7 */ { M65816_SBC, OP_INDIRECT8_Y_LONG, 0, 0 },
  /* 0xF8 */ { M65816_SED, OP_NONE, 0, 0 },
  /* 0xF9 */ { M65816_SBC, OP_INDEXED16_Y, 0, 0 },
  /* 0xFA */ { M65816_PLX, OP_NONE, 0, 0 },
  /* 0xFB */ { M65816_XCE, OP_NONE, 0, 0 },
  /* 0xFC */ { M65816_JSR, OP_X_INDIRECT16, 0, 0 },
  /* 0xFD */ { M65816_SBC, OP_INDEXED16_X, 0, 0 },
  /* 0xFE */ { M65816_INC, OP_INDEXED16_X, 0, 0 },
  /* 0xFF */ { M65816_SBC, OP_INDEXED24_X, 0, 0 }
};

