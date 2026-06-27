# How This Makefile Works

A Makefile isn't just a shell script with extra steps. We should treat it as a dependency graph. Make figures out what has changed since the last build and does the minimum work needed to get back to a consistent state. That's the core idea behind everything here.


## Part 1: Configuration Variables

```makefile
CC       := gcc
CFLAGS   := -Wall -Wextra -Wpedantic -O2 -std=c23
CPPFLAGS := -Iinclude -MMD -MP

NAME     := my_program

SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := $(OBJ_DIR)/bin
TARGET   := $(BIN_DIR)/$(NAME)
```

A few things worth noting:

**`:=` vs `=`** — `:=` evaluates immediately and stores the result. Plain `=` would re-evaluate the expression every time the variable is referenced. For path variables like these, you want `:=`.

**`CFLAGS` vs `CPPFLAGS`** — The split is intentional. `CFLAGS` goes to the compiler: warnings, optimization level, language standard. `CPPFLAGS` goes to the preprocessor, which runs before compilation. `-Iinclude` belongs there because it's how `#include` directives get resolved to actual paths.

**`-Iinclude`** — Adds the project's `include/` directory to the preprocessor's search path. You can write `#include "math_utils.h"` in any `.c` file and the preprocessor will find it, regardless of where that `.c` file lives. No relative path gymnastics needed.

**Directory nesting** — `BIN_DIR := $(OBJ_DIR)/bin` means the final executable ends up at `build/bin/my_program`. The project root stays uncluttered; `build/` can be deleted wholesale as a cleanup operation.


## Part 2: File Detection

```makefile
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
```

This replaces manually listing every source file in the Makefile. Say the project has `src/main.c` and `src/utils.c`:

1. `wildcard` scans the directory and produces: `src/main.c src/utils.c`
2. The pattern substitution `$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)` rewrites that to: `build/main.o build/utils.o`
3. `.o=.d` substitution gives: `build/main.d build/utils.d`

Add a new `.c` file to `src/` and it gets picked up automatically. Nothing in the Makefile needs to change.


## Part 3: Build Rules

```makefile
.PHONY: all build run clean test

all: build
build: $(TARGET)
```

**`.PHONY`** — Make normally works by comparing file timestamps. If a file named `clean` existed in the project directory, `make clean` would see it, notice it has no dependencies that are newer, and do nothing. `.PHONY` tells Make to treat these targets as commands, not files, so it always runs them.

**`all` → `build` → `$(TARGET)`** — This is the default chain. Running `make` with no arguments hits `all`, which requires `build`, which requires the binary. Make walks the dependency tree from there.

### Linking

```makefile
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking executable: $@"
	@$(CC) $(CFLAGS) $^ -o $@
```

Make sees that `$(TARGET)` depends on `$(OBJS)`. If those object files don't exist or are out of date, it compiles them first, then comes back here to link.

**`| $(BIN_DIR)` (order-only prerequisite)** — The pipe separates regular dependencies from order-only ones. It means: "this directory must exist before the rule runs, but don't factor its timestamp into whether the target needs rebuilding." Without it, touching anything in `build/bin/` would update the directory's mtime and potentially trick Make into thinking a rebuild was needed.

**Automatic variables** — `$@` expands to the target name. `$^` expands to all prerequisites. The `@` prefix suppresses echoing the command itself.

### Compiling object files

```makefile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling: $< -> $@"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
```

The `%` is a pattern match. This one rule handles every `.c` file in `src/`. To build `build/main.o`, Make looks for `src/main.c`. `$<` expands to just the first prerequisite (the specific `.c` file, not all of them). The `-c` flag tells GCC to produce an object file and stop — don't try to link yet.

### Directories, run, clean, test

```makefile
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

run: $(TARGET)
	@echo ""
	@echo "-------------------------[test]--------------------------------"
	@echo ""
	@./$(TARGET)
	@echo ""
	@echo "---------------------------------------------------------------"
	@echo ""

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR)

test: run clean
```

`mkdir -p` creates the directory (and any missing parents) without erroring if it already exists.

`make run` recompiles if anything is stale, then executes the binary. `make test` does the same and then runs `clean` afterward, so you end up with no build artifacts after the test run. Useful for verifying a clean build from scratch.


## Part 4: Header Dependency Tracking

```makefile
-include $(DEPS)
```

This is what `-MMD -MP` in `CPPFLAGS` was for.

By default, Make only knows about `.c` files. If you change a header, Make has no idea any of the translation units that include it are now stale. The naive fix is `make clean && make` every time you touch a header, but that defeats the whole point of incremental builds.

With `-MMD`, GCC generates a `.d` file alongside each object file during compilation. That `.d` file contains a Make rule like:

```makefile
build/main.o: src/main.c include/math_utils.h
```

`-MP` adds phony targets for each header so Make doesn't complain if a header gets deleted.

`-include` pulls all those generated `.d` files into the Makefile at parse time. The leading `-` means "don't error if these files don't exist yet" — which is the case on the very first build, before any `.d` files have been generated.

The result: if you modify a header, Make correctly identifies which object files depend on it and recompiles only those.

