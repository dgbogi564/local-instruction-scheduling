/*
 *********************************************
 *  415 Compilers                            *
 *  Spring 2022                              *
 *  Students                                 *
 *  Authors: Ulrich Kremer                   *
 *********************************************
 */

#ifndef INSTR_H
#define INSTR_H

typedef enum {LOADI, LOADAI, STOREAI, ADD, SUB, MUL, DIV, OUTPUTAI} OpCode;

typedef struct InstructionInfo Instruction;

struct InstructionInfo {
	OpCode opcode;
	int field1;
	int field2;
	int field3;
    int cycles;
    int weight;
	Instruction *prev;	/* previous instruction */
	Instruction *next;	/* next instruction */
    Instruction *dep1;  /* first dependency of instruction */
    Instruction *dep2;  /* second dependency of instruction */
    Instruction *anti;  /* anti-dependency of instruction */
};

#endif
