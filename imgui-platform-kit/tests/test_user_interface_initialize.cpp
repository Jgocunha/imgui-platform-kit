#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

// Only Linux can be made to fail GLFW initialization from inside a test, by hiding
// the display from it. The macOS backend shares the same initialize() code.
#if defined(__linux__)

#include <cstdlib>
#include <optional>
#include <string>

#include "user_interface.h"

using namespace imgui_kit;

namespace
{
    class ScopedUnsetEnvironmentVariable
    {
    public:
        explicit ScopedUnsetEnvironmentVariable(const char* name)
            : name(name)
        {
            if (const char* value = std::getenv(name))
                savedValue = value;
            unsetenv(name);
        }

        ~ScopedUnsetEnvironmentVariable()
        {
            if (savedValue)
                setenv(name, savedValue->c_str(), 1);
        }

        ScopedUnsetEnvironmentVariable(const ScopedUnsetEnvironmentVariable&) = delete;
        ScopedUnsetEnvironmentVariable& operator=(const ScopedUnsetEnvironmentVariable&) = delete;
        ScopedUnsetEnvironmentVariable(ScopedUnsetEnvironmentVariable&&) = delete;
        ScopedUnsetEnvironmentVariable& operator=(ScopedUnsetEnvironmentVariable&&) = delete;

    private:
        const char* name;
        std::optional<std::string> savedValue;
    };

    bool isDisplayAvailable()
    {
        return std::getenv("DISPLAY") != nullptr || std::getenv("WAYLAND_DISPLAY") != nullptr;
    }

    // Explicit dimensions keep WindowParameters from querying the monitor itself.
    UserInterfaceParameters parametersWithExplicitWindowSize()
    {
        return UserInterfaceParameters{ WindowParameters("initialize test", 800, 600) };
    }
}

TEST_CASE("UserInterface::initialize throws when GLFW finds no display", "[user_interface][initialize]")
{
    const ScopedUnsetEnvironmentVariable display("DISPLAY");
    const ScopedUnsetEnvironmentVariable waylandDisplay("WAYLAND_DISPLAY");
    UserInterface userInterface(parametersWithExplicitWindowSize());

    REQUIRE_THROWS_WITH(userInterface.initialize(), "Failed to initialize GLFW.");
}

TEST_CASE("UserInterface::initialize throws when GLFW cannot create the window", "[user_interface][initialize]")
{
    if (!isDisplayAvailable())
        SKIP("needs a display for GLFW to initialize");

    UserInterfaceParameters parameters = parametersWithExplicitWindowSize();
    parameters.windowParameters.width = 0; // GLFW rejects a window with no width
    UserInterface userInterface(parameters);

    REQUIRE_THROWS_WITH(userInterface.initialize(), "Failed to create the GLFW window.");
}

#endif
