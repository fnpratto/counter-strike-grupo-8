.PHONY: all test clean editor client common server build gui tui default install-deps

default: install-deps gui

install-deps:
	@echo "Checking and installing missing dependencies..."
	@for pkg in \
		libopusfile-dev \
		libxmp-dev \
		fluidsynth \
		libfluidsynth-dev \
		libwavpack-dev \
		libfreetype6-dev \
		qtbase5-dev \
		qtbase5-dev-tools \
		qt5-qmake; do \
			dpkg -s $$pkg >/dev/null 2>&1 || apt install -y $$pkg; \
	done

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

gui:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug -DUI_TYPE=gui $(EXTRA_GENERATE)
	cmake --build build/ $(EXTRA_COMPILE)

tui:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug -DUI_TYPE=tui $(EXTRA_GENERATE)
	cmake --build build/ $(EXTRA_COMPILE)

run-tests: compile-debug
	./build/taller_tests

test: run-tests

all: clean run-tests

clean:
	rm -Rf build/
