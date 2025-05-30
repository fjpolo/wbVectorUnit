# !/bin/bash

# Set PYTHONPATH to include the parent directory of litex_boards
export PYTHONPATH="${PWD}/litex/litex-boards${PYTHONPATH:+:$PYTHONPATH}"
#
ROOT_DIR=${PWD}
DEMO_PATH=${PWD}/demo/demo.bin
# DEMO_PATH=${PWD}/firmware/demo/demo.bin

# Fw application
cd firmware/demo/ && make && cd ${ROOT_DIR} && cp firmware/demo/demo.bin . && mv demo.bin bootrom.bin

# LiteX
# source ~/oss-cad-suite/environment && cd litex/litex-boards/litex_boards/targets/ && ./sipeed_tang_nano_20k.py --flash --build --toolchain=apicula --cpu-type=serv --bus-standard=wishbone && cd ${PWD}/build/sipeed_tang_nano_20k/gateware/ && sudo openFPGALoader -b tangnano20k -f sipeed_tang_nano_20k.fs -m flash --verbose && cd ${ROOT_DIR}
# sudo openFPGALoader -b tangnano20k -f sipeed_tang_nano_20k.fs -m flash --verbose
# source ~/oss-cad-suite/environment && ./litex/litex-boards/litex_boards/targets/sipeed_tang_nano_20k.py --build --toolchain=apicula --cpu-type=serv --bus-standard=wishbone && cd ${PWD}/build/sipeed_tang_nano_20k/gateware/ && sudo openFPGALoader -b tangnano20k -f sipeed_tang_nano_20k.fs -m flash --verbose && cd ${ROOT_DIR}
# source ~/oss-cad-suite/environment && ./litex/litex-boards/litex_boards/targets/sipeed_tang_nano_20k.py --build --integrated-rom-init=${DEMO_PATH} --toolchain=apicula --cpu-type=serv --bus-standard=wishbone && cd ${PWD}/build/sipeed_tang_nano_20k/gateware/ && sudo openFPGALoader -b tangnano20k -f sipeed_tang_nano_20k.fs -m flash --verbose && cd ${ROOT_DIR}
# source ~/oss-cad-suite/environment && ./litex/litex-boards/litex_boards/targets/sipeed_tang_nano_20k.py --build --integrated-rom-init=${DEMO_PATH} --toolchain=apicula --cpu-type=serv --bus-standard=wishbone && cd ${PWD}/build/sipeed_tang_nano_20k/gateware/ && sudo openFPGALoader -b tangnano20k -f sipeed_tang_nano_20k.fs -m flash --verbose && cd ${ROOT_DIR}
source ~/oss-cad-suite/environment && ./litex/litex-boards/litex_boards/targets/sipeed_tang_nano_20k.py --build --toolchain=apicula --cpu-type=serv --bus-standard=wishbone && cd ${PWD}/build/sipeed_tang_nano_20k/gateware/ && sudo openFPGALoader -b tangnano20k -f sipeed_tang_nano_20k.fs -m flash --verbose && cd ${ROOT_DIR}

# # Flash app to SPIFlash
# python3 -m litex.soc.software.crcfbigen firmware/demo/demo.bin -o firmware/demo/demo.fbi --fbi --little
# sudo openFPGALoader -b tangnano20k -f ${PWD}/firmware/demo/demo.fbi --offset 0x700000


###

# # Fw application
# cd firmware/demo/ && make && cd ../..
# # Use the LiteX crcfbigen tool to create the flash-bootable binary (.fbi)
# # We assume your CPU (RISC-V) is little-endian, which is typical for LiteX.
# python3 -m litex.soc.software.crcfbigen firmware/demo/demo.bin -o firmware/demo/demo.fbi --fbi --little

# Build demo
# litex_bare_metal_demo --build-path=/mnt/c/Workspace/wbVectorUnit/build/sipeed_tang_nano_20k

# # Ensure openFPGALoader is ready (permissions handled as discussed earlier)
# # sudo openFPGALoader -b tangnano20k -f ${PWD}/firmware/demo/demo.fbi --offset 0x40000
# sudo openFPGALoader -b tangnano20k -f ${PWD}/firmware/demo/demo.fbi --offset 0x700000
# # sudo openFPGALoader -b tangnano20k -f ${PWD}/firmware/demo/demo.fbi --offset 0x800000
