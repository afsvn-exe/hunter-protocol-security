# HUNTER PROTOCOL: Linux Systems Security

> *"From 'buffer overflows are dangerous' to understanding the exact stack offset where corruption occurs."*

A 12-week intensive curriculum building secure systems programming expertise through hands-on library and tool development.

---

## Repository Structure

```
hunter-protocol-security/
├── lib/                    # Security libraries (Weeks 1-3, 5-7, 9-10)
│   ├── safe-string/        # Week 1: Bounded string operations
│   ├── arena-allocator/    # Week 2: UAF-resistant allocation
│   ├── safe-integer/       # Week 3: Overflow-safe arithmetic
│   ├── priv-drop/          # Week 5: Privilege dropping
│   ├── seccomp-sandbox/    # Week 6: Syscall filtering
│   ├── fd-ops/             # Week 7: Race-resistant file ops
│   ├── signal-safe-log/    # Week 9: Async-signal-safe logging
│   └── secure-socket/      # Week 10: Hardened socket operations
│
├── tools/                  # Security analysis tools (Weeks 4, 8, 11)
│   ├── priv-audit/         # Week 4: Credential inspection
│   ├── race-detector/      # Week 8: TOCTOU pattern detection
│   └── protocol-fuzzer/    # Week 11: Protocol input fuzzing
│
├── capstone/               # Week 12
│   └── hardened-daemon/    # Integration of all techniques
│
├── docs/                   # Documentation and logs
│   ├── VULNERABILITY_LOG.md
│   ├── DEATH_LOG.md
│   └── weekly/             # Weekly progress notes
│
├── scripts/                # Build and test automation
│   ├── build-all.sh
│   ├── test-all.sh
│   └── sanitizer-check.sh
│
└── tests/                  # Shared test utilities
    └── test_utils.h
```

---

## Build Requirements

### Compiler & Tools
- GCC 11+ or Clang 14+
- GNU Make
- Valgrind
- GDB

### Sanitizers (Required)
All code must pass:
- AddressSanitizer (ASan)
- UndefinedBehaviorSanitizer (UBSan)
- ThreadSanitizer (TSan) for concurrent code

### Standard Compile Flags
```bash
CFLAGS = -std=c17 -Wall -Wextra -Werror -pedantic
CFLAGS += -Wconversion -Wsign-conversion -Wshadow
CFLAGS += -fstack-protector-strong -D_FORTIFY_SOURCE=2

# Debug build (default)
CFLAGS += -g3 -O0 -fsanitize=address,undefined

# Release build
CFLAGS_RELEASE = -O2 -DNDEBUG
```

---

## Quick Start

```bash
# Clone
git clone https://github.com/YOUR_USERNAME/hunter-protocol-security.git
cd hunter-protocol-security

# Build everything
make all

# Run all tests with sanitizers
make test

# Build specific library
cd lib/safe-string && make

# Run under valgrind
make valgrind
```

---

## Weekly Deliverables

| Week | Component | Directory | CWE Focus |
|------|-----------|-----------|-----------|
| 1 | safe-string | `lib/safe-string/` | CWE-120: Buffer Overflow |
| 2 | arena-allocator | `lib/arena-allocator/` | CWE-416: Use-After-Free |
| 3 | safe-integer | `lib/safe-integer/` | CWE-190: Integer Overflow |
| 4 | priv-audit | `tools/priv-audit/` | Credential Analysis |
| 5 | priv-drop | `lib/priv-drop/` | CWE-271: Privilege Drop Errors |
| 6 | seccomp-sandbox | `lib/seccomp-sandbox/` | Attack Surface Reduction |
| 7 | fd-ops | `lib/fd-ops/` | CWE-367: TOCTOU |
| 8 | race-detector | `tools/race-detector/` | TOCTOU Detection |
| 9 | signal-safe-log | `lib/signal-safe-log/` | CWE-479: Signal Safety |
| 10 | secure-socket | `lib/secure-socket/` | Network Security |
| 11 | protocol-fuzzer | `tools/protocol-fuzzer/` | CWE-20: Input Validation |
| 12 | hardened-daemon | `capstone/hardened-daemon/` | All |

---

## Commit Convention

```
[WEEK-N] component: description

Security rationale: Why this change improves security
CWE: CWE-XXX (if applicable)
Tested: ASan/UBSan/TSan status
```

Example:
```
[WEEK-1] safe-string: implement ss_copy with truncation detection

Security rationale: Prevents buffer overflow by enforcing destination
size limits and always NUL-terminating output.
CWE: CWE-120
Tested: ASan clean, UBSan clean
```

---

## Documentation Requirements

### Per-Library/Tool
Each component must have:
- `README.md` — API documentation and usage
- `SECURITY.md` — Threat model and security guarantees
- Inline comments explaining security decisions

### Logs (Updated Daily)
- `docs/VULNERABILITY_LOG.md` — Discovered vulnerabilities and fixes
- `docs/DEATH_LOG.md` — Bugs, confusion points, and lessons learned

---

## License

MIT License — See [LICENSE](LICENSE)

---

## Target

**NVIDIA Systems Security** — Every skill serves the goal.
