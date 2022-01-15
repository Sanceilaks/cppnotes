cmake_minimum_required (VERSION 3.8)

find_package(SDL2 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable (cppnotes "cppnotes.cpp" "gui.hpp" "gui.cpp"  "notemanager.hpp" "notemanager.cpp" "models.hpp" 
"executor.hpp" )

target_link_libraries(cppnotes PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(cppnotes PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(cppnotes PRIVATE imgui::imgui)
target_link_libraries(cppnotes PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(cppnotes PRIVATE ImGui-SFML::ImGui-SFML)