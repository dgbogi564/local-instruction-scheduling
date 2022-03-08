/*
 *********************************************
 *  415 Compilers                            *
 *  Spring 2022                              *
 *  Students                                 *
 *  Authors: Ulrich Kremer                   *
 *********************************************
 */


#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "Instr.h"
#include "InstrUtils.h"
#include "Utils.h"

Instruction *Schedule(Instruction *InstrList, Heuristic HEURISTIC) {
    InstructionQueue *InstrQueue = InitInstructionQueue();
    Instruction *NewInstrList = NULL;
    Instruction *instr, *i, *tail;
    int cycle = 0;

    instr = InstrList;
    do {
        if (!instr->dep1 && !instr->dep2 && !instr->anti) {
            Enqueue(InstrQueue, instr, HEURISTIC, cycle);
            instr->visited = 1;
        }
    } while ((instr = instr->next));

    while ((i = Dequeue(InstrQueue, &cycle))) {
        instr = InitInstruction();
        instr->opcode = i->opcode;
        instr->field1 = i->field1;
        instr->field2 = i->field2;
        instr->field3 = i->field3;
        if (!NewInstrList) {
            NewInstrList = instr;
            tail = instr;
        } else {
            tail->next = instr;
            instr->prev = tail;
            tail = instr;
        }
        instr = i;
        while ((instr = instr->next)) {
            if (instr->visited) {
                continue;
            }
            if (instr->dep1 == i) {
                instr->dep1 = NULL;
            }
            if (instr->dep2 == i) {
                instr->dep2 = NULL;
            }
            if (instr->anti == i) {
                instr->anti = NULL;
            }
            if (!instr->dep1 && !instr->dep2 && !instr->anti) {
                Enqueue(InstrQueue, instr, HEURISTIC, (cycle-1) + i->cycles);
                instr->visited = 1;
            }
        }
    }

    DestroyInstructionQueue(InstrQueue);
    DestroyInstructionList(InstrList);
    return NewInstrList;
}

int main(int argc, char *argv[])
{
    Instruction *InstrList = NULL;

    if (argc != 2) {
        ERROR("Use of command:\n  schedule -a/b/c < ILOC file\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr,"------------------------------------------------\n");
    fprintf(stderr,"            Instruction Scheduler\n               415:compilers\n                Spring 2022\n");
    fprintf(stderr,"------------------------------------------------\n");

    InstrList = ReadInstructionList(stdin);

    if (!strcmp(argv[1], "-a")) {
        fprintf(stderr, "   HEURISTIC: longest latency weighted path\n\n");
        InstrList = Schedule(InstrList, LONGEST_LATENCY_PATH);
    } else
        if (!strcmp(argv[1], "-b")) {
        fprintf(stderr, "    HEURISTIC: highest latency instruction\n\n");
        InstrList = Schedule(InstrList, HIGHEST_LATENCY_INSTRUCTION);
    } else
        if (!strcmp(argv[1], "-c")) {
        fprintf(stderr, "         HEURISTIC: dependence height\n\n");
        InstrList = Schedule(InstrList, DEPENDENCE_HEIGHT);
    } else {
        ERROR("INVALID HEURISTIC: Use of command:\n  schedule -a/b/c < ILOC file\n");
        exit(EXIT_FAILURE);
    }

    PrintInstructionList(stdout, InstrList);

    fprintf(stderr,"\n-----------------DONE---------------------------\n");

    return EXIT_SUCCESS;
}
