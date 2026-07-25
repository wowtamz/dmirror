.PHONY: all build test #declare recipies

all: build

build:
	cmake -S . -B build $(CMAKE_ARGS) \
		-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=bin
	cmake --build build --config Release

test: build
	ctest --test-dir build --output-on-failure
