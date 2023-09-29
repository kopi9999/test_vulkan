.PHONY = all compile link clean debug

CC = gcc
LINKERFLAG := -lm

SRD := src/
BD := build/
SRCS := ${SRD}main.c
BINS := ${BD}main
OBJ := main.o
LIBS:= -lglfw -lvulkan

all: compile link

debug: dcompile link

link:
	@echo "Linkowanie: "
	#ld -o ${BINS} ${OBJ} ${LIBS} --verbose
	${CC} ${LINKERFLAG} ${OBJ} -o ${BINS} ${LIBS} #--verbose
	rm ${OBJ}

dcompile:
	@echo "Kompilacja: "
	${CC} -g -c ${SRCS}
compile:
	@echo "Kompilacja: "
	${CC} -c ${SRCS}

clean:
	@echo "Usuwanie plików wynikowych"
	rm -rvf ${BINS} ${OBJ}
