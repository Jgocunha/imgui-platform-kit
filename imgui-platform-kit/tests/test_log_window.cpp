#include <catch2/catch_test_macros.hpp>
#include "log_window.h"
#include "colour_palette.h"

using namespace imgui_kit;

// ─── isActive ─────────────────────────────────────────────────────────────────

TEST_CASE("LogWindow: isActive becomes true after construction", "[log]")
{
    {
        LogWindow lw; // sets isWindowActive = true, clears logs
        CHECK(LogWindow::isActive());
    }
    // isWindowActive stays true after destruction (static state)
    CHECK(LogWindow::isActive());
}

// ─── getLogCount / clearLogs ─────────────────────────────────────────────────

TEST_CASE("LogWindow: log count is 0 after construction", "[log]")
{
    LogWindow lw; // calls clean() internally
    REQUIRE(LogWindow::getLogCount() == 0);
}

TEST_CASE("LogWindow: clearLogs resets count to 0", "[log]")
{
    LogWindow lw;
    LogWindow::addLog(colours::White, "entry one");
    LogWindow::addLog(colours::White, "entry two");
    LogWindow::clearLogs();
    REQUIRE(LogWindow::getLogCount() == 0);
}

// ─── addLog ───────────────────────────────────────────────────────────────────

TEST_CASE("LogWindow: addLog increments the log count", "[log]")
{
    LogWindow lw; // clean slate
    REQUIRE(LogWindow::getLogCount() == 0);

    LogWindow::addLog(colours::White, "first message");
    CHECK(LogWindow::getLogCount() == 1);

    LogWindow::addLog(colours::Green, "second message");
    CHECK(LogWindow::getLogCount() == 2);
}

TEST_CASE("LogWindow: addLog with printf-style format string works", "[log]")
{
    LogWindow lw;
    REQUIRE_NOTHROW(LogWindow::addLog(colours::Red, "[ERROR] code=%d", 42));
    REQUIRE(LogWindow::getLogCount() == 1);
}

TEST_CASE("LogWindow: addLog with multiple format arguments works", "[log]")
{
    LogWindow lw;
    REQUIRE_NOTHROW(LogWindow::addLog(colours::Yellow, "[%s] value=%.2f index=%d", "TAG", 3.14f, 7));
    REQUIRE(LogWindow::getLogCount() == 1);
}

TEST_CASE("LogWindow: addLog with empty format string works", "[log]")
{
    LogWindow lw;
    REQUIRE_NOTHROW(LogWindow::addLog(colours::Gray, ""));
    REQUIRE(LogWindow::getLogCount() == 1);
}

TEST_CASE("LogWindow: addLog with various colours does not crash", "[log]")
{
    LogWindow lw;
    REQUIRE_NOTHROW(LogWindow::addLog(colours::Red,   "[FATAL]   fatal"));
    REQUIRE_NOTHROW(LogWindow::addLog(colours::Yellow,"[WARNING] warn"));
    REQUIRE_NOTHROW(LogWindow::addLog(colours::Green, "[DEBUG]   debug"));
    REQUIRE_NOTHROW(LogWindow::addLog(colours::White, "[INFO]    info"));
    REQUIRE_NOTHROW(LogWindow::addLog(colours::Cyan,  "[TRACE]   trace"));
    CHECK(LogWindow::getLogCount() == 5);
}

TEST_CASE("LogWindow: addLog with a long message does not crash", "[log]")
{
    LogWindow lw;
    // Build a 600-char string (longer than the 1024 internal buffer is fine;
    // vsnprintf will truncate at 1023 chars + null terminator)
    const std::string long_msg(600, 'X');
    REQUIRE_NOTHROW(LogWindow::addLog(colours::White, "%s", long_msg.c_str()));
    REQUIRE(LogWindow::getLogCount() == 1);
}

TEST_CASE("LogWindow: constructing a second LogWindow clears previous logs", "[log]")
{
    {
        LogWindow lw1;
        LogWindow::addLog(colours::White, "msg1");
        LogWindow::addLog(colours::White, "msg2");
        REQUIRE(LogWindow::getLogCount() == 2);
    }
    LogWindow lw2; // constructor calls clean()
    REQUIRE(LogWindow::getLogCount() == 0);
}

// ─── Static state isolation ───────────────────────────────────────────────────

TEST_CASE("LogWindow: clearLogs after many adds leaves count at zero", "[log]")
{
    LogWindow lw;
    for (int i = 0; i < 50; ++i)
        LogWindow::addLog(colours::White, "message %d", i);
    REQUIRE(LogWindow::getLogCount() == 50);

    LogWindow::clearLogs();
    REQUIRE(LogWindow::getLogCount() == 0);
}
