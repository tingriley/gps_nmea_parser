CCFLAGS = -Wall -O3
OBJ_DIR  := obj
BIN_DIR  := bin
TARGET = gps
INCLUDE = -Iinclude/
LDFLAGS = -lm 

SRC     :=$(wildcard src/*.c) 
OBJECTS := $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(BIN_DIR)/$(TARGET)


$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) -o $@ -c $<


$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) $(LDFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJECTS)

.PHONY: clean  all


clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(BIN_DIR)/*
