#pragma once

#include <memory>

#include "models.hpp"
#include "executor.hpp"

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

	inline models::note_t* get_note_by_guid(std::string_view guid) {
		auto state = get_manager_state();
		auto note = std::find_if(state->notes.begin(), state->notes.end(), [&](models::note_t& u) { return u.id == guid; });
		return note == state->notes.end() ? nullptr : &*note;
	}

	void read_files();
	void write_files();

	inline void add_user(const models::user_t& user) {
		if (get_user_by_guid(user.id)) return;
		get_manager_state()->users.push_back(user);
		executor::execute(std::bind(write_files));
	}

	inline void remove_user(std::string_view guid) {
		auto state = get_manager_state();
		std::erase_if(state->users, [&](models::user_t& u) { return u.id == guid; });
		std::erase_if(state->notes, [&](models::note_t& n) { return n.user_id == guid; });
		executor::execute(std::bind(write_files));
	}

	inline void add_note(const models::note_t& note) {
		if (get_note_by_guid(note.id)) return;
		get_manager_state()->notes.push_back(note);
		executor::execute(std::bind(write_files));
	}

	inline void remove_note(std::string_view guid) {
		auto state = get_manager_state();
		std::erase_if(state->notes, [&](models::note_t& u) { return u.id == guid; });
		executor::execute(std::bind(write_files));
	}
}