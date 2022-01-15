#include "executor.hpp"
#include "gui.hpp"
#include "notemanager.hpp"

#include <iostream>

int main() {
	try {
		executor::init_executor();
		notemanager::init_manager();
		gui::init_gui();

		gui::render();
	} catch (std::exception& e) {
		printf_s("Fail with exception: %s\n", e.what());
		getchar();
	}
}
