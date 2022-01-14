#include "gui.hpp"
#include "notemanager.hpp"

int main() {
	try {
		notemanager::init_manager();
		gui::init_gui();

		printf("%s", notemanager::generate_new_guid().c_str());
	} catch (std::exception& e) {
		printf_s("Fail with exception: %s\n", e.what());
		getchar();
	}
}
