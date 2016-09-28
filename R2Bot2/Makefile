NAME=R2Bot2
CC = g++
CCFLAGS = -std=c++11

SRC=./src
INCLUDE=./include
BUILD=./build
BIN=./bin

SOURCES=$(wildcard $(SRC)/*.cpp)
OBJECTS=$(addprefix $(BUILD)/, $(notdir $(SOURCES:.cpp=.o)))
LIBRARIES=-lstdc++

all: $(OBJECTS) $(BIN)/$(NAME)

new: clean all

build:
	-mkdir -p $(BUILD)

$(BIN)/$(NAME): $(OBJECTS)
	$(AR) rcs "$@" $(OBJECTS)

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) $(CCFLAGS) -c -I "$(INCLUDE)" "$<" -o "$@"

clean:
	-rm $(BUILD)/*
