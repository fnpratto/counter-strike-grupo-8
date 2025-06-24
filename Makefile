.PHONY: all test clean editor client common server build gui tui default install-deps install-path fish

default: install-deps gui install-path

fish: install-deps gui install-path-fish

install-deps:
	@echo "Checking and installing missing dependencies..."
	@for pkg in \
		libssl-dev \
		cmake \
		libopusfile-dev \
		libxmp-dev \
		fluidsynth \
		libfluidsynth-dev \
		libwavpack-dev \
		libfreetype-dev \
		qtbase5-dev \
		qtbase5-dev-tools \
		qt5-qmake; do \
			dpkg -s $$pkg >/dev/null 2>&1 || sudo apt install -y $$pkg; \
	done

export PATH := $(HOME)/.local/bin:$(PATH)

install-path:
	@echo "Installing to ~/.local/bin"
	@mkdir -p ~/.local/bin
	@[ ! -L ~/.local/bin/taller_client ] && ln -s "$(realpath ./build/taller_client)" ~/.local/bin/taller_client || true
	@[ ! -L ~/.local/bin/taller_server ] && ln -s "$(realpath ./build/taller_server)" ~/.local/bin/taller_server || true
	@[ ! -L ~/.local/bin/taller_editor ] && ln -s "$(realpath ./build/taller_editor)" ~/.local/bin/taller_editor || true
	@grep -qxF 'export PATH="$$HOME/.local/bin:$$PATH"' ~/.bashrc || \
	echo 'export PATH="$$HOME/.local/bin:$$PATH"' >> ~/.bashrc
	@echo "Added ~/.local/bin to PATH in ~/.bashrc (if not already)"
	@echo "Please restart your shell or run 'source ~/.bashrc' to use the commands"

install-path-fish:
	@echo "Installing to ~/.local/bin for fish shell"
	@mkdir -p ~/.local/bin
	@mkdir -p ~/.config/fish
	@[ ! -L ~/.local/bin/taller_client ] && ln -s "$(realpath ./build/taller_client)" ~/.local/bin/taller_client || true
	@[ ! -L ~/.local/bin/taller_server ] && ln -s "$(realpath ./build/taller_server)" ~/.local/bin/taller_server || true
	@[ ! -L ~/.local/bin/taller_editor ] && ln -s "$(realpath ./build/taller_editor)" ~/.local/bin/taller_editor || true
	@grep -qxF 'set -gx PATH $$HOME/.local/bin $$PATH' ~/.config/fish/config.fish 2>/dev/null || \
	echo 'set -gx PATH $$HOME/.local/bin $$PATH' >> ~/.config/fish/config.fish
	@echo "Added ~/.local/bin to PATH in ~/.config/fish/config.fish (if not already)"
	@echo "Please restart your fish shell or run 'source ~/.config/fish/config.fish' to use the commands"

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
