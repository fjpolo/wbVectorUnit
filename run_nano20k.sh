#!/bin/bash

# --- Configuration ---
# Exit immediately if a command exits with a non-zero status.
set -e

# Define the root directory of your project (where this script is located)
PROJECT_ROOT="/mnt/c/Workspace/wbVectorUnit"

# Set PYTHONPATH to include the root of your LiteX and LiteX-Boards installations.
# This is crucial for Python to find 'litex.soc', 'litex_boards', etc.
# This line ensures that Python can locate modules like 'litex.soc.interconnect.wishbone_interface'.
export PYTHONPATH="${PROJECT_ROOT}/litex/litex:${PROJECT_ROOT}/litex/litex-boards${PYTHONPATH:+:$PYTHONPATH}"

# Path to your OSS CAD Suite environment script
OSS_CAD_SUITE_ENV_SCRIPT="${HOME}/oss-cad-suite/environment"

# Paths relative to PROJECT_ROOT
FIRMWARE_DIR="${PROJECT_ROOT}/firmware/demo"
LITEX_TARGETS_DIR="${PROJECT_ROOT}/litex/litex-boards/litex_boards/targets"
LITEX_BUILD_OUTPUT_DIR="${LITEX_TARGETS_DIR}/build/sipeed_tang_nano_20k"
GATEWARE_DIR="${LITEX_BUILD_OUTPUT_DIR}/gateware"
BOOTROM_BIN_DEST="${LITEX_TARGETS_DIR}/bootrom.bin" # Where LiteX expects bootrom.bin

# Define the name of the generated bitstream file
BITSTREAM_FILENAME="sipeed_tang_nano_20k.fs"

# Define the log file for LiteX build output
LITEX_BUILD_LOG="${PROJECT_ROOT}/build.log"

# --- Script Start ---
echo "--- Starting LiteX Build and Flash Script ---"
echo "Project Root: ${PROJECT_ROOT}"

# 1. Activate OSS CAD Suite environment
echo "1. Activating OSS CAD Suite environment..."
if [ -f "${OSS_CAD_SUITE_ENV_SCRIPT}" ]; then
    source "${OSS_CAD_SUITE_ENV_SCRIPT}"
    echo "   Environment activated successfully."
else
    echo "ERROR: OSS CAD Suite environment script not found at ${OSS_CAD_SUITE_ENV_SCRIPT}"
    echo "Please ensure the path is correct."
    exit 1
fi

# 2. Compile Firmware Application
echo "2. Compiling firmware application in ${FIRMWARE_DIR}..."
cd "${FIRMWARE_DIR}"
make clean # Always clean before building to avoid stale artifacts
make all
if [ $? -ne 0 ]; then
    echo "ERROR: Firmware compilation failed."
    exit 1
fi
echo "   Firmware compiled successfully."

# 3. Copy compiled firmware to LiteX's expected bootrom location
echo "3. Copying demo.bin to ${BOOTROM_BIN_DEST}..."
cp "${FIRMWARE_DIR}/demo.bin" "${BOOTROM_BIN_DEST}"
if [ $? -ne 0 ]; then
    echo "ERROR: Failed to copy demo.bin to bootrom.bin."
    exit 1
fi
echo "   Firmware copied successfully."

# 4. Run LiteX Build (Hardware Synthesis and Bitstream Generation)
echo "4. Running LiteX build (this may take a while)..."
echo "   Full log will be saved to ${LITEX_BUILD_LOG}"
cd "${LITEX_TARGETS_DIR}"
python3 sipeed_tang_nano_20k.py \
    --build \
    --toolchain=apicula \
    --cpu-type=serv \
    --bus-standard=wishbone \
    --integrated-rom-size 32768 \
    --csr-csv csr.csv \
    | tee "${LITEX_BUILD_LOG}"
if [ $? -ne 0 ]; then
    echo "ERROR: LiteX build failed. Check ${LITEX_BUILD_LOG} for details."
    exit 1
fi
echo "   LiteX build completed successfully."

# 5. Flash Bitstream to Tang Nano 20K
echo "5. Flashing bitstream to Tang Nano 20K..."
echo "   Ensure your board is connected and recognized by openFPGALoader."
cd "${GATEWARE_DIR}"
sudo openFPGALoader -b tangnano20k -f "${BITSTREAM_FILENAME}" -m flash --verbose
if [ $? -ne 0 ]; then
    echo "ERROR: openFPGALoader failed. Check board connection and permissions."
    exit 1
fi
echo "   Bitstream flashed successfully."

# --- Script End ---
echo "--- Script finished. Connect to serial console to see firmware output. ---"
cd "${PROJECT_ROOT}" # Return to the project root