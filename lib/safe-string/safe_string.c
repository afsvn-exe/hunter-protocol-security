/**
 * safe-string: Implementation
 * Hunter Protocol - Week 1
 * 
 * YOUR TASK: Implement each function following the security requirements
 * in the header file. Think about:
 * 
 * 1. What can be NULL?
 * 2. What sizes are invalid?
 * 3. What happens at boundaries?
 * 4. How do we detect/report truncation?
 * 5. Is the output ALWAYS valid after the call?
 */

#include "safe_string.h"
#include <stdarg.h>
#include <stdio.h>

ssize_t ss_copy(char *dest, size_t destsize, const char *src) {
    // TODO: Validate inputs
    //   - What if dest is NULL?
    //   - What if src is NULL?
    //   - What if destsize is 0?
    
    // TODO: Copy characters with bounds checking
    //   - How many characters can we safely copy?
    //   - When do we stop?
    
    // TODO: Ensure NUL termination
    //   - Where does the NUL go?
    //   - Is this always possible?
    
    // TODO: Detect and report truncation
    //   - How do we know if truncation occurred?
    //   - What return value indicates truncation?
    
    (void)dest;     // Remove when implemented
    (void)destsize; // Remove when implemented
    (void)src;      // Remove when implemented
    return SS_ENULL; // Placeholder
}

ssize_t ss_concat(char *dest, size_t destsize, const char *src) {
    // TODO: Validate inputs
    
    // TODO: Find end of existing string in dest
    //   - What if dest isn't properly terminated?
    //   - How far do we search?
    
    // TODO: Append src with bounds checking
    //   - How much space remains?
    //   - Can we reuse ss_copy here?
    
    // TODO: Handle truncation
    
    (void)dest;
    (void)destsize;
    (void)src;
    return SS_ENULL; // Placeholder
}

ssize_t ss_length(const char *str, size_t maxlen) {
    // TODO: Validate input
    
    // TODO: Count characters up to NUL or maxlen
    //   - What if there's no NUL within maxlen?
    
    (void)str;
    (void)maxlen;
    return SS_ENULL; // Placeholder
}

ssize_t ss_format(char *dest, size_t destsize, const char *fmt, ...) {
    // TODO: Validate inputs
    
    // TODO: Use vsnprintf for bounded formatting
    //   - What does vsnprintf return?
    //   - How do we detect truncation?
    //   - What if vsnprintf fails?
    
    // HINT: You'll need va_list, va_start, va_end
    
    (void)dest;
    (void)destsize;
    (void)fmt;
    return SS_ENULL; // Placeholder
}

int ss_compare(const char *s1, const char *s2, size_t maxlen) {
    // TODO: Validate inputs
    //   - What should we return for NULL?
    
    // TODO: Compare character by character
    //   - Stop at NUL, difference, or maxlen
    
    (void)s1;
    (void)s2;
    (void)maxlen;
    return SS_ENULL; // Placeholder
}
