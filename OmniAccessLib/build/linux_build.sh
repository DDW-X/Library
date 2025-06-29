// linux_build.sh - Placeholder content
#!/bin/bash

# Configuration
BUILD_DIR="build/linux"
SRC_DIR="src"
INCLUDE_DIR="include"
DRIVER_DIR="drivers/linux"
EXAMPLES_DIR="examples"
MODULE_NAME="omniaccess"

# Clean and create build directory
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$BUILD_DIR/lib"
mkdir -p "$BUILD_DIR/module"
mkdir -p "$BUILD_DIR/examples"

# Build the library
echo "Building OmniAccess library..."
gcc -fPIC -shared -O3 -flto -march=native -I"$INCLUDE_DIR" \
    "$SRC_DIR/core.c" \
    "$SRC_DIR/linux/physical.c" \
    "$SRC_DIR/linux/pci.c" \
    "$SRC_DIR/linux/msr.c" \
    "$SRC_DIR/common/acpi.c" \
    "$SRC_DIR/common/smm.c" \
    "$SRC_DIR/common/pci_common.c" \
    -o "$BUILD_DIR/lib/libomniaccess.so"

# Build kernel module
echo "Building kernel module..."
cd "$DRIVER_DIR"
make -j$(nproc) KERNELDIR=/lib/modules/$(uname -r)/build
cd -
cp "$DRIVER_DIR/$MODULE_NAME.ko" "$BUILD_DIR/module/"

# Build examples
echo "Building examples..."
gcc -O3 -flto -march=native -I"$INCLUDE_DIR" -L"$BUILD_DIR/lib" -lomniaccess \
    "$EXAMPLES_DIR/physical_read.c" \
    -o "$BUILD_DIR/examples/physical_read"

gcc -O3 -flto -march=native -I"$INCLUDE_DIR" -L"$BUILD_DIR/lib" -lomniaccess \
    "$EXAMPLES_DIR/pci_scan.c" \
    -o "$BUILD_DIR/examples/pci_scan"

gcc -O3 -flto -march=native -I"$INCLUDE_DIR" -L"$BUILD_DIR/lib" -lomniaccess \
    "$EXAMPLES_DIR/msr_dump.c" \
    -o "$BUILD_DIR/examples/msr_dump"

# Create installation script
cat > "$BUILD_DIR/install_module.sh" << 'EOF'
#!/bin/bash
MODULE_NAME="omniaccess"
MODULE_PATH=$(dirname "$0")/module/$MODULE_NAME.ko

if [ ! -f "$MODULE_PATH" ]; then
    echo "Error: Module not found at $MODULE_PATH"
    exit 1
fi

sudo cp "$MODULE_PATH" /lib/modules/$(uname -r)/kernel/drivers/
sudo depmod -a
sudo modprobe $MODULE_NAME

echo "Kernel module installed and loaded successfully"
EOF

chmod +x "$BUILD_DIR/install_module.sh"

# Create uninstallation script
cat > "$BUILD_DIR/uninstall_module.sh" << 'EOF'
#!/bin/bash
MODULE_NAME="omniaccess"

sudo modprobe -r $MODULE_NAME
sudo rm -f /lib/modules/$(uname -r)/kernel/drivers/$MODULE_NAME.ko
sudo depmod -a

echo "Kernel module uninstalled successfully"
EOF

chmod +x "$BUILD_DIR/uninstall_module.sh"

# Create library path setup script
cat > "$BUILD_DIR/setup_library.sh" << 'EOF'
#!/bin/bash
BUILD_DIR=$(dirname "$0")
LIB_PATH="$BUILD_DIR/lib"

if [ ! -d "$LIB_PATH" ]; then
    echo "Error: Library directory not found at $LIB_PATH"
    exit 1
fi

echo "Adding $LIB_PATH to LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$LIB_PATH:$LD_LIBRARY_PATH"
echo "To make permanent, add to your .bashrc:"
echo "export LD_LIBRARY_PATH=\"$LIB_PATH:\$LD_LIBRARY_PATH\""
EOF

chmod +x "$BUILD_DIR/setup_library.sh"

# Print summary
echo ""
echo "===================================================="
echo "Build completed successfully in $BUILD_DIR directory"
echo "===================================================="
echo ""
echo "Contents:"
echo "  lib/                 - Shared library"
echo "  module/              - Kernel module"
echo "  examples/            - Sample applications"
echo "  install_module.sh    - Module installation script"
echo "  uninstall_module.sh  - Module removal script"
echo "  setup_library.sh     - Library path setup"
echo ""
echo "To install module:"
echo "  sudo $BUILD_DIR/install_module.sh"
echo ""
echo "To run examples:"
echo "  . $BUILD_DIR/setup_library.sh"
echo "  $BUILD_DIR/examples/physical_read"
echo "===================================================="