# safe-string Security Analysis

## Threat Model

### Assets Protected
- Destination buffer integrity
- Adjacent memory (stack/heap)
- Control flow (return addresses)

### Attacker Capabilities
- Controls source string content
- Controls source string length
- May provide unterminated strings
- May provide NULL pointers

### Attack Vectors Mitigated

| Vector | Defense |
|--------|---------|
| Buffer overflow via long input | Explicit size parameter, bounded copy |
| Missing NUL terminator | Always NUL-terminate, even on truncation |
| NULL pointer dereference | Check all pointers before use |
| Integer overflow in size calc | Validate sizes, check for wraparound |

## Security Guarantees

### G1: Bounded Writes
No function writes beyond `dest[destsize-1]`. Period.

**Verification:** ASan testing with destsize at allocation boundary.

### G2: Always Terminated
After any successful call, `dest` is a valid C string (NUL-terminated).

**Verification:** Test cases verify terminal NUL after truncation.

### G3: Truncation Detection  
Callers can always detect if truncation occurred via return value.

**Verification:** Test cases with inputs longer than destination.

### G4: NULL Safety
NULL pointer arguments return error, never crash.

**Verification:** Test cases with NULL for all pointer parameters.

## What This Library Does NOT Protect Against

- Use-after-free of source or destination buffers
- Double-free
- Race conditions on shared buffers
- Information disclosure via uninitialized memory

These require different defenses (arena allocator, synchronization, etc.).

## CWE Coverage

| CWE | Status | Notes |
|-----|--------|-------|
| CWE-120 | ✅ Mitigated | Bounded copies prevent overflow |
| CWE-121 | ✅ Mitigated | Stack buffer overflow prevented |
| CWE-122 | ✅ Mitigated | Heap buffer overflow prevented |
| CWE-126 | ✅ Mitigated | Over-read prevented by maxlen |
| CWE-170 | ✅ Mitigated | Always NUL-terminate |

## Testing Requirements

All code must pass:
- [x] AddressSanitizer
- [x] UndefinedBehaviorSanitizer
- [ ] Fuzzing with AFL/libFuzzer (TODO)
