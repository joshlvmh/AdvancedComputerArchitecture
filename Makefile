CC = g++
CC_FLAGS = -g -Wall -std=c++11 -std=gnu++11

TARGET = simulator
SRC = src
BIN = bin
BENCHMARKS = benchmarks
SOURCES = $(wildcard ${SRC}/*.cpp)
HEADERS = $(wildcard ${SRC}/*.h)

all:	${BIN}/${TARGET}

run:	
	./$(BIN)/$(TARGET) ${BENCHMARKS}/${BENCHMARK} ${PIPELINES} ${BRPR_TYPE} > ${BIN}/simulator.out

${BIN}/${TARGET}: ${SOURCES}
	${CC} ${CC_FLAGS} $^ -o $@

clean:
	-rm -rf ${BIN}/*
