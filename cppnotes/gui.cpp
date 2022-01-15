#include "gui.hpp"

#include <functional>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "models.hpp"
#include "notemanager.hpp"

namespace g {
	struct guistate_t {
		std::shared_ptr<sf::RenderWindow> window;
		sf::Clock delta_clock;
		std::function<void()> topfn;
		std::string current_user;
	};

	static std::shared_ptr<guistate_t> guistate;
	static ImFont* font;
}

namespace draw_funcs {
	std::string get_time_string() {
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		std::string s(30, '\0');
		std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
		return s;
	}

	void _create_new_user() {
		static std::string username;
		if ((ImGui::InputText("User name##usernametext", &username, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Create user##menubutton")) 
		&& !username.empty()) {
			models::user_t tmp;
			tmp.name = username;
			tmp.id = notemanager::generate_new_guid(notemanager::guid_type::user);

			notemanager::add_user(std::move(tmp));
			username = "";
		}
	}

	void _create_new_note() {
		static std::string note_text;
		static std::string note_title;

		ImGui::InputText("Title##NEWNOTETITLE", &note_title);
		ImGui::InputTextMultiline("Text##NEWNOTETEXT", &note_text);

		if (ImGui::Button("Create note##NEWNOTEBUTTON")) {
			models::note_t tmp;
			tmp.id = notemanager::generate_new_guid();
			tmp.text = note_text;
			tmp.title = note_title;
			tmp.user_id = g::guistate->current_user;
			tmp.date = get_time_string();
			
			notemanager::add_note(std::move(tmp));
			note_text = "";
			note_title = "";
		}
	}
}

void gui::init_gui() {
	g::guistate = std::make_shared<g::guistate_t>();
	g::guistate->window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "CppNotes", sf::Style::Close | sf::Style::Titlebar);

	ImGui::SFML::Init(*g::guistate->window);
	ImGui::GetIO().IniFilename = nullptr;
}

inline void shutdown() {
	ImGui::SFML::Shutdown();
	g::guistate.reset();
}

inline void render_window() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("User")) {
			for (auto& i : notemanager::get_manager_state()->users) {
				if (ImGui::BeginMenu((i.name + "##user").c_str())) {
					if (ImGui::Button(std::string("Set").append("##").append(i.name).c_str()))
						g::guistate->current_user = i.id;

					if (ImGui::Button(std::string("Remove").append("##").append(i.name).c_str()))
						notemanager::remove_user(i.id);

					ImGui::EndMenu();
				}
			}

			if (ImGui::BeginMenu("Create new user")) {
				draw_funcs::_create_new_user();
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
	ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
	ImGui::Begin("##MAINWINDOW", (bool*)0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	auto current_user = notemanager::get_user_by_guid(g::guistate->current_user);
	if (current_user) {
		ImGui::Button("Add note##NEWNOTE");
		if (ImGui::BeginPopupContextItem(NULL, ImGuiPopupFlags_MouseButtonLeft)) {
			draw_funcs::_create_new_note();
			ImGui::EndPopup();
		}

		for (auto& i : notemanager::get_manager_state()->notes) {
			if (i.user_id == current_user->id) {
				ImGui::Button(i.title.c_str());
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					notemanager::remove_note(i.id);
				if (ImGui::BeginPopupContextItem(NULL, ImGuiPopupFlags_MouseButtonLeft)) {
					ImGui::TextColored(ImColor(0, 255, 0), i.title.c_str());
					ImGui::Separator();
					ImGui::Text(i.text.c_str());
					ImGui::Separator();
					ImGui::TextColored(ImColor(0, 0, 255), i.date.c_str());
					ImGui::EndPopup();
				}
			}
		}
	}

	ImGui::End();
}

void gui::render() {
	using namespace g;

	guistate->delta_clock.restart();
	while (guistate->window->isOpen()) {
		sf::Event _event;
		while (guistate->window->pollEvent(_event)) {
			ImGui::SFML::ProcessEvent(_event);
			if (_event.type == sf::Event::Closed)
				guistate->window->close();
		}

		ImGui::SFML::Update(*guistate->window, guistate->delta_clock.restart());

		render_window();

		guistate->window->clear();
		ImGui::SFML::Render(*guistate->window);
		guistate->window->display();
	}

	shutdown();
}
