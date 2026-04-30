# Built-in Windows

## LogWindow

`LogWindow` is a scrollable, filterable log panel. Its state is static — any code in your application can write to it without holding a reference to the window instance.

### Setup

```cpp
#include "log_window.h"

userInterface.addWindow<imgui_kit::LogWindow>();
```

### Writing log entries

```cpp
using namespace imgui_kit;

LogWindow::addLog(colours::Green,  "[DEBUG]   Debug message.");
LogWindow::addLog(colours::White,  "[INFO]    Informative message.");
LogWindow::addLog(colours::Yellow, "[WARNING] Warning message.");
LogWindow::addLog(colours::Red,    "[ERROR]   Error message.");
LogWindow::addLog(colours::Red,    "[FATAL]   Fatal error message.");
```

`addLog` accepts a `printf`-style format string:

```cpp
LogWindow::addLog(colours::White, "[INFO] Frame %d processed in %.2f ms", frameIndex, elapsed);
```

### Utility methods

| Method | Description |
|---|---|
| `LogWindow::clearLogs()` | Remove all entries |
| `LogWindow::getLogCount()` | Number of entries currently stored |
| `LogWindow::isActive()` | Whether the window is currently visible |
| `LogWindow::renderContent()` | Render only the log content (no window chrome) — useful for embedding in another panel |

### Auto-scroll

The panel scrolls to the latest entry by default. Users can uncheck the **Auto-scroll** checkbox in the window to pause it.

### Filtering

The window includes a built-in text filter. Typing in the filter box hides entries that do not match, making it easy to isolate specific log levels or keywords.

---

## TemplateWindow

`TemplateWindow` is a minimal skeleton window. It renders a basic ImGui window and serves as a copy-paste starting point for new windows. See [Creating Windows](Creating-Windows) for details.
