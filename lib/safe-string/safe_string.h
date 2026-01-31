/**
 * safe-string: Bounded String Operations Library
 * Hunter Protocol - Week 1
 * 
 * CWE-120: Buffer Copy without Checking Size of Input
 * 
 * SECURITY GUARANTEE:
 * - No function writes beyond specified buffer size
 * - All outputs are NUL-terminated
 * - Truncation is detectable via return value
 * - NULL inputs handled gracefully
 */

#ifndef SAFE_STRING_H
#define SAFE_STRING_H

#include <stddef.h>
#include <sys/types.h>

/* Return codes */
typedef enum {
    SS_OK = 0,          /* Success, no truncation */
    SS_ETRUNC = 1,      /* Success, but output was truncated */
    SS_ENULL = -1,      /* NULL pointer argument */
    SS_ESIZE = -2,      /* Invalid size argument (0 or overflow) */
    SS_EFMT = -3        /* Format string error */
} ss_result_t;

/**
 * ss_copy - Bounded string copy with truncation detection
 * 
 * @dest:     Destination buffer
 * @destsize: Size of destination buffer (including NUL)
 * @src:      Source string (must be NUL-terminated)
 * 
 * Copies at most destsize-1 characters from src to dest.
 * Always NUL-terminates dest (unless destsize is 0).
 * 
 * RETURNS:
 *   >= 0: Number of characters copied (not including NUL)
 *   SS_ETRUNC: Truncation occurred (dest still valid)
 *   SS_ENULL: NULL pointer passed
 *   SS_ESIZE: destsize is 0
 * 
 * SECURITY:
 *   - Never writes beyond dest[destsize-1]
 *   - Safe if src is unterminated (reads at most destsize-1 chars)
 *     NOTE: This is a design decision - implement accordingly
 */
ssize_t ss_copy(char *dest, size_t destsize, const char *src);

/**
 * ss_concat - Bounded string concatenation
 * 
 * @dest:     Destination buffer (must contain valid string)
 * @destsize: Total size of destination buffer
 * @src:      String to append
 * 
 * Appends src to dest, ensuring total length < destsize.
 * 
 * RETURNS:
 *   >= 0: Total length of resulting string
 *   SS_ETRUNC: Truncation occurred
 *   SS_ENULL: NULL pointer passed
 *   SS_ESIZE: destsize is 0
 */
ssize_t ss_concat(char *dest, size_t destsize, const char *src);

/**
 * ss_length - Safe string length with maximum
 * 
 * @str:    String to measure
 * @maxlen: Maximum characters to scan
 * 
 * RETURNS:
 *   >= 0: Length of string (up to maxlen)
 *   SS_ENULL: str is NULL
 * 
 * SECURITY:
 *   - Never reads beyond str[maxlen-1]
 *   - Safe for unterminated strings
 */
ssize_t ss_length(const char *str, size_t maxlen);

/**
 * ss_format - Bounded formatted string (safe sprintf)
 * 
 * @dest:     Destination buffer
 * @destsize: Size of destination buffer
 * @fmt:      Format string
 * @...:      Format arguments
 * 
 * RETURNS:
 *   >= 0: Number of characters written (not including NUL)
 *   SS_ETRUNC: Output was truncated
 *   SS_ENULL: NULL pointer passed
 *   SS_ESIZE: destsize is 0
 *   SS_EFMT: Format error
 * 
 * SECURITY:
 *   - Never writes beyond destsize
 *   - Always NUL-terminates
 */
ssize_t ss_format(char *dest, size_t destsize, const char *fmt, ...)
    __attribute__((format(printf, 3, 4)));

/**
 * ss_compare - Safe string comparison with length limit
 * 
 * @s1:     First string
 * @s2:     Second string  
 * @maxlen: Maximum characters to compare
 * 
 * RETURNS:
 *   < 0: s1 < s2
 *   = 0: s1 == s2 (up to maxlen characters)
 *   > 0: s1 > s2
 *   SS_ENULL if either string is NULL
 */
int ss_compare(const char *s1, const char *s2, size_t maxlen);

#endif /* SAFE_STRING_H */
