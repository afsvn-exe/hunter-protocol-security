# DEATH LOG

> Every death is a lesson. Track bugs, confusion, and the adaptations that followed.

---

## Log Format

```
## [DATE] DEATH-NNN: Title

**Phase:** Build/Test/Debug/Understanding
**Time Lost:** How long stuck?
**Rage Level:** 🔥 to 🔥🔥🔥🔥🔥

### What Happened
Describe the failure

### What I Tried
List debugging attempts

### Root Cause
What was actually wrong?

### The Fix
How was it resolved?

### Adaptation
What will I do differently next time?
```

---

## Entries

<!-- Add new entries below, newest first -->

### Template Entry (Delete This)

## [YYYY-MM-DD] DEATH-001: Segfault in test but not in GDB

**Phase:** Debug
**Time Lost:** 45 minutes
**Rage Level:** 🔥🔥🔥

### What Happened
Program segfaults when run normally, but works fine under GDB.

### What I Tried
1. Added printf debugging - segfault moved
2. Ran under GDB - no crash
3. Suspected timing issue
4. Finally ran under ASan

### Root Cause
Heap buffer overflow. GDB changes memory layout, hiding the bug. ASan caught it immediately:
```
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x...
```

### The Fix
Off-by-one in loop bound: `i <= size` should be `i < size`

### Adaptation
**ALWAYS run ASan first before printf debugging.** If behavior changes under GDB, it's almost always memory corruption. ASan is faster than GDB for memory bugs.

---

## Death Statistics

| Week | Deaths | Total Time Lost | Top Killer |
|------|--------|-----------------|------------|
| 1    | 0      | 0h              | -          |
| 2    | 0      | 0h              | -          |
| ...  | ...    | ...             | ...        |

---

## Recurring Killers (Patterns to Watch)

1. **Off-by-one errors** — Always double-check loop bounds
2. **Uninitialized variables** — Compile with `-Wuninitialized`
3. **Sign conversion** — `size_t` vs `ssize_t` confusion
4. **NULL pointer dereference** — Check returns from malloc, fopen, etc.

---

<!-- Your entries go below -->
