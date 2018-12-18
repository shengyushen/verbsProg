EXECS = query_device.exe
#MPICC = mpicc
CC = gcc
#CCFLAG = -fopenmp
LDFLAG = -libverbs

.DEFAULT_GOAL := all

all: ${EXECS}

%.exe : %.c
	${CC} -o $@ $< ${LDFLAG} 

.PHONY: clean

clean:
	rm -f ${EXECS}
