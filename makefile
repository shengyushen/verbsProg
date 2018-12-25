OBJS = datastruct.o print_rdma.o ssysocket.o query_device.o main.o
EXES = query_device.exe
CC = gcc
LDFLAG = -libverbs

.DEFAULT_GOAL := all

all: ${EXES}

${EXES} : ${OBJS}
	${CC} -o $@ $^ ${LDFLAG} 

%.o : %.c
	${CC} -o $@ -c $<

.PHONY: clean

clean:
	rm -f *.exe ${OBJS}
