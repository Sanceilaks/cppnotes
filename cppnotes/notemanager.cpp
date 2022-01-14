#include "notemanager.hpp"

#include <random>
#include <thread>
#include <filesystem>
#include <fstream>

constexpr auto users_file_name = "users.cppnotes";
constexpr auto notes_file_name = "notes.cppnotes";

namespace g {
	static std::shared_ptr<notemanager::manager_state> manager_state;
	static bool need_update = true;
}

template<typename T>
inline T read_file_and_parse(const std::filesystem::path& path) {
	std::ifstream f(path);
	return nlohmann::json::parse(std::istream_iterator<char>(f), std::istream_iterator<char>()).get<T>();
}

inline std::string random_string(std::size_t size = 32) {
	static auto& symbols = 
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static std::mt19937 random_generator{ std::random_device{}() };
	static std::uniform_int_distribution<std::size_t> pick_char(0, sizeof(symbols - 2));
	std::string foo;
	while (size--) foo += symbols[pick_char(random_generator)];
	return foo;
}

void notemanager::init_manager() {
	g::manager_state = std::make_shared<manager_state>();
	
	auto create_file = [](const std::filesystem::path& p) {
		if (!std::filesystem::exists(p)) {
			std::ofstream s(p.c_str()); s << "[]";
		}
	};

	create_file("users.cppnotes");
	create_file("notes.cppnotes");

	read_files();
}

std::shared_ptr<notemanager::manager_state> notemanager::get_manager_state() {
	return g::manager_state;
}

models::user_t notemanager::generate_user(std::string_view name) {
	models::user_t user;
	user.name = name;
	user.id = generate_new_guid(guid_type::user);
	return user;
}

std::string notemanager::generate_new_guid(guid_type type) {
	return std::string("CPPNOTESGUID").append(type == guid_type::note ? "_N_" : "_U_").
		append(random_string(6).append("_").append(random_string(6).append("_").append(random_string(6))));
}

void notemanager::read_files() {
	g::manager_state->users = read_file_and_parse<decltype(g::manager_state->users)>(users_file_name);
	g::manager_state->notes = read_file_and_parse<decltype(g::manager_state->notes)>(notes_file_name);
}

void notemanager::write_files() {

}

void notemanager::update_files() {
	
}
