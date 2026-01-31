# safe-string Library

> Week 1 Deliverable — CWE-120: Buffer Copy without Checking Size of Input

## Overview

A bounded string operations library that prevents buffer overflows by:
- Requiring explicit destination sizes
- Always NUL-terminating outputs  
- Reporting truncation via return values
- Handling NULL inputs gracefully

## API

```c
#include "safe_string.h"

// Bounded copy
ssize_t ss_copy(char *dest, size_t destsize, const char *src);

// Bounded concatenation
ssize_t ss_concat(char *dest, size_t destsize, const char *src);

// Safe length with maximum
ssize_t ss_length(const char *str, size_t maxlen);

// Safe formatted string
ssize_t ss_format(char *dest, size_t destsize, const char *fmt, ...);

// Safe comparison
int ss_compare(const char *s1, const char *s2, size_t maxlen);
```

## Return Values

| Value | Meaning |
|-------|---------|
| `>= 0` | Success, returns character count |
| `SS_ETRUNC (1)` | Success with truncation |
| `SS_ENULL (-1)` | NULL pointer argument |
| `SS_ESIZE (-2)` | Invalid size (0 or overflow) |
| `SS_EFMT (-3)` | Format string error |

## Usage Example

```c
char buffer[64];

// Safe copy with truncation handling
ssize_t len = ss_copy(buffer, sizeof(buffer), user_input);
if (len == SS_ETRUNC) {
    // Handle truncation - input was too long
    log_warning("Input truncated from user");
}

// Safe formatting
ss_format(buffer, sizeof(buffer), "User: %s, ID: %d", name, id);
```

## Build

```bash
make          # Build library
make test     # Run tests with ASan/UBSan
make valgrind # Run under valgrind
make clean    # Clean build artifacts
```

## Security Guarantees

See [SECURITY.md](SECURITY.md) for threat model and guarantees.
