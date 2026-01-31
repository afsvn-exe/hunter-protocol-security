# safe-string Library Makefile
# Hunter Protocol - Week 1

# Use parent's flags if set, otherwise define our own
CC ?= gcc
CFLAGS ?= -std=c17 -Wall -Wextra -Werror -pedantic \
          -Wconversion -Wsign-conversion -Wshadow \
          -g3 -O0 -fsanitize=address,undefined \
          -fno-omit-frame-pointer
LDFLAGS ?= -fsanitize=address,undefined

# Targets
LIB = libsafe_string.a
TEST = test_safe_string

# Sources
SRC = safe_string.c
OBJ = $(SRC:.c=.o)
TEST_SRC = test_safe_string.c

.PHONY: all clean test valgrind

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c safe_string.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST): $(TEST_SRC) $(LIB)
	$(CC) $(CFLAGS) $< -L. -lsafe_string $(LDFLAGS) -o $@

test: $(TEST)
	@echo "=== Running safe-string tests ==="
	./$(TEST)
	@echo "=== All tests passed ==="

valgrind: $(TEST)
	valgrind --leak-check=full --show-leak-kinds=all \
	         --track-origins=yes --error-exitcode=1 \
	         ./$(TEST)

clean:
	rm -f $(OBJ) $(LIB) $(TEST)

# Show what flags we're using
flags:
	@echo "CC     = $(CC)"
	@echo "CFLAGS = $(CFLAGS)"
