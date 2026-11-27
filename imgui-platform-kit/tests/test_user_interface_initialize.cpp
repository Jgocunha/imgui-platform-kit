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
    // Sets an environment variable, or unsets it when value is nullptr, until destroyed.
    class ScopedEnvironmentVariable
    {
    public:
        ScopedEnvironmentVariable(const char* name, const char* value)
            : name(name)
        {
            if (const char* current = std::getenv(name))
                savedValue = current;
            assign(value);
        }

        ~ScopedEnvironmentVariable()
        {
            assign(savedValue ? savedValue->c_str() : nullptr);
        }

        ScopedEnvironmentVariable(const ScopedEnvironmentVariable&) = delete;
        ScopedEnvironmentVariable& operator=(const ScopedEnvironmentVariable&) = delete;
        ScopedEnvironmentVariable(ScopedEnvironmentVariable&&) = delete;
        ScopedEnvironmentVariable& operator=(ScopedEnvironmentVariable&&) = delete;

    private:
        void assign(const char* value) const
        {
            if (value)
                setenv(name, value, 1);
            else
                unsetenv(name);
        }

        const char* name;
        std::optional<std::string> savedValue;
    };

    // Explicit dimensions keep WindowParameters from querying the monitor itself.
    UserInterfaceParameters parametersWithExplicitWindowSize()
    {
        return UserInterfaceParameters{ WindowParameters("initialize test", 800, 600) };
    }
}

TEST_CASE("UserInterface::initialize throws when GLFW finds no display", "[user_interface][initialize]")
{
    const ScopedEnvironmentVariable display("DISPLAY", nullptr);
    // An unset WAYLAND_DISPLAY makes libwayland fall back to the default socket, so
    // point it at one that does not exist instead.
    const ScopedEnvironmentVariable waylandDisplay("WAYLAND_DISPLAY", "imgui-kit-test-no-such-display");
    const ScopedEnvironmentVariable waylandSocket("WAYLAND_SOCKET", nullptr);
    UserInterface userInterface(parametersWithExplicitWindowSize());

    REQUIRE_THROWS_WITH(userInterface.initialize(), "Failed to initialize GLFW.");
}

TEST_CASE("UserInterface::initialize throws when GLFW cannot create the window", "[user_interface][initialize]")
{
    // GLFW stays initialized after this check, so initialize() reuses its display
    // connection. Closing it and reconnecting would let an X server without
    // -noreset (such as xvfb-run's) drop the second connection while it resets.
    if (!glfwInit())
        SKIP("GLFW cannot reach a display");

    UserInterfaceParameters parameters = parametersWithExplicitWindowSize();
    parameters.windowParameters.width = 0; // GLFW rejects a window with no width
    UserInterface userInterface(parameters);

    REQUIRE_THROWS_WITH(userInterface.initialize(), "Failed to create the GLFW window.");
}

#endif
