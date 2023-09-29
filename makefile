.PHONY = all compile link clean debug

CC = gcc
LINKERFLAG := -lm

SRCS := main.c
BINS := main
OBJ := main.o
LIBS:= -lglfw

all: compile link

debug: dcompile link

link:
	@echo "Linkowanie: "
	#ld -o ${BINS} ${OBJ} ${LIBS} --verbose
	${CC} ${LINKERFLAG} ${OBJ} -o ${BINS} ${LIBS} #--verbose

dcompile:
	@echo "Kompilacja: "
	${CC} -g -c ${SRCS}
compile:
	@echo "Kompilacja: "
	${CC} -c ${SRCS}

clean:
	@echo "Usuwanie plików wynikowych"
	rm -rvf ${BINS} ${OBJ}
