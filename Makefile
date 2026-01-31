# Hunter Protocol: Root Makefile
# Build all libraries and tools with security-focused flags

.PHONY: all clean test libs tools capstone help

# Compiler settings
CC := gcc
CSTD := -std=c17

# Warning flags (strict)
WARNINGS := -Wall -Wextra -Werror -pedantic
WARNINGS += -Wconversion -Wsign-conversion -Wshadow
WARNINGS += -Wformat=2 -Wformat-security
WARNINGS += -Wnull-dereference -Wstack-protector
WARNINGS += -Wstrict-overflow=2

# Security hardening
HARDENING := -fstack-protector-strong
HARDENING += -D_FORTIFY_SOURCE=2
HARDENING += -fPIE

# Sanitizers (debug builds)
SANITIZERS := -fsanitize=address,undefined
SANITIZERS += -fno-omit-frame-pointer

# Debug flags
DEBUG := -g3 -O0

# Release flags  
RELEASE := -O2 -DNDEBUG

# Combined flags
CFLAGS := $(CSTD) $(WARNINGS) $(HARDENING) $(DEBUG) $(SANITIZERS)
LDFLAGS := -pie $(SANITIZERS)

# Export for sub-makefiles
export CC CFLAGS LDFLAGS

# Library directories
LIB_DIRS := lib/safe-string \
            lib/arena-allocator \
            lib/safe-integer \
            lib/priv-drop \
            lib/seccomp-sandbox \
            lib/fd-ops \
            lib/signal-safe-log \
            lib/secure-socket

# Tool directories
TOOL_DIRS := tools/priv-audit \
             tools/race-detector \
             tools/protocol-fuzzer

# Capstone
CAPSTONE_DIR := capstone/hardened-daemon

# Targets
all: libs tools

libs:
	@echo "=== Building Libraries ==="
	@for dir in $(LIB_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "Building $$dir..."; \
			$(MAKE) -C $$dir || exit 1; \
		fi \
	done

tools: libs
	@echo "=== Building Tools ==="
	@for dir in $(TOOL_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "Building $$dir..."; \
			$(MAKE) -C $$dir || exit 1; \
		fi \
	done

capstone: libs
	@echo "=== Building Capstone ==="
	@if [ -f $(CAPSTONE_DIR)/Makefile ]; then \
		$(MAKE) -C $(CAPSTONE_DIR); \
	fi

test:
	@echo "=== Running Tests ==="
	@for dir in $(LIB_DIRS) $(TOOL_DIRS); do \
		if [ -f $$dir/Makefile ]; then \
			echo "Testing $$dir..."; \
			$(MAKE) -C $$dir test || exit 1; \
		fi \
	done

clean:
	@echo "=== Cleaning ==="
	@for dir in $(LIB_DIRS) $(TOOL_DIRS) $(CAPSTONE_DIR); do \
		if [ -f $$dir/Makefile ]; then \
			$(MAKE) -C $$dir clean; \
		fi \
	done

# Build without sanitizers (for performance testing)
release:
	$(MAKE) CFLAGS="$(CSTD) $(WARNINGS) $(HARDENING) $(RELEASE)" \
	        LDFLAGS="-pie" all

# Verbose build
verbose:
	$(MAKE) V=1 all

help:
	@echo "Hunter Protocol Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build all libraries and tools (default)"
	@echo "  libs     - Build libraries only"
	@echo "  tools    - Build tools only"
	@echo "  capstone - Build capstone daemon"
	@echo "  test     - Run all tests"
	@echo "  clean    - Remove build artifacts"
	@echo "  release  - Build without sanitizers"
	@echo "  help     - Show this message"
	@echo ""
	@echo "Compiler Flags:"
	@echo "  CFLAGS  = $(CFLAGS)"
	@echo "  LDFLAGS = $(LDFLAGS)"
