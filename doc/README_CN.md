# Global Hotkey

**[English](README_EN.md) | 简体中文**

## 🚀 特点

- 独立！

  不依赖其他库与框架！

- 用于任意程序！

  甚至是控制台程序！

- 跨平台！

  **Windows**、**MacOS**、**Linux**（*Ubuntu*与*Debian*已测试）不在话下！

- 广泛兼容！

  **Qt**？**MFC**？没问题！

- 易于使用！

  参见示例代码。

- `监听式热键 (Hook GHM)`！

  需要实现~~后台监听木马~~某些特殊的功能？

## 🔧 如何编译？

项目使用`CMake`进行组织，仅需下面几行脚本即可编译使用！

```shell
git clone https://github.com/JaderoChan/GlobalHotkey.git
cd GlobalHotkey
cmake -Bbuild
cd build
make
```

### ⚙️ 编译选项

---

- `GBHK_BUILD_SHARED` 指定是否生成动态库，默认为`OFF`。若启用，在项目中使用动态库时定义`GLOBAL_HOTKEY_SHARED`宏以获得更好的性能（仅**Windows**）。

- `GBHK_DISABLE_REGISTER` 指定是否禁用`注册式热键 (Register GHM)`，默认为`OFF`。

- `GBHK_DISABLE_HOOK` 指定是否禁用`监听式热键 (Hook GHM)`），默认为`OFF`。

- `GBHK_BUILD_EXAMPLE` 是否生成示例程序，默认值取决于项目是否为主项目。

- `GBHK_EXAMPLE_USE_HOOK` 指定示例程序使用的热键类型，默认为`OFF`（即在示例程序中使用`Register GHM`）。

- `GBHK_EXAMPLE_BUILD_BASE_EXAMPLE` 指定是否生成`base example`示例程序，默认`ON`。

- `GBHK_EXAMPLE_BUILD_EVENT_QUEUE` 指定是否生成`event quque`实例程序，默认为`ON`。

## 🚩 如何使用？

1. 通过`getInstance`接口获取一个`热键管理器 (GHM)`对象。

2. 通过`start`接口启动`GHM`。

3. 通过相应的接口增加、移除或替换热键。

4. 热键被触发时将执行对应的回调函数。

5. 通过`end`接口结束`GHM`

---

下面展示了一个基本流程的示例代码

```cpp
GlobalHotkeyManager& ghm = RegisterGlobalHotkeyManager::getInstance();  // 获取`注册式热键管理器`实例对象。
ghm.start();    // 启动热键管理器。

KeyCombination hotkey1(CTRL, 'G');
KeyCombination hotkey2(CTRL, 'H');
KeyCombination hotkey3(CTRL, 'M');
ghm.add(hotkey1, &callback);                        // 绑定一个回调函数。
ghm.add(hotkey2, [=]() { if(isOk) emitSignal(); }); // 绑定一个Lambda函数。在热键触发且条件为真时发射一个信号。
ghm.add(hotkey3, [=]() { printf("Hello world!") }); // 仅仅打印一个消息。

// 主循环。
while (!shouldClose)
{
    // Do Something.
}

ghm.end();      // 结束热键管理器。
```

## 💡 示例

[基础示例](example/base_example/main.cpp)

[事件队列示例](example/event_queue/main.cpp)

## ❓ 答疑

### 许可证是什么？

---

本库使用MIT许可证，这意味着你可以拿它做任何事情。尽管不是必须，但在程序中进行署名是值得赞誉的！

### `Register GHM` 和 `Hook GHM`有什么区别？

---

`Register GHM`由操作系统或桌面环境进行维护，它可能会与其他应程序的全局热键发生冲突，而且通常来讲它不需要管理员权限。

`Hook GHM`完全基于`Hook`之类的键盘监听技术，并于此实现热键逻辑，所以它不会与其他应用程序的全局热键发生冲突，并且可使用的热键更为宽泛（例如在**Windows**下，`Win + T`是不可被注册为热键的，但通过`Hook GHM`可以做到），代价是它需要管理员权限。

除某些特殊需求外，应该尽量使用`Register GHM`，如果使用`Hook GHM`，你应该向用户阐明需要管理员权限的原因以及热键的具体用途。

### 为什么库需要维护额外的线程？

---

本项目对许多模块进行了解耦，而这些模块有时需要各维护一个线程。

例如`HooK GHM`除`热键管理器 (GHM)`自身的工作线程外，还需要额外维护一个`键盘监听 (Keyboard Hook)`模块的线程，如果你的程序需要进行深度的定制，你可以在此基础上进行更针对性的修改。

### 支持Wayland窗口系统吗？

---

仅支持`Hook GHM`，而`Hook GHM`需要管理员权限才可以正常工作。

### Wayland窗口系统下可以使用`Register GHM`吗？

---

不可以。**Linux**下的`Register GHM`依赖于**X11**。

## 🔔 注意点

- 增加、删除或替换热键等操作，必须在对应的`GHM`启动之后才可进行。
- 避免添加一个无效的热键或移除一个未添加的热键，这是未定义行为，可能造成非预期结果。本库对此类操作不做安全性检查，这些操作应该由用户决定。
- 热键的回调函数不应执行繁重的任务，以免出现线程阻塞等异常情况，合理的做法是使用异步机制或消息队列（例如**Qt**中的信号槽）。
- 在**Windows**平台下使用`Hook GHM`时，应保证回调函数的执行时间在限定范围内。

  *（详细信息参见 [Windows LowLevelKeyboard](https://learn.microsoft.com/zh-cn/windows/win32/winmsg/lowlevelkeyboardproc) ，其**Remarks**中提到**Timeout**时间为**1000毫秒**）*

- 使用**MSVC编译器**且启用了`Hook GHM`时需要配置`pthread for Windows`，参见[pthreads-win32](https://sourceware.org/pthreads-win32/)。
