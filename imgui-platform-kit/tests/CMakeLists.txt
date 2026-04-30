find_package(Catch2 3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

set(test_sources
    test_parameters.cpp
    test_window_flags.cpp
    test_colour_palette.cpp
    test_themes.cpp
    test_log_window.cpp
)

add_executable(imgui-platform-kit-tests ${test_sources})

target_link_libraries(imgui-platform-kit-tests PRIVATE
    Catch2::Catch2WithMain
    imgui-platform-kit
    imgui::imgui
)

include(CTest)
include(Catch)
catch_discover_tests(imgui-platform-kit-tests)
