#!/bin/bash
# Hunter Protocol: Run All Tests
# Runs tests with sanitizers enabled

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

cd "$ROOT_DIR"

echo "=========================================="
echo "Hunter Protocol Test Suite"
echo "Sanitizers: ASan + UBSan"
echo "=========================================="

# Build with sanitizers (default)
make all

# Run tests
make test

echo ""
echo "=========================================="
echo "All tests passed with sanitizers!"
echo "=========================================="
