# ImPlot and Node Editor

**Context ownership — what the kit manages vs what you manage:**

| Context | Created by | Destroyed by |
|---|---|---|
| `ImGui` | `UserInterface::initialize()` | `UserInterface::shutdown()` |
| `ImPlot` | `UserInterface::initialize()` | your code (`ImPlot::DestroyContext()`) |
| Node editor (`EditorContext`) | your code (`CreateEditor()`) | `UserInterface::shutdown()` via `GetCurrentEditor()` |

You do not need to call `ImGui::CreateContext()` or `ImPlot::CreateContext()` — the kit does that. You must call `ImPlot::DestroyContext()` yourself before or after `shutdown()`. For the node editor, create your own `EditorContext` with `ax::NodeEditor::CreateEditor()`, set it as current with `SetCurrentEditor()` before each use, and the kit will destroy it for you in `shutdown()` (via `GetCurrentEditor()`).

## ImPlot

[ImPlot](https://github.com/epezent/implot) provides rich 2D plotting inside ImGui windows. Use it directly from any `render()` override:

```cpp
#include "user_interface_window.h"
#include "implot.h"

class PlotWindow : public imgui_kit::UserInterfaceWindow
{
public:
    void render() override
    {
        if (ImGui::Begin("Plot"))
        {
            if (ImPlot::BeginPlot("My Plot"))
            {
                static float xs[] = {0, 1, 2, 3};
                static float ys[] = {0, 1, 4, 9};
                ImPlot::PlotLine("y = x²", xs, ys, 4);
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }
};
```

The ImPlot context is created by the kit but not destroyed — call `ImPlot::DestroyContext()` after `userInterface.shutdown()`:

```cpp
userInterface.initialize();
while (!userInterface.isShutdownRequested())
    userInterface.render();
userInterface.shutdown();
ImPlot::DestroyContext();
```

Refer to the [ImPlot documentation](https://github.com/epezent/implot) for the full API.

## imgui-node-editor

[imgui-node-editor](https://github.com/thedmd/imgui-node-editor) provides a node graph editing canvas. The namespace alias `ImNodeEditor` is available after including any platform header via `user_interface.h`:

```cpp
#include "user_interface_window.h"
#include <imgui-node-editor/imgui_node_editor.h>
namespace ImNodeEditor = ax::NodeEditor;

class GraphWindow : public imgui_kit::UserInterfaceWindow
{
    ax::NodeEditor::EditorContext* context = nullptr;
public:
    GraphWindow()
    {
        ax::NodeEditor::Config config;
        context = ax::NodeEditor::CreateEditor(&config);
        // Set as current immediately so shutdown() can find it via GetCurrentEditor()
        ax::NodeEditor::SetCurrentEditor(context);
    }

    // Do NOT call DestroyEditor here — UserInterface::shutdown() does it
    // via GetCurrentEditor(). Destroying it twice would crash.
    ~GraphWindow() override = default;

    void render() override
    {
        if (ImGui::Begin("Node Graph"))
        {
            ax::NodeEditor::SetCurrentEditor(context);
            ax::NodeEditor::Begin("My Editor");

            ax::NodeEditor::BeginNode(1);
            ImGui::Text("Node A");
            ax::NodeEditor::EndNode();

            ax::NodeEditor::End();
            ax::NodeEditor::SetCurrentEditor(nullptr);
        }
        ImGui::End();
    }
};
```

Refer to the [imgui-node-editor documentation](https://github.com/thedmd/imgui-node-editor) for the full API.
