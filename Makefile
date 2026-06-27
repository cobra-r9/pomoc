# ==============================================================================
# 1. COMPILER & FLAGS CONFIGURATION
# ==============================================================================
SRC_DIR  := src
OBJ_DIR  := build
INC_DIR  := include
BIN_DIR  := $(OBJ_DIR)/bin

CC       := gcc
CFLAGS   := -Wall -Wextra -Wpedantic -std=c23
CPPFLAGS := -I$(INC_DIR) -MMD -MP

# ==============================================================================
# 2. DYNAMIC FILE DETECTION
# ==============================================================================
COMMON_SRCS := $(filter-out $(SRC_DIR)/pomod.c $(SRC_DIR)/pomoc.c, $(wildcard $(SRC_DIR)/*.c))
COMMON_OBJS := $(COMMON_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

POMOD_OBJS  := $(OBJ_DIR)/pomod.o $(COMMON_OBJS)
POMOC_OBJS  := $(OBJ_DIR)/pomoc.o $(OBJ_DIR)/protocol.o

DEPS        := $(wildcard $(OBJ_DIR)/*.d)

# ==============================================================================
# 3. BUILD RULES
# ==============================================================================
.PHONY: all build run clean reset purge

all: build

build: $(BIN_DIR)/pomod $(BIN_DIR)/pomoc

$(BIN_DIR)/pomod: $(POMOD_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking executable: $@"
	@$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/pomoc: $(POMOC_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking executable: $@"
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling: $< -> $@"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: build
	@echo ""
	@echo "-------------------------[pomod]--------------------------------"
	@./$(BIN_DIR)/pomod &
	@echo "pomod started in background (PID: $$!)"
	@echo "----------------------------------------------------------------"
	@echo ""

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR)

reset:
	@echo "Resetting the project..."
	@rm -rf $(SRC_DIR)/*
	@rm -rf $(INC_DIR)/*
	@rm -rf $(OBJ_DIR)

purge: clean
	@echo "Cleaning makepkg residues..."
	@rm -rf pkg
	@rm -f *.tar.zst

# ==============================================================================
# 4. HEADER DEPENDENCY INCLUSION
# ==============================================================================
-include $(DEPS)
