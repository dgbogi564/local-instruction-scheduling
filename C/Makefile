#/*
# *********************************************
# *  415 Compilers                            *
# *  Spring 2019                              *
# *  Students                                 *
# *  Authors: Ulrich Kremer                   *
# *********************************************
# */


CCFLAGS = -ggdb -Wall -lm


compile: Scheduler.c InstrUtils.c InstrUtils.h Utils.c Utils.h
	gcc $(CCFLAGS) Scheduler.c InstrUtils.c Utils.c -o schedule

submission:
	rm -rf submission
	mkdir submission
	rsync -av --progress . submission --exclude submission
	tar -zcvf submission.tar submission

clean:
	rm -rf schedule
	rm -rf submission
	rm submission.tar