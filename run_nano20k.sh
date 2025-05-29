# !/bin/bash

# 
source ~/oss-cad-suite && ./litex/litex-boards/litex_boards/targets/sipeed_tang_nano_20k.py --build --toolchain=apicula --cpu-type=serv --bus-standard=wishbone && sudo $(which python3) ./litex/litex-boards/litex_boards/targets/sipeed_tang_nano_20k.py --load