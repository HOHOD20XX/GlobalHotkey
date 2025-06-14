# Global Hotkey

**English | [简体中文](README.md)**

## Features

- Can be used for any type of program (even console programs)

  e.g. Qt, MFC, command line programs etc.

- Supports hook-based keyborad event listener to instead of register-based hotkey.
- Supports debouncing to avoid repeated trigger of the hook hotkey.
- Virtual key enumeration.
- Keycombination is supported.

## Compiling

Compile with MinGW and specify other options as needed.

```console
git clone https://github.com/JaderoChan/GlobalHotkey
cd GlobalHotkey
cmake -S ./ -B ./build -G "MinGW Makefiles"
cd build
make
```

### Options

- `GBHK_BUILD_SHARED` specifies whether to generate a dynamic library, default is OFF. If enabled, specify `GLOBAL_HOTKEY_SHARED` macro in your project to get better performance.
- `GBHK_DISABLE_REGISTER` specifies whether to disable generating register-based hotkeys, default is OFF.
- `GBHK_DISABLE_HOOK` specifies whether to disable generating hook-based hotkeys, default is OFF.
- `GBHK_BUILD_EXAMPLE` specifies whether to build the example program, default is ON if the project is the main project, otherwise it is OFF.
- `GBHK_EXAMPLE_USE_HOOK` specifies the type of hotkey used by the example program, default is OFF.

## Concepts

- The object that centrally manages hotkeys is called a **Global Hotkey Manager**.
- Register typed hotkeys represent hotkey systems managed by the platform/system.
- Hook typed hotkeys represent hotkey systems implemented by libraries through Hook or injection techniques for key monitoring. They do not have hotkey conflict issues and can be used in certain special circumstances.

## Usage

The most basic process is as follows:

1. Decide on the hotkey system (registration type / Hook type) you need to use
2. Obtain the corresponding instance object of the **Global     Hotkey Manager**.
3. Call the `start` function of the **Global Hotkey Manager** to start.
4. Use the relevant interfaces to add, delete and change hotkeys.
5. Call the `end` function to end the **Global Hotkey Manager** when the program ends or the hotkey system is no longer needed.

```cpp
GlobalHotkeyManager& ghm = RegisterGHM::getInstance();  // Get the instance object of the Global Hotkey Manager.

ghm.start();    // Start the Global Hotkey Manager.

// Add, delete and modify hotkeys.
KeyCombination kc1(MODI_CTRL | MODI_SHIFT, 'G');
KeyCombination kc2(MODI_ALT, 'J');
KeyCombination kc3(MODI_ALT, 'H');
ghm.add(kc1, callback1);
ghm.add(kc2, callback2);
ghm.replace(kc2, kc3);
ghm.remove(kc3);

// ...

ghm.end();    // End the Global Hotkey Manager.
```

## Attention

- Only supports Windows platform for now.
- The addition, deletion and modification of hotkeys in the "Register Global Hotkey Manager" must be carried out after it is started.
- `GBHK_DISABLE_REGISTER` and `GBHK_DISABLE_HOOK` cannot be set to ON at the same time.
- Cannot enable the `GBHK_EXAMPLE_USE_HOOK` option when the `GBHK_DISABLE_HOOK` is turned on.

## Example

[Example Code](example/example1.cpp)
