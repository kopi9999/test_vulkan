.PHONY = all compile link clean debug

CC := gcc
SPVC := glslc
LINKERFLAG := -lm

SRD := src/
BD := build/
HDR := header/
SRCS := ${SRD}main.c ${SRD}window.c ${SRD}vinit.c ${SRD}misc.c ${SRD}vPipelineInit.c
BINS := ${BD}main
OBJ := main.o window.o vinit.o misc.o vPipelineInit.o
LIBS:= -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SHSRD := ${SRD}shaders/
SHBD := ${BD}shaders/
VERTO := vert.spv
FRAGO := frag.spv
#SHFLAGS := --target-env=vulkan1.3 --target-spv=spv1.4
SHFLAGS := --target-env=vulkan1.0
VERT := -fshader-stage=vertex ${SHSRD}shader.vert ${SHFLAGS} -o ${VERTO}
FRAG := -fshader-stage=fragment ${SHSRD}shader.frag ${SHFLAGS} -o ${FRAGO}
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


shader:
	@echo "Kompilacja shaderów: "
	${SPVC} ${VERT}
	${SPVC} ${FRAG}
	mv ${VERTO} ${SHBD}
	mv ${FRAGO} ${SHBD}


clean:
	@echo "Usuwanie plików wynikowych"
	rm -rvf ${BINS} ${OBJ}
