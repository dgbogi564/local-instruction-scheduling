/*
 *********************************************
 *  415 Compilers                            *
 *  Spring 2022                              *
 *  Students                                 *
 *  Authors: Ulrich Kremer                   *
 *********************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Instr.h"
#include "InstrUtils.h"
#include "Utils.h"

void PrintInstruction(FILE * outfile, Instruction * instr)
{
	if (!outfile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	if (instr) {
		switch (instr->opcode) {
		case LOADI:
			fprintf(outfile, "loadI %d => r%d\n", instr->field1,
				instr->field2);
			break;
		case LOADAI:
			fprintf(outfile, "loadAI r%d, %d => r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case STOREAI:
			fprintf(outfile, "storeAI r%d => r%d, %d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case ADD:
			fprintf(outfile, "add r%d, r%d => r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case SUB:
			fprintf(outfile, "sub r%d, r%d => r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case MUL:
			fprintf(outfile, "mult r%d, r%d => r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case DIV:
			fprintf(outfile, "div r%d, r%d => r%d\n", instr->field1,
				instr->field2, instr->field3);
			break;
		case OUTPUTAI:
		  fprintf(outfile, "outputAI r%d, %d\n", instr->field1, instr->field2);
			break;
		default:
			ERROR("Illegal instructions\n");
		}
	}
}

Instruction *InitInstruction() {
    Instruction *instr = (Instruction *) calloc(1, sizeof(Instruction));
    if (!instr) {
        ERROR("Calloc failed\n");
        exit(EXIT_FAILURE);
    }
    instr->prev = NULL;
    instr->next = NULL;
    instr->dep1 = NULL;
    instr->dep2 = NULL;
    instr->anti = NULL;
    instr->visited = 0;
    return instr;
}

void PrintInstructionList(FILE * outfile, Instruction * instr)
{
	Instruction *prev;

	while (instr) {
		PrintInstruction(outfile, instr);
		prev = instr;
		instr = instr->next;
		free(prev);
	}
}

Instruction *ReadInstruction(FILE * infile)
{
	static char InstrBuffer[100];
	Instruction *instr = NULL;
	char dummy;

	if (!infile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	instr = InitInstruction();
	fscanf(infile, "%99s", InstrBuffer);
	if (strnlen(InstrBuffer, sizeof(InstrBuffer)) == 0) {
		free(instr);
		return NULL;
	}
	if (!strcmp(InstrBuffer, "loadI")) {
		instr->opcode = LOADI;
		/* get first operand: immediate constant */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%d", &(instr->field1));
		/* skip over "=>"  */
		fscanf(infile, "%s", InstrBuffer);
		/* get second operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
        /* get cycles */
        instr->cycles = 1;
	} else if (!strcmp(InstrBuffer, "loadAI")) {
		instr->opcode = LOADAI;
		/* get first operand: base register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: immediate constant */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%d", &(instr->field2));
		/* skip over "=>"  */
		fscanf(infile, "%s", InstrBuffer);
		/* get third operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
        /* get cycles */
        instr->cycles = 5;
	} else if (!strcmp(InstrBuffer, "storeAI")) {
		instr->opcode = STOREAI;
		/* get first operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* skip over "=>"  */
		fscanf(infile, "%s", InstrBuffer);
		/* get base register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* get second operand: immediate constant */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%d", &(instr->field3));
        /* get cycles */
        instr->cycles = 5;
	} else if (!strcmp(InstrBuffer, "add")) {
		instr->opcode = ADD;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* skip over "=>"  */
		fscanf(infile, "%s", InstrBuffer);
		/* get third operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
        /* get cycles */
        instr->cycles = 1;
	} else if (!strcmp(InstrBuffer, "sub")) {
		instr->opcode = SUB;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* skip over "=>"  */
		fscanf(infile, "%s", InstrBuffer);
		/* get third operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
        /* get cycles */
        instr->cycles = 1;
	} else if (!strcmp(InstrBuffer, "mult")) {
		instr->opcode = MUL;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* skip over "=>"  */
		fscanf(infile, "%s", InstrBuffer);
		/* get third operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
        /* get cycles */
        instr->cycles = 3;
	} else if (!strcmp(InstrBuffer, "div")) {
		instr->opcode = DIV;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field2));
		/* skip over "=>"  */
		fscanf(infile, "%s", InstrBuffer);
		/* get third operand: register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field3));
        /* get cycles */
        instr->cycles = 3;
	} else if (!strcmp(InstrBuffer, "outputAI")) {
		instr->opcode = OUTPUTAI;
		/* get first operand: target register */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%c%d", &dummy, &(instr->field1));
		/* get second operand: immediate constant */
		fscanf(infile, "%s", InstrBuffer);
		sscanf(InstrBuffer, "%d", &(instr->field2));
        /* get cycles */
        instr->cycles = 1;
	} else {
		free(instr);
		return NULL;
	}
	return instr;
}

typedef struct Dependency Dependency;
struct Dependency {
    int value;
    int isMemoryLocationOffset;
};
void CalculateInstructionListWeights(Instruction * InstrList)
{
    Instruction *instr = LastInstruction(InstrList);
    do {
        if (!instr->weight) {
            instr->weight = instr->cycles;
        }
        Instruction *i = instr;
        Dependency dep1 = {-1, 0};
        Dependency dep2 = {-1, 0};
        Dependency anti = {-1, 0};
        int get_dep1, get_dep2, get_anti;

        switch(instr->opcode) {
            case LOADI:
                /* registers */
                anti.value = instr->field2;
                break;
            case LOADAI:
                /* registers */
                dep1.value = instr->field1;
                anti.value = instr->field3;
                /* memory location offsets */
                dep2.value = instr->field2;
                dep2.isMemoryLocationOffset = 1;
                break;
            case STOREAI:
                /* registers */
                dep1.value = instr->field1;
                dep2.value = instr->field2;
                /* memory location offsets */
                anti.value = instr->field3;
                anti.isMemoryLocationOffset = 1;
                break;
            case ADD:
            case SUB:
            case MUL:
            case DIV:
                /* registers */
                dep1.value = instr->field1;
                dep2.value = instr->field2;
                anti.value = instr->field3;
                break;
            case OUTPUTAI:
                /* memory location offsets */
                dep1.value = instr->field2;
                dep1.isMemoryLocationOffset = 1;
                break;
            default:
                ERROR("Illegal instructions\n");
                return;
        }

        get_dep1 = dep1.value >= 0;
        get_dep2 = dep2.value >= 0;
        get_anti = anti.value >= 0;
        /* search for the dependencies and anti-dependencies of the current instruction */
        while ((i = i->prev) && (get_dep1 || get_dep2 || get_anti)) {
            /* dependencies */
            switch(i->opcode) {
                /* writes to a memory location */
                case STOREAI:
                    if (get_dep1 && dep1.isMemoryLocationOffset && dep1.value == i->field3) {
                        instr->dep1 = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_dep1 = 0;
                    }
                    if (get_dep2 && dep2.isMemoryLocationOffset && dep2.value == i->field3) {
                        instr->dep2 = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_dep2 = 0;
                    }
                /* writes to a register */
                case LOADI:
                    if (get_dep1 && !dep1.isMemoryLocationOffset && dep1.value == i->field2) {
                        instr->dep1 = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_dep1 = 0;
                    }
                    if (get_dep2 && !dep2.isMemoryLocationOffset && dep2.value == i->field2) {
                        instr->dep2 = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_dep2 = 0;
                    }
                    break;
                case LOADAI:
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                    if (get_dep1 && !dep1.isMemoryLocationOffset && dep1.value == i->field3) {
                        instr->dep1 = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_dep1 = 0;
                    }
                    if (get_dep2 && !dep2.isMemoryLocationOffset && dep2.value == i->field3) {
                        instr->dep2 = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_dep2 = 0;
                    }
                    break;
                /* does not write */
                case OUTPUTAI:
                    break;
                default:
                    ERROR("Illegal instructions\n");
                    return;
            }

            /* anti-dependencies */
            switch(i->opcode) {
                /* reads from a memory location */
                case LOADAI:
                case OUTPUTAI:
                    if (get_anti && anti.isMemoryLocationOffset && anti.value == i->field2) {
                        instr->anti = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_anti = 0;
                    }
                    break;
                /* reads from a register */
                case STOREAI:
                    if (get_anti && !anti.isMemoryLocationOffset && anti.value == i->field1) {
                        instr->anti = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_anti = 0;
                    }
                    break;
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                    if (get_anti && !anti.isMemoryLocationOffset && (anti.value == i->field1 || anti.value == i->field2)) {
                        instr->anti = i;
                        if (i->weight < instr->weight + i->cycles) {
                            i->weight = instr->weight + i->cycles;
                        }
                        get_anti = 0;
                    }
                    break;
                /* does not read */
                case LOADI:
                    break;
                default:
                    ERROR("Illegal instructions\n");
                    return;
            }
        }
    } while ((instr = instr->prev));
}

Instruction *ReadInstructionList(FILE * infile)
{
	Instruction *instr, *head, *tail;

	if (!infile) {
		ERROR("File error\n");
		exit(EXIT_FAILURE);
	}
	head = tail = NULL;
	while ((instr = ReadInstruction(infile))) {
		if (!head) {
			head = tail = instr;
			continue;
		}
		instr->prev = tail;
		instr->next = NULL;
		tail->next = instr;
		tail = instr;
	}
    CalculateInstructionListWeights(head);
	return head;
}

Instruction *LastInstruction(Instruction * instr)
{
	if (!instr) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}
	while (instr->next)
		instr = instr->next;
	return instr;
}

void DestroyInstructionList(Instruction * instr)
{
	Instruction *i;

	if (!instr)
		return;
	while (instr) {
		i = instr;
		instr = instr->next;
		free(i);
	}
}

InstructionNode *InitInstructionNode(Instruction * instr, int earliestCycleCanRun)
{
    InstructionNode *InstrNode = (InstructionNode *) calloc(1, sizeof(InstructionNode));
    if (!InstrNode) {
        ERROR("Calloc failed\n");
        exit(EXIT_FAILURE);
    }
    InstrNode->earliestCycleCanRun = earliestCycleCanRun;
    InstrNode->instr = instr;
    InstrNode->next = NULL;
    return InstrNode;
}

void DestroyInstructionNode(InstructionNode * node) {
    free(node);
}

InstructionQueue *InitInstructionQueue()
{
    InstructionQueue *InstrQueue = (InstructionQueue *) calloc(1, sizeof(InstructionQueue));
    if (!InstrQueue) {
        ERROR("Calloc failed\n");
        exit(EXIT_FAILURE);
    }
    InstrQueue->head = NULL;
    InstrQueue->tail = NULL;
    InstrQueue->size = 0;
    return InstrQueue;
}

void Enqueue(InstructionQueue * InstrQueue, Instruction * instr, Heuristic HEURISTIC, int earliestCycleCanRun)
{
    InstructionNode *InstrNode = InitInstructionNode(instr, earliestCycleCanRun);
    if (!InstrQueue->size) {
        InstrQueue->head = InstrQueue->tail = InstrNode;
        InstrQueue->size++;
        return;
    }
    InstructionNode *i = InstrQueue->head;
    switch(HEURISTIC) {
        case LONGEST_LATENCY_PATH:
            if (instr->weight > i->instr->weight || (instr->weight == i->instr->weight && instr->cycles > i->instr->cycles)) {
                InstrNode->next = i;
                InstrQueue->head = InstrNode;
            }
            while (i->next != NULL && instr->weight <= i->next->instr->weight) {
                i = i->next;
            }
            break;
        case HIGHEST_LATENCY_INSTRUCTION:
            if (instr->cycles > i->instr->cycles) {
                InstrNode->next = i;
                InstrQueue->head = InstrNode;
            }
            while (i->next != NULL && instr->cycles <= i->next->instr->cycles) {
                i = i->next;
            }
            break;
        case MY_OWN:
            break;
        default:
            ERROR("Illegal heuristic\n");
    }
    if (InstrQueue->head != InstrNode) {
        InstrNode->next = i->next;
        i->next = InstrNode;
        if(!InstrNode->next) {
            InstrQueue->tail = InstrNode;
        }
    }
    InstrQueue->size++;
}

Instruction *Dequeue(InstructionQueue * InstrQueue, int *cycle)
{
    if (!InstrQueue->size) {
        return NULL;
    }

    InstructionNode *InstrNode, *lowestCycleInstrNode;
    InstrNode = lowestCycleInstrNode = InstrQueue->head;
    while (InstrNode != NULL) {
        if (InstrNode->earliestCycleCanRun <= *cycle) {
            break;
        }
        if (lowestCycleInstrNode->earliestCycleCanRun > InstrNode->earliestCycleCanRun) {
            lowestCycleInstrNode = InstrNode;
        }
        InstrNode = InstrNode->next;
    }
    if (!InstrNode) {
        InstrNode = lowestCycleInstrNode;
    }
    *cycle = InstrNode->earliestCycleCanRun + 1;

    Instruction *instr = InstrNode->instr;
    RemoveNode(InstrQueue, InstrNode);
    return instr;
}

void RemoveNode(InstructionQueue * InstrQueue, InstructionNode *InstrNode) {
    if (!InstrQueue->size || !InstrNode) {
        return;
    }
    InstructionNode  *i = InstrQueue->head;
    if (i == InstrNode) {
        InstrQueue->head = i->next;
    }
    while (i->next != NULL) {
        if (i->next == InstrNode) {
            i->next = InstrNode->next;
            break;
        }
        i = i->next;
    }
    DestroyInstructionNode(InstrNode);
    InstrQueue->size--;
}

void DestroyInstructionQueue(InstructionQueue * InstrQueue)
{
    InstructionNode *InstrNode = InstrQueue->head, *temp;
    while(InstrNode) {
        temp = InstrNode;
        InstrNode = InstrNode->next;
        DestroyInstructionNode(temp);
    }
    free(InstrQueue);
}