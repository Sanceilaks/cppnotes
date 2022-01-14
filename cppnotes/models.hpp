#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace models {
	struct user_t {
		std::string name;
		std::string id;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(user_t, name, id);
	};

	struct note_t {
		std::string id;
		std::string user_id;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(note_t, id, user_id);
	};
}

