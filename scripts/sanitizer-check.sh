#!/bin/bash
# Hunter Protocol: Sanitizer Verification
# Verifies all components pass ASan, UBSan, and optionally TSan

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

cd "$ROOT_DIR"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=========================================="
echo "Sanitizer Verification Suite"
echo "=========================================="

# Check for sanitizer support
check_sanitizer_support() {
    echo -n "Checking sanitizer support... "
    
    cat > /tmp/san_test.c << 'EOF'
int main(void) { return 0; }
EOF
    
    if gcc -fsanitize=address,undefined -o /tmp/san_test /tmp/san_test.c 2>/dev/null; then
        echo -e "${GREEN}OK${NC}"
        rm -f /tmp/san_test /tmp/san_test.c
        return 0
    else
        echo -e "${RED}FAILED${NC}"
        echo "Sanitizers not available. Install libasan and libubsan."
        rm -f /tmp/san_test.c
        return 1
    fi
}

run_asan_tests() {
    echo ""
    echo "=== AddressSanitizer Tests ==="
    
    export ASAN_OPTIONS="detect_leaks=1:abort_on_error=1"
    
    make clean > /dev/null 2>&1 || true
    make CFLAGS="-std=c17 -Wall -Wextra -Werror -g3 -O0 -fsanitize=address -fno-omit-frame-pointer" \
         LDFLAGS="-fsanitize=address" test
    
    echo -e "${GREEN}ASan: PASS${NC}"
}

run_ubsan_tests() {
    echo ""
    echo "=== UndefinedBehaviorSanitizer Tests ==="
    
    export UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1"
    
    make clean > /dev/null 2>&1 || true
    make CFLAGS="-std=c17 -Wall -Wextra -Werror -g3 -O0 -fsanitize=undefined -fno-omit-frame-pointer" \
         LDFLAGS="-fsanitize=undefined" test
    
    echo -e "${GREEN}UBSan: PASS${NC}"
}

run_tsan_tests() {
    echo ""
    echo "=== ThreadSanitizer Tests ==="
    echo -e "${YELLOW}(Skipped - enable for concurrent code)${NC}"
    
    # Uncomment for concurrent code:
    # export TSAN_OPTIONS="halt_on_error=1"
    # make clean > /dev/null 2>&1 || true
    # make CFLAGS="-std=c17 -Wall -Wextra -Werror -g3 -O0 -fsanitize=thread -fno-omit-frame-pointer" \
    #      LDFLAGS="-fsanitize=thread" test
}

# Main
check_sanitizer_support || exit 1

run_asan_tests
run_ubsan_tests
run_tsan_tests

echo ""
echo "=========================================="
echo -e "${GREEN}All sanitizer checks passed!${NC}"
echo "=========================================="
