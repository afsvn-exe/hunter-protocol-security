# Hunter Protocol 2.0 — Week 1, Day 2 Notes

## Phase: MEMORY | Focus: Stack Layout & Safe String Operations

---

## 1. Stack Layout Mastery

### The Complete Picture

When a function with a 64-byte buffer is called on x86-64 with stack protection:

```
    Higher addresses (where main lives)
    ┌──────────────────┐
    │   return addr    │  offset 88-95  (8 bytes) ← attacker's target
    ├──────────────────┤
    │   saved RBP      │  offset 80-87  (8 bytes) ← frame pointer
    ├──────────────────┤
    │   alignment pad  │  offset 72-79  (8 bytes) ← compiler inserted
    ├──────────────────┤
    │   stack canary   │  offset 64-71  (8 bytes) ← defense mechanism
    ├──────────────────┤
    │                  │
    │   buffer[64]     │  offset 0-63   (64 bytes) ← your data
    │                  │
    └──────────────────┘
    Lower addresses (RSP points here)
```

### Key Numbers to Memorize

| Component | Size | Offset from buffer start |
|-----------|------|--------------------------|
| buffer[64] | 64 bytes | 0-63 |
| canary | 8 bytes | 64-71 |
| alignment padding | 8 bytes | 72-79 |
| saved RBP | 8 bytes | 80-87 |
| return address | 8 bytes | 88-95 |

**Total to overwrite return address: 96 bytes**

### Why These Components Exist

**Stack Canary:** Random value placed between buffer and saved registers. If overflow corrupts buffer, it corrupts canary first. Before function returns, compiler checks if canary changed. If yes → crash via `__stack_chk_fail`. Stops most naive overflows.

**Alignment Padding:** x86-64 ABI requires 16-byte stack alignment at `call` instruction. Compiler inserts padding to make math work. Not security—performance requirement.

**Saved RBP:** Previous function's frame pointer. Helps debuggers walk the stack. If corrupted, program behavior becomes unpredictable.

**Return Address:** Where CPU jumps when function ends. Attacker's ultimate target—control this, control execution.

### Attack Math

To hijack control flow:
- Fill 64 bytes (buffer)
- Corrupt 8 bytes (canary) — triggers detection
- Fill 8 bytes (padding)
- Corrupt 8 bytes (saved RBP)
- Write 8 bytes of attacker's chosen address

**64 + 8 + 8 + 8 + 8 = 96 bytes minimum payload**

But canary stops this unless attacker leaks canary value first.

---

## 2. ss_length Implementation

### The Problem with strlen

Standard `strlen` is dangerous:

```c
size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {  // No limit!
        len++;
    }
    return len;
}
```

If `s` has no NUL terminator, this reads forever until:
- It finds a random zero byte in memory
- It crashes by reading unmapped memory
- It exposes sensitive data from adjacent buffers

**Root cause:** `strlen` trusts the input to be well-formed.

### The Safe Version

```c
ssize_t ss_length(const char *str, size_t maxlen) {
    if (str == NULL) {
        return SS_ENULL;
    }
    
    size_t count = 0;
    while (count < maxlen && str[count] != '\0') {
        count++;
    }
    return count;
}
```

**Security properties:**
- NULL input → error code, not crash
- Reads at most `maxlen` bytes, never more
- Unterminated string → returns `maxlen`, not infinite loop

### Critical: Condition Order Matters

```c
// CORRECT — bounds check FIRST
while (count < maxlen && str[count] != '\0')

// WRONG — dereference FIRST
while (str[count] != '\0' && count < maxlen)
```

**Why it matters:**

C evaluates `&&` left-to-right with short-circuit evaluation:
- If first condition is false, second is never evaluated

With correct order:
1. `count` reaches `maxlen`
2. `count < maxlen` is false
3. `str[count]` is NEVER read
4. Safe—no out-of-bounds access

With wrong order:
1. `count` reaches `maxlen`
2. `str[count] != '\0'` is evaluated FIRST
3. `str[maxlen]` is read—OUT OF BOUNDS
4. Undefined behavior, potential vulnerability

**Rule: Always check bounds before dereferencing.**

---

## 3. ss_copy Implementation

### Why strcpy Is Dangerous

```c
char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++) != '\0');
    return ret;
}
```

No size parameter. If `src` is longer than `dest`, overflow occurs.

**CWE-120: Buffer Copy without Checking Size of Input**

### The Safe Version

```c
ssize_t ss_copy(char *dest, size_t destsize, const char *src) {
    // Input validation
    if (dest == NULL || src == NULL) {
        return SS_ENULL;
    }
    if (destsize == 0) {
        return SS_ESIZE;
    }
    
    // Bounded copy
    size_t i = 0;
    while (i < destsize - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    
    // Always NUL-terminate
    dest[i] = '\0';
    
    // Detect truncation
    if (src[i] != '\0') {
        return SS_ETRUNC;
    }
    
    return i;
}
```

### Security Properties

1. **NULL-safe:** Returns error code instead of crashing on NULL
2. **Bounded:** Never writes beyond `destsize` bytes
3. **Always terminates:** `dest` is always a valid C string after call
4. **Truncation detection:** Caller knows if data was lost

### Why destsize - 1?

If `destsize` is 10, you have positions 0-9.
Position 9 must hold NUL terminator.
So you can only copy characters into positions 0-8.
That's 9 characters max = `destsize - 1`.

### The NUL Termination Requirement

The loop does NOT copy the NUL—it stops when it sees it:

```c
while (i < destsize - 1 && src[i] != '\0') {
    dest[i] = src[i];  // Only runs when src[i] is NOT '\0'
    i++;
}
```

So after the loop, you MUST add:

```c
dest[i] = '\0';
```

Without this, `dest` contains garbage after your copied characters.

### Truncation Detection Logic

After the loop exits, check why it stopped:

```c
if (src[i] != '\0') {
    return SS_ETRUNC;  // Buffer full, but source had more
}
return i;  // Source ended naturally, no truncation
```

If `src[i]` is NUL → we copied everything
If `src[i]` is NOT NUL → there was more to copy but no room

---

## 4. Error Handling Pattern

### Use Enums, Not Magic Numbers

**Bad:**
```c
return -1;  // What does -1 mean?
```

**Good:**
```c
return SS_ENULL;  // Clear: NULL pointer error
```

### Different Errors Need Different Codes

**Bad:**
```c
if (dest == NULL || src == NULL || destsize == 0) {
    return -1;  // Which error occurred?
}
```

**Good:**
```c
if (dest == NULL || src == NULL) {
    return SS_ENULL;  // Caller passed NULL
}
if (destsize == 0) {
    return SS_ESIZE;  // Caller passed invalid size
}
```

Caller can now distinguish between "I passed NULL" vs "I passed zero size."

### Return Types Matter

`ss_copy` returns `ssize_t` (signed integer), not a pointer.

**Wrong:**
```c
return NULL;  // NULL is a pointer, not a number
```

**Right:**
```c
return SS_ENULL;  // -1, a number
```

---

## 5. Gaps Identified & Addressed

### Gap 1: Stack Layout Recall

**Symptom:** Couldn't reconstruct what sits between buffer and return address.

**Fix:** Memorize the table. Draw it from memory until automatic:
- buffer → canary → padding → RBP → return addr
- 64 + 8 + 8 + 8 + 8 = 96

### Gap 2: Condition Order in Loops

**Symptom:** Wrote `str[count] != '\0' && count < maxlen` (wrong order).

**Fix:** Mental model—"Check the lock before opening the door."
- Bounds check = "Am I allowed to access this index?"
- Dereference = "Now I'll read/write that index."
- Always ask permission before acting.

### Gap 3: Return Type Confusion

**Symptom:** Returned `NULL` from a function that returns `ssize_t`.

**Fix:** Always check the function signature before writing return statements.
- `ssize_t` → return numbers (including negative error codes)
- Pointer type → return addresses or NULL

### Gap 4: Error Code Consistency

**Symptom:** Used `-1` instead of `SS_ENULL`, used same code for different errors.

**Fix:** The enum exists for a reason. Each error condition gets its own code.
- Read the enum definitions
- Match error type to error code
- Never use raw numbers

### Gap 5: Truncation Detection Concept

**Symptom:** Wasn't sure how to detect truncation.

**Fix:** Think about the loop exit conditions:
- Loop stops when `i == destsize - 1` OR `src[i] == '\0'`
- After loop, `src[i]` tells you which happened
- If `src[i] != '\0'`, there was more—truncation occurred

---

## 6. Test Design Framework

### Input Space Analysis

For any function, categorize each parameter:

```
Parameter type    Categories to test
─────────────────────────────────────
Pointer           NULL, valid
Size              0, 1, small, exact fit, large
String            "", short, long, unterminated
```

### Boundary Testing

Test where behavior changes:
- One below boundary
- Exactly at boundary
- One above boundary

For `ss_copy` with `destsize = 6`:
- "hello" (5 chars) → exact fit
- "hell" (4 chars) → room to spare  
- "hello!" (6 chars) → truncation

### Test Structure Pattern

```c
static void test_function_case(void) {
    TEST(function_case);
    
    // 1. Setup
    char dest[SIZE];
    
    // 2. Execute
    result = function_under_test(inputs);
    
    // 3. Verify
    ASSERT_EQ(result, expected_result, "message");
    ASSERT_STR_EQ(dest, expected_string, "message");
    
    PASS();
}
```

---

## 7. Day 2 Deliverables

- [x] `ss_length` implemented and tested (5 tests)
- [x] `ss_copy` implemented and tested (8 tests)
- [x] 13/16 total tests passing
- [x] ASan/UBSan clean compilation

**Remaining for Week 1:**
- [ ] `ss_concat`
- [ ] `ss_format`
- [ ] `ss_compare`

---

## 8. Commands Reference

### Compile with Sanitizers
```bash
gcc -Wall -Wextra -Werror -fsanitize=address,undefined -o test_safe_string test_safe_string.c safe_string.c
```

### Run Tests
```bash
./test_safe_string
```

### Git Commit Pattern
```bash
git add safe_string.c test_safe_string.c
git commit -m "feat(safe-string): implement ss_copy with bounds checking

- Validates dest, src for NULL (returns SS_ENULL)
- Validates destsize != 0 (returns SS_ESIZE)  
- Copies at most destsize-1 characters
- Always NUL-terminates
- Detects truncation (returns SS_ETRUNC)

Security: Prevents CWE-120 (Buffer Copy without Checking Size of Input)"
```

---

## 9. Tomorrow's Preview

**ss_concat** builds on `ss_copy`:
1. Find end of existing string in `dest`
2. Calculate remaining space
3. Copy `src` into remaining space
4. Handle truncation

Think about: What if `dest` isn't NUL-terminated when passed in?
