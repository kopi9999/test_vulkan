.PHONY = all compile link clean debug

CC = gcc
LINKERFLAG := -lm

SRD := src/
BD := build/
HDR := header/
SRCS := ${SRD}main.c ${SRD}window.c ${SRD}vinit.c ${SRD}misc.c
BINS := ${BD}main
OBJ := main.o window.o vinit.o misc.o
LIBS:= -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

all: compile link

debug: dcompile link

link:
	@echo "Linkowanie: "
	#ld -o ${BINS} ${OBJ} ${LIBS} -I ${HDR} --verbose
	${CC} ${LINKERFLAG} ${OBJ} -o ${BINS} ${LIBS} # -I ${HDR} #--verbose
	rm ${OBJ}

dcompile:
	@echo "Kompilacja: "
	${CC} -g -c ${SRCS} -I ${HDR}

compile:
	@echo "Kompilacja: "
	${CC} -c ${SRCS} -I ${HDR}

clean:
	@echo "Usuwanie plików wynikowych"
	rm -rvf ${BINS} ${OBJ}
