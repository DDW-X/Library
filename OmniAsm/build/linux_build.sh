#!/bin/bash

# ==============================================================================
# OmniAsm Linux Build Script
# Version: 3.0
# Author: DDW-X
# Date: 2025-07-01
# ==============================================================================

# تنظیمات پروژه
PROJECT_NAME="OmniAsm"
VERSION="3.0"
ARCH="x86_64"
BUILD_DIR="build/linux"
SRC_DIR="src/linux"
INCLUDE_DIR="include"
EXAMPLES_DIR="examples"
DEPLOY_DIR="deploy/${PROJECT_NAME}-${VERSION}-linux-${ARCH}"

# رنگ‌های ترمینال
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# تابع نمایش خطا
error() {
    echo -e "${RED}[ERROR]${NC} $1"
    exit 1
}

# تابع نمایش هشدار
warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# تابع نمایش موفقیت
success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

# بررسی وابستگی‌ها
check_dependencies() {
    local required=("gcc" "as" "make" "ld" "tar")
    local missing=()
    
    echo "Checking dependencies..."
    for cmd in "${required[@]}"; do
        if ! command -v "$cmd" &> /dev/null; then
            missing+=("$cmd")
        fi
    done
    
    if [ ${#missing[@]} -gt 0 ]; then
        error "Missing dependencies: ${missing[*]}\nInstall with: sudo apt-get install build-essential binutils"
    fi
}

# آماده‌سازی دایرکتوری‌ها
prepare_directories() {
    echo "Preparing directories..."
    
    # پاکسازی ساخت‌های قبلی
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR" || warning "Failed to clean build directory"
    fi
    
    if [ -d "$DEPLOY_DIR" ]; then
        rm -rf "$DEPLOY_DIR" || warning "Failed to clean deploy directory"
    fi
    
    # ایجاد دایرکتوری‌های جدید
    mkdir -p "$BUILD_DIR" || error "Failed to create build directory"
    mkdir -p "$DEPLOY_DIR/lib" || error "Failed to create lib directory"
    mkdir -p "$DEPLOY_DIR/include" || error "Failed to create include directory"
    mkdir -p "$DEPLOY_DIR/examples" || error "Failed to create examples directory"
}

# کامپایل ماژول اسمبلی
compile_asm_module() {
    echo "Compiling assembly module..."
    
    # کامپایل کد اسمبلی
    as --64 -I"$INCLUDE_DIR" -o "$BUILD_DIR/omniasm.o" "$SRC_DIR/omniasm.S" \
        || error "Assembly compilation failed"
    
    # ایجاد کتابخانه اشتراکی
    gcc -shared -fPIC -flto -O3 -march=native -o "$BUILD_DIR/libomniasm.so" \
        "$BUILD_DIR/omniasm.o" \
        || error "Shared library creation failed"
    
    # ایجاد کتابخانه استاتیک
    ar rcs "$BUILD_DIR/libomniasm.a" "$BUILD_DIR/omniasm.o" \
        || warning "Static library creation failed"
}

# کامپایل مثال‌ها
compile_examples() {
    echo "Compiling examples..."
    
    # مثال تست عملکرد
    gcc -O3 -flto -march=native -I"$INCLUDE_DIR" -L"$BUILD_DIR" -lomniasm \
        -o "$BUILD_DIR/test_perf" "$EXAMPLES_DIR/test_perf.c" \
        || error "Test performance compilation failed"
    
    # مثال کنترل مستقیم سخت‌افزار
    cat > "$BUILD_DIR/hardware_control.c" << 'EOF'
#include "omniasm.h"
#include <stdio.h>

int main() {
    // فعال‌سازی AVX
    omni_asm_enable_avx();
    printf("AVX extensions enabled\n");
    
    // خواندن TSC
    uint32_t eax, edx;
    omni_asm_rdtsc(&eax, &edx);
    uint64_t tsc = ((uint64_t)edx << 32) | eax;
    printf("TSC: 0x%016llX\n", tsc);
    
    // خواندن MSR
    uint64_t msr = omni_asm_read_msr(0x1B); // APIC_BASE
    printf("MSR 0x1B: 0x%016llX\n", msr);
    
    return 0;
}
EOF

    gcc -O3 -I"$INCLUDE_DIR" -L"$BUILD_DIR" -lomniasm \
        -o "$BUILD_DIR/hardware_control" "$BUILD_DIR/hardware_control.c" \
        || error "Hardware control example compilation failed"
}

# ایجاد اسکریپت‌های اجرایی
create_launch_scripts() {
    echo "Creating launch scripts..."
    
    # اسکریپت اجرای تست عملکرد
    cat > "$DEPLOY_DIR/run_test_perf.sh" << 'EOF'
#!/bin/bash
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
LD_LIBRARY_PATH="$SCRIPT_DIR/lib" "$SCRIPT_DIR/examples/test_perf"
EOF
    
    # اسکریپت اجرای کنترل سخت‌افزار
    cat > "$DEPLOY_DIR/run_hardware_control.sh" << 'EOF'
#!/bin/bash
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
LD_LIBRARY_PATH="$SCRIPT_DIR/lib" "$SCRIPT_DIR/examples/hardware_control"
EOF
    
    # اسکریپت تنظیم محیط
    cat > "$DEPLOY_DIR/setup_env.sh" << 'EOF'
#!/bin/bash
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
export LD_LIBRARY_PATH="$SCRIPT_DIR/lib:$LD_LIBRARY_PATH"
echo "Environment configured. Now you can run:"
echo "  ./examples/test_perf"
echo "  ./examples/hardware_control"
EOF
    
    # اعتبارسنجی اسکریپت‌ها
    chmod +x "$DEPLOY_DIR"/*.sh || warning "Failed to make scripts executable"
}

# بسته‌بندی برای انتشار
package_for_deployment() {
    echo "Packaging for deployment..."
    
    # کپی فایل‌های کتابخانه
    cp "$BUILD_DIR/libomniasm.so" "$DEPLOY_DIR/lib/" || error "Failed to copy shared library"
    cp "$BUILD_DIR/libomniasm.a" "$DEPLOY_DIR/lib/" || warning "Failed to copy static library"
    
    # کپی فایل‌های هدر
    cp "$INCLUDE_DIR/omniasm.h" "$DEPLOY_DIR/include/" || error "Failed to copy header"
    
    # کپی مثال‌ها
    cp "$BUILD_DIR/test_perf" "$DEPLOY_DIR/examples/" || error "Failed to copy test_perf"
    cp "$BUILD_DIR/hardware_control" "$DEPLOY_DIR/examples/" || error "Failed to copy hardware_control"
    
    # ایجاد فایل README
    cat > "$DEPLOY_DIR/README.md" << 'EOF'
# OmniAsm - Low-Level Hardware Access Module

## Overview
OmniAsm provides direct hardware access and control through highly optimized assembly routines. 
Key features include:
- Physical memory mapping
- MSR (Model-Specific Register) access
- Port I/O operations
- CPU feature control (SSE, AVX)
- Performance monitoring

## Quick Start

### Environment Setup
```bash
source setup_env.sh