BUILD_DIR ?= build
CONFIG ?= Debug

.PHONY: configure build clean

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(CONFIG)

build: configure
	cmake --build $(BUILD_DIR)

clean:
	cmake -E rm -rf $(BUILD_DIR)
