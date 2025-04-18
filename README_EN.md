# Global Hotkey

## Features

- Can be used for any type of program (even console programs)

  e.g. Qt, MFC, command line programs etc.

- Supports hook-based keyborad event listener to instead of register-based hotkey.
- Supports debouncing to avoid repeated trigger of the hook hotkey.
- Has a good exception handling mechanism.
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

#### Options

- `GBHK_BUILD_SHARED` specifies whether to generate a dynamic library, default is OFF. If enabled, specify `GLOBAL_HOTKEY_SHARED` macro in your project to get better performance.
- `GBHK_DISABLE_REGISTER` specifies whether to disable generating register-based hotkeys, default is OFF.
- `GBHK_DISABLE_HOOK` specifies whether to disable generating hook-based hotkeys, default is OFF.
- `GBHK_BUILD_EXAMPLE` specifies whether to build the example program, default is ON if the project is the main project, otherwise it is OFF.
- `BUILD_EXAMPLE_USE_HOOK` specifies the type of hotkey used by the example program, default is OFF.

## Concepts

- The object that centrally manages hotkeys is called a **Hotkey Manager**, and all subclasses inherited from `GlobalHotkeyBase` belong to a **Hotkey Manager**.
- Register typed hotkeys represent hotkey systems managed by the platform/system.
- Hook typed hotkeys represent hotkey systems implemented by libraries through Hook or injection techniques for key monitoring. They do not have hotkey conflict issues and can be used in certain special circumstances.

## Usage

The most basic process is as follows:

1. Decide on the hotkey system (registration type / Hook type) you need to use
2. Obtain the corresponding instance object of the **Hotkey Manager**.
3. Call the `start` function of the **Hotkey Manager** to start.
4. Use the relevant interfaces to add, delete and change hotkeys.
5. Call the `end` function to end the **Hotkey Manager** when the program ends or the hotkey system is no longer needed.

```cpp
GlobalHotkeyBase* instance = RegGlobalHotkey::getInstance();  // Obtain the instance of the Hotkey Manager.

instance->start();    // Start the Hotkey Manager.

// Add, delete and change hotkeys.
KeyCombination kc1(CTRL | SHIFT, 'G');
KeyCombination kc2(ALT, 'J');
KeyCombination kc3(ALT, 'H');
instance->add(kc1);
instance->add(kc2);
instance->replace(kc2, kc3);
instance->remove(kc3);

// ...

instance->end();    // End the Hotkey Manager.
```

## Attention

- Only supports Windows platform for now.
- The addition, deletion and modification of hotkeys must be carried out after the corresponding **Hotkey Manager** is started.
- `GBHK_DISABLE_REGISTER` and `GBHK_DISABLE_HOOK` cannot be set to ON at the same time.
- Cannot enable the `BUILD_EXAMPLE_USE_HOOK` option when the `GBHK_DISABLE_HOOK` is turned on.

## Example

[Example Code](example/example1.cpp)
