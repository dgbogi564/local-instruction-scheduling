/*
 *********************************************
 *  415 Compilers                            *
 *  Spring 2022                              *
 *  Students                                 *
 *  Authors: Ulrich Kremer                   *
 *********************************************
 */


#include "Instr.h"
#include <stdio.h>

#ifndef INSTR_UTILS_H
#define INSTR_UTILS_H

void PrintInstruction(FILE * outfile, Instruction * instr);
void PrintInstructionList(FILE * outfile, Instruction * instr);
Instruction *InitInstruction();
Instruction *ReadInstruction(FILE * infile);
void CalculateInstructionListWeightsAndHeights(Instruction * InstrList);
Instruction *ReadInstructionList(FILE * infile);
Instruction *LastInstruction(Instruction * instr);
void DestroyInstructionList(Instruction * instr);
InstructionNode *InitInstructionNode(Instruction * instr, int cycle);
void DestroyInstructionNode(InstructionNode * node);
InstructionQueue *InitInstructionQueue();
void Enqueue(InstructionQueue * InstrQueue, Instruction * instr, Heuristic HEURISTIC, int cycle);
Instruction *Dequeue(InstructionQueue * InstrQueue, int *curr_cycle);
void RemoveNode(InstructionQueue * InstrQueue, InstructionNode *InstrNode);
void DestroyInstructionQueue(InstructionQueue * InstrQueue);

#endif
