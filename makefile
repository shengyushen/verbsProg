OBJS = rdma_common.o print_rdma.o ssysocket.o rctest.o chk.o main.o
EXES = query_device.exe
CC = gcc
LDFLAG = -libverbs

.DEFAULT_GOAL := all

all: ${EXES}

${EXES} : ${OBJS}
	${CC} -g -o $@ $^ ${LDFLAG} 

%.o : %.c
	${CC} -g -o $@ -c $<

.PHONY: clean

clean:
	rm -f *.exe ${OBJS}
