# Creating Windows

## The base class

Every UI panel inherits from `UserInterfaceWindow` and implements `render()`:

```cpp
#include "user_interface_window.h"

namespace imgui_kit
{
    class UserInterfaceWindow
    {
    public:
        virtual void render() = 0;
        virtual ~UserInterfaceWindow() = default;
        // non-copyable, non-movable
    };
}
```

`render()` is called once per frame, inside an active ImGui frame. You can call any ImGui, ImPlot, or node-editor API from it.

---

## Minimal example

```cpp
#include "user_interface_window.h"

class MyWindow : public imgui_kit::UserInterfaceWindow
{
public:
    void render() override
    {
        if (ImGui::Begin("My Window"))
            ImGui::Text("Hello, World!");
        ImGui::End();
    }
};
```

---

## Adding windows to the interface

Use `addWindow<T>()` on your `UserInterface` instance. Arguments are forwarded to the constructor:

```cpp
userInterface.addWindow<MyWindow>();
userInterface.addWindow<SomeOtherWindow>(arg1, arg2);
```

Windows are stored as `shared_ptr<UserInterfaceWindow>` and rendered in insertion order.

---

## Global window flags

You can set ImGui window flags that apply to all windows:

```cpp
imgui_kit::addGlobalWindowFlags(ImGuiWindowFlags_NoCollapse);
imgui_kit::removeGlobalWindowFlags(ImGuiWindowFlags_NoMove);
imgui_kit::setGlobalWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
```

Read them inside your `render()`:

```cpp
if (ImGui::Begin("My Window", nullptr, imgui_kit::getGlobalWindowFlags()))
    // ...
ImGui::End();
```

---

## The render loop

```cpp
userInterface.initialize();
while (!userInterface.isShutdownRequested())
    userInterface.render();
userInterface.shutdown();
```

`render()` processes OS events, begins an ImGui frame, calls `render()` on every registered window, and presents the frame. `isShutdownRequested()` returns `true` when the user closes the OS window.

---

## TemplateWindow

`TemplateWindow` is a minimal built-in window that demonstrates the pattern. Use it as a starting point or a placeholder:

```cpp
#include "template_window.h"
userInterface.addWindow<imgui_kit::TemplateWindow>();
```
