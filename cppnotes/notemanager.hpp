#pragma once

#include <memory>

#include "models.hpp"

namespace notemanager {
	enum class guid_type {
		note,
		user
	};

	struct manager_state {
		manager_state() {} manager_state(manager_state& s) = delete; // non copy and basic constructor

		std::vector<models::user_t> users;
		std::vector<models::note_t> notes;
	};

	void init_manager();
	std::shared_ptr<manager_state> get_manager_state();

	models::user_t generate_user(std::string_view name);
	std::string generate_new_guid(guid_type type = guid_type::note);

	inline models::user_t* get_user_by_guid(std::string_view guid) {
		auto state = get_manager_state();
		auto user = std::find_if(state->users.begin(), state->users.end(), [&](models::user_t& u) { return u.id == guid; });
		return user == state->users.end() ? nullptr : &*user;
	}

	inline models::note_t* get_node_by_guid(std::string_view guid) {
		auto state = get_manager_state();
		auto note = std::find_if(state->notes.begin(), state->notes.end(), [&](models::note_t& u) { return u.id == guid; });
		return note == state->notes.end() ? nullptr : &*note;
	}

	void read_files();
	void write_files();

	// Update data from and to files. First - from, second - to.
	void update_files();
}