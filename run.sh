# !/bin/bash

export PATH="/Users/fjpolo/oss-cad-suite/bin:$PATH"
export QTDIR=$(brew --prefix)/opt/qt@5
export QT_PLUGIN_PATH=$QTDIR/plugins
export PATH="$QTDIR/bin:$PATH"
export PKG_CONFIG_PATH=$QTDIR/lib/pkgconfig:$PKG_CONFIG_PATH # Essential for CMake to find Qt5
python3 -m venv venv && source venv/bin/activate && python3 -m pip install migen nmigen pyusb && python3 -m pip install -e litex/litex && python3 -m pip install -e litex/litedram && python3 -m pip3 install -e litex/liteeth && python3 -m pip3 install -e litex/litevideo && python3 -m pip3 install -e litex/litex-boards && python3 -m pip3 install -e litex/litespi
# python3 litex/litex-boards/litex_boards/targets/sipeed_tang_nano_9k.py --help
cd litex/litex-boards/litex_boards/targets/ && python3 sipeed_tang_nano_9k.py --build --toolchain=apicula --cpu-type=serv --bus-standard=wishbone