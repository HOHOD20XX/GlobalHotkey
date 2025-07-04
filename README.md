# Global Hotkey

**English | [Simplified Chinese](README_CN.md)**

## 🚀 Features

- Independent!

  No dependencies on other libraries or frameworks!

- Works with any program!

  Even console applications!

- Cross-platform!

  Supports **Windows**, **MacOS**, and **Linux** (*Ubuntu* and *Debian* tested) with ease!

- Broad compatibility!

  **Qt**? **MFC**? No problem!

- Easy to use!

  See example code.

- `Hook-style Global Hotkey (Hook GHM)`!

  Need to implement ~~background monitoring trojan~~ some special features?

## 🔧 How to Build?

The project uses `CMake` for organization. Just run these few scripts to build and use it!

```shell
git clone https://github.com/JaderoChan/GlobalHotkey.git
cd GlobalHotkey
cmake -Bbuild
cd build
make
```

### ⚙️ Build Options

---

- `GBHK_BUILD_SHARED` Specifies whether to build a dynamic library. Default is `OFF`. If enabled, define the `GLOBAL_HOTKEY_SHARED` macro when using the dynamic library in your project for better performance (only on **Windows**).

- `GBHK_DISABLE_REGISTER` Specifies whether to disable `Register-style Global Hotkey (Register GHM)`. Default is `OFF`.

- `GBHK_DISABLE_HOOK` Specifies whether to disable `Hook-style Global Hotkey (Hook GHM)`. Default is `OFF`.

- `GBHK_BUILD_EXAMPLE` Whether to build example programs. Default depends on whether the project is the main project.

- `GBHK_EXAMPLE_USE_HOOK` Specifies the hotkey type used in the example program. Default is `OFF` (i.e., uses `Register GHM` in the example program).

- `GBHK_EXAMPLE_BUILD_BASE_EXAMPLE` Specifies whether to build the `base example` example program. Default is `ON`.

- `GBHK_EXAMPLE_BUILD_EVENT_QUEUE` Specifies whether to build the `event queue` example program. Default is `ON`.

## 🚩 How to Use?

1. Obtain a `Global Hotkey Manager (GHM)` object via the `getInstance` interface.

2. Start the `GHM` via the `start` interface.

3. Add, remove, or replace hotkeys using the corresponding interfaces.

4. When a hotkey is triggered, the corresponding callback function will be executed.

5. End the `GHM` via the `end` interface.

---

Below is example code demonstrating the basic workflow:

```cpp
GlobalHotkeyManager& ghm = RegisterGlobalHotkeyManager::getInstance();  // Get an instance of the `Register GHM`.
ghm.start();    // Start the Global Hotkey Manager.

KeyCombination hotkey1(CTRL, 'G');
KeyCombination hotkey2(CTRL, 'H');
KeyCombination hotkey3(CTRL, 'M');
ghm.add(hotkey1, &callback);                        // Bind a callback function.
ghm.add(hotkey2, [=]() { if(isOk) emitSignal(); }); // Bind a Lambda function. Emits a signal when the hotkey is triggered and the condition is true.
ghm.add(hotkey3, [=]() { printf("Hello world!") }); // Simply prints a message.

// Main loop.
while (!shouldClose)
{
    // Do Something.
}

ghm.end();      // End the Global Hotkey Manager.
```

## 💡 Examples

[Base Example](example/base_example/main.cpp)

[Event Queue Example](example/event_queue/main.cpp)

## ❓ FAQ

### What is the License?

---

This library is licensed under the MIT License, which means you can do whatever you want with it. Although not mandatory, giving attribution in your program is highly appreciated.

### What's the difference between `Register GHM` and `Hook GHM`?

---

`Register GHM` is maintained by the operating system or desktop environment. It may conflict with global hotkeys from other applications and typically does not require administrator privileges.

`Hook GHM` is entirely based on keyboard monitoring techniques like `Hook` to implement hotkey logic. Thus, it won't conflict with other applications' global hotkeys and supports a broader range of hotkeys (e.g., on **Windows**, `Win + T` cannot be registered as a hotkey, but `Hook GHM` can achieve this). The trade-off is that it requires administrator privileges.

Unless there are special requirements, `Register GHM` should be preferred. If using `Hook GHM`, you should clearly explain to users why administrator privileges are needed and the specific purpose of the hotkey.

### Why does the library need to maintain additional threads?

---

This project decouples many modules, and these modules sometimes require their own threads.

For example, `Hook GHM` requires an additional thread for the `Keyboard Hook` module besides the `Global Hotkey Manager (GHM)`'s own worker thread. If your program needs deep customization, you can make more targeted modifications based on this.

### Does it support the Wayland window system?

---

Only `Hook GHM` is supported, and `Hook GHM` requires administrator privileges to work properly.

### Can `Register GHM` be used under the Wayland window system?

---

No. `Register GHM` on **Linux** relies on **X11**.

## 🔔 Notes

- Operations like adding, deleting, or replacing hotkeys can only be performed after the corresponding `GHM` has started.
- Hotkey callback functions should not execute heavy tasks to avoid thread blocking or other anomalies. A reasonable approach is to use asynchronous mechanisms or message queues (e.g., **Qt**'s signal-slot system).
- When using `Hook GHM` on **Windows**, ensure the callback function's execution time stays within the specified limit.

  *(For details, refer to [Windows LowLevelKeyboard](https://learn.microsoft.com/en-us/windows/win32/winmsg/lowlevelkeyboardproc). The **Remarks** section mentions a **Timeout** of **1000 milliseconds**.)*
