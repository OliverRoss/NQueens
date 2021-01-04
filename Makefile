INC_DIR=./
OUT_DIR=build
SRC_DIR=./
LIB_DIR=./

GCC ?= gcc
SANITIZERS ?= -fsanitize=undefined
CFLAGS := $(CFLAGS)
CFLAGS += -std=gnu11 -Wall -Wextra -pedantic -I./$(INC_DIR)/

targets = $(OUT_DIR)/main

all: CFLAGS += -O2
all: $(targets) 

.PHONEY: debug
debug: CFLAGS +=-Og -g $(SANITIZERS)
debug: $(targets) 

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@$(GCC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/%.o: $(INC_DIR)/%.h $(LIB_DIR)/%.c
	@$(GCC) $(CFLAGS) $(LIB_DIR)/$*.c -c -o $@ -l$*

$(OUT_DIR)/lib%.a: $(OUT_DIR)/%.o
	@ar -s -r $(OUT_DIR)/lib$*.a $(OUT_DIR)/$*.o

$(OUT_DIR)/main: $(OUT_DIR)/main.o $(OUT_DIR)/libalgo_x.a
	@$(GCC) $(CFLAGS) $< -o $@ -L$(OUT_DIR) -lalgo_x -lm

.PHONEY: clean
clean:
	@rm -rf $(OUT_DIR)

$(shell mkdir -p $(OUT_DIR))
