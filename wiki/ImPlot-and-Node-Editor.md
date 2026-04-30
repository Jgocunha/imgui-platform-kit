# ImPlot and Node Editor

ImGui Platform Kit initialises both ImPlot and imgui-node-editor contexts for you as part of `UserInterface::initialize()` and tears them down in `UserInterface::shutdown()`. You do not need to call `ImPlot::CreateContext()` or `ImNodeEditor::CreateEditor()` yourself.

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
    }

    ~GraphWindow() override
    {
        ax::NodeEditor::DestroyEditor(context);
    }

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
