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

typedef enum {LONGEST_LATENCY_PATH, HIGHEST_LATENCY_INSTRUCTION, CRITICAL_PATH} op;
void Schedule(Instruction *InstrList, op flag)
{

        if (flag == LONGEST_LATENCY_PATH) {
      // TODO

    } else
        if (flag == HIGHEST_LATENCY_INSTRUCTION) {
      // TODO
    } else
        if (flag == CRITICAL_PATH) {
      // TODO
    }
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
      Schedule(InstrList, LONGEST_LATENCY_PATH);
	  fprintf(stderr, "    HEURISTIC: longest latency weighted path\n\n");
	} else
        if (!strcmp(argv[1], "-b")) {
      Schedule(InstrList, HIGHEST_LATENCY_INSTRUCTION);
	  fprintf(stderr, "    HEURISTIC: highest latency instruction\n\n");
	} else
        if (!strcmp(argv[1], "-c")) {
      Schedule(InstrList, CRITICAL_PATH);
	  fprintf(stderr, "    HEURISTIC: critical path\n\n");
	} else  {
	  ERROR(" INVALID HEURISTIC: Use of command:\n  schedule -a/b/c < ILOC file\n");
	  exit(EXIT_FAILURE);
	}

        PrintInstructionList(stdout, InstrList);

	fprintf(stderr,"\n-----------------DONE---------------------------\n");

	return EXIT_SUCCESS;
}
