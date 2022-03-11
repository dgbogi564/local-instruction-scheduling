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
typedef enum {LONGEST_LATENCY_PATH, HIGHEST_LATENCY_INSTRUCTION, DEPENDENCE_HEIGHT} Heuristic;

typedef struct InstructionInfo Instruction;
typedef struct InstructionNode InstructionNode;
typedef struct InstructionQueue InstructionQueue;

struct InstructionInfo {
	OpCode opcode;
	int field1;
	int field2;
	int field3;
    int cycles;
    int earliestCycleCanRun;
    int weight;
    int height;
    int visited;
	Instruction *prev;	/* previous instruction */
	Instruction *next;	/* next instruction */
    Instruction *dep1;  /* first dependency of instruction */
    Instruction *dep2;  /* second dependency of instruction */
    Instruction *anti;  /* anti-dependency of instruction */
};

struct InstructionNode {
    Instruction *instr;
    InstructionNode *next;
};

struct InstructionQueue {
    int size;
    InstructionNode *head;
    InstructionNode *tail;
};

#endif
