.PHONY: help configure build debug release run test clean rebuild

BUILD_DIR = build
APP_NAME = smtp-server

ifeq ($(OS),Windows_NT)
	GENERATOR = -G "Visual Studio 18 2026" -A x64
	VCPKG_ARGS = -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static
	CONFIG_ARG = --config Release
	DEBUG_CONFIG_ARG = --config Debug
	CTEST_CONFIG_ARG = -C Debug
	RUN_EXE = $(BUILD_DIR)/Release/$(APP_NAME).exe
else
	GENERATOR =
	VCPKG_ARGS =
	CONFIG_ARG =
	DEBUG_CONFIG_ARG =
	CTEST_CONFIG_ARG =
	RUN_EXE = ./$(BUILD_DIR)/$(APP_NAME)
endif

help:
	@echo Commands:
	@echo   make configure
	@echo   make build
	@echo   make debug
	@echo   make release
	@echo   make run
	@echo   make test
	@echo   make clean
	@echo   make rebuild

configure:
	cmake -S . -B $(BUILD_DIR) $(GENERATOR) $(VCPKG_ARGS) -DCMAKE_BUILD_TYPE=Release

build:
	cmake --build $(BUILD_DIR) $(CONFIG_ARG)

release: build

debug:
	cmake --build $(BUILD_DIR) $(DEBUG_CONFIG_ARG)

run: build
	$(RUN_EXE)

test: debug
	ctest --test-dir $(BUILD_DIR) $(CTEST_CONFIG_ARG) --output-on-failure

clean:
	cmake -E remove_directory $(BUILD_DIR)

rebuild: clean configure build
