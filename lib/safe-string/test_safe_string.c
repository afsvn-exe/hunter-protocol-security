/**
 * safe-string: Test Suite
 * Hunter Protocol - Week 1
 * 
 * Test categories:
 * 1. Happy path - normal usage
 * 2. Edge cases - boundaries and limits
 * 3. Error cases - invalid inputs
 * 4. Security cases - malicious inputs
 */

#include "safe_string.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Test counters */
static int tests_run = 0;
static int tests_passed = 0;

/* Test macros */
#define TEST(name) \
    do { \
        tests_run++; \
        printf("  [TEST] %s... ", #name); \
        fflush(stdout); \
    } while(0)

#define PASS() \
    do { \
        tests_passed++; \
        printf("PASS\n"); \
    } while(0)

#define FAIL(msg) \
    do { \
        printf("FAIL: %s\n", msg); \
    } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            FAIL(msg); \
            return; \
        } \
    } while(0)

#define ASSERT_STR_EQ(a, b, msg) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            FAIL(msg); \
            return; \
        } \
    } while(0)

/* ============================================================
 * ss_copy tests
 * ============================================================ */

static void test_ss_copy_basic(void) {
    TEST(ss_copy_basic);
    
    char dest[32];
    ssize_t result = ss_copy(dest, sizeof(dest), "hello");
    
    ASSERT_EQ(result, 5, "should return length");
    ASSERT_STR_EQ(dest, "hello", "should copy string");
    PASS();
}

static void test_ss_copy_exact_fit(void) {
    TEST(ss_copy_exact_fit);
    
    char dest[6]; // Exactly fits "hello" + NUL
    ssize_t result = ss_copy(dest, sizeof(dest), "hello");
    
    ASSERT_EQ(result, 5, "should return length");
    ASSERT_STR_EQ(dest, "hello", "should copy string");
    PASS();
}

static void test_ss_copy_truncation(void) {
    TEST(ss_copy_truncation);
    
    char dest[4]; // Only fits "hel" + NUL
    ssize_t result = ss_copy(dest, sizeof(dest), "hello");
    (void)result;     
    // TODO: What should result be for truncation?
    // ASSERT_EQ(result, ???, "should indicate truncation");
    ASSERT_STR_EQ(dest, "hel", "should truncate correctly");
    ASSERT_EQ(dest[3], '\0', "should NUL-terminate");
    PASS();
}

static void test_ss_copy_null_dest(void) {
    TEST(ss_copy_null_dest);
    
    ssize_t result = ss_copy(NULL, 32, "hello");
    ASSERT_EQ(result, SS_ENULL, "should return SS_ENULL");
    PASS();
}

static void test_ss_copy_null_src(void) {
    TEST(ss_copy_null_src);
    
    char dest[32];
    ssize_t result = ss_copy(dest, sizeof(dest), NULL);
    ASSERT_EQ(result, SS_ENULL, "should return SS_ENULL");
    PASS();
}

static void test_ss_copy_zero_size(void) {
    TEST(ss_copy_zero_size);
    
    char dest[32] = "unchanged";
    ssize_t result = ss_copy(dest, 0, "hello");
    
    ASSERT_EQ(result, SS_ESIZE, "should return SS_ESIZE");
    ASSERT_STR_EQ(dest, "unchanged", "should not modify dest");
    PASS();
}

static void test_ss_copy_empty_src(void) {
    TEST(ss_copy_empty_src);
    
    char dest[32] = "garbage";
    ssize_t result = ss_copy(dest, sizeof(dest), "");
    
    ASSERT_EQ(result, 0, "should return 0");
    ASSERT_STR_EQ(dest, "", "should be empty string");
    PASS();
}

static void test_ss_copy_size_one(void) {
    TEST(ss_copy_size_one);
    
    char dest[1];
    ssize_t result = ss_copy(dest, 1, "hello");
    (void)result;
    // With size 1, can only fit NUL
    ASSERT_STR_EQ(dest, "", "should be empty");
    // TODO: Should this indicate truncation?
    PASS();
}

/* ============================================================
 * ss_concat tests - TODO: Add your tests
 * ============================================================ */

static void test_ss_concat_basic(void) {
    TEST(ss_concat_basic);
    
    char dest[32] = "hello";
    ssize_t result = ss_concat(dest, sizeof(dest), " world");
    
    ASSERT_EQ(result, 11, "should return total length");
    ASSERT_STR_EQ(dest, "hello world", "should concatenate");
    PASS();
}

/* TODO: Add more ss_concat tests:
 * - Truncation during concat
 * - Concat to empty string
 * - Concat empty string
 * - NULL inputs
 * - Zero size
 * - Dest not terminated (security case!)
 */

/* ============================================================
 * ss_length tests - TODO: Add your tests
 * ============================================================ */

static void test_ss_length_basic(void) {
    TEST(ss_length_basic);
    
    ssize_t result = ss_length("hello", 100);
    ASSERT_EQ(result, 5, "should return length");
    PASS();
}

/* TODO: Add more ss_length tests:
 * - String longer than maxlen
 * - Empty string
 * - NULL string
 * - maxlen = 0
 */

static void test_ss_length_exceeds_max(void){
    TEST(ss_length_exceeds_max);

    ssize_t result = ss_length("helloworld", 5);
    ASSERT_EQ(result, 5, "should return maxlen when string is longer");
    PASS(); 
}

static void test_ss_length_empty(void){
    TEST(ss_length_empty); 

    ssize_t result = ss_length("", 100);
    ASSERT_EQ(result, 0, "empty string has length 0");
    PASS();
}

static void test_ss_length_null(void){
    TEST(ss_length_null);

    ssize_t result = ss_length(NULL, 100);
    ASSERT_EQ(result, SS_ENULL, "should return SS_ENULL for NULL");
    PASS(); 
}

static void test_ss_length_zero_max(void){
    TEST(ss_length_zero_max);

    size_t result = ss_length("hello", 0);
    ASSERT_EQ(result, 0, "should return 0 when maxlen is 0");
    PASS();
}

/* ============================================================
 * ss_format tests - TODO: Add your tests
 * ============================================================ */

static void test_ss_format_basic(void) {
    TEST(ss_format_basic);
    
    char dest[32];
    ssize_t result = ss_format(dest, sizeof(dest), "value: %d", 42);
    
    ASSERT_EQ(result, 9, "should return length");
    ASSERT_STR_EQ(dest, "value: 42", "should format correctly");
    PASS();
}

/* TODO: Add more ss_format tests:
 * - Truncation
 * - NULL inputs
 * - Complex format strings
 * - %s with NULL string (security case!)
 */

/* ============================================================
 * ss_compare tests - TODO: Add your tests
 * ============================================================ */

static void test_ss_compare_equal(void) {
    TEST(ss_compare_equal);
    
    int result = ss_compare("hello", "hello", 100);
    ASSERT_EQ(result, 0, "should return 0 for equal strings");
    PASS();
}

/* TODO: Add more ss_compare tests */

/* ============================================================
 * Main test runner
 * ============================================================ */

int main(void) {
    printf("\n=== safe-string Test Suite ===\n\n");
    
    printf("ss_copy:\n");
    test_ss_copy_basic();
    test_ss_copy_exact_fit();
    test_ss_copy_truncation();
    test_ss_copy_null_dest();
    test_ss_copy_null_src();
    test_ss_copy_zero_size();
    test_ss_copy_empty_src();
    test_ss_copy_size_one();
    
    printf("\nss_concat:\n");
    test_ss_concat_basic();
    
    printf("\nss_length:\n");
    test_ss_length_basic();
    test_ss_length_exceeds_max();
    test_ss_length_empty();
    test_ss_length_null();
    test_ss_length_zero_max();
    
    printf("\nss_format:\n");
    test_ss_format_basic();
    
    printf("\nss_compare:\n");
    test_ss_compare_equal();
    
    printf("\n=== Results: %d/%d tests passed ===\n\n", 
           tests_passed, tests_run);
    
    return (tests_passed == tests_run) ? 0 : 1;
}
