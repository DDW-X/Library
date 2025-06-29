#!/bin/bash

SRC_DIR="src"
BUILD_DIR="build/linux"
INCLUDE_DIR="include"
EXAMPLES_DIR="examples"
TESTS_DIR="tests"

mkdir -p "$BUILD_DIR"

# کامپایل کتابخانه
gcc -fPIC -shared -O3 -flto -march=native -I"$INCLUDE_DIR" \
    "$SRC_DIR/perf_monitor.c" \
    "$SRC_DIR/hw_manager.c" \
    "$SRC_DIR/sys_controller.c" \
    "$SRC_DIR/security_layer.c" \
    "$SRC_DIR/omnicontrol.c" \
    -o "$BUILD_DIR/libomnicontrol.so" \
    -lpci -lssl -lcrypto -lpthread

# کامپایل مثال‌ها
gcc -O3 -I"$INCLUDE_DIR" -L"$BUILD_DIR" -lomnicontrol \
    "$EXAMPLES_DIR/system_monitor.c" \
    -o "$BUILD_DIR/system_monitor"

# کامپایل تست‌ها
gcc -O3 -I"$INCLUDE_DIR" -L"$BUILD_DIR" -lomnicontrol \
    "$TESTS_DIR/unit_tests.c" \
    -o "$BUILD_DIR/unit_tests" \
    -lpci -lssl -lcrypto -lpthread

echo "Build completed. Output:"
echo "  Library: $BUILD_DIR/libomnicontrol.so"
echo "  Examples: $BUILD_DIR/system_monitor"
echo "  Tests: $BUILD_DIR/unit_tests"