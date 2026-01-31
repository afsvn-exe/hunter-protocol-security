#!/bin/bash
# Hunter Protocol: Build All Components
# Usage: ./scripts/build-all.sh [debug|release]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

cd "$ROOT_DIR"

BUILD_TYPE="${1:-debug}"

echo "=========================================="
echo "Hunter Protocol Build"
echo "Build Type: $BUILD_TYPE"
echo "=========================================="

if [ "$BUILD_TYPE" = "release" ]; then
    make release
else
    make all
fi

echo ""
echo "Build complete!"
