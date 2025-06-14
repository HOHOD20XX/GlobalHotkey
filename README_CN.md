# Global Hotkey

**[English](README_EN.md) | 简体中文**

## 特点

- 可用于任意类型的程序（甚至是控制台程序）

  例如Qt、MFC、命令行等等。

- 支持使用Hook实现的按键监听替代注册型热键。
- 支持Hook按键监听的去抖动。
- 虚拟键枚举。
- 组合热键支持。

## 编译

使用MinGW进行编译，可根据需求指定其他选项。

```console
git clone https://github.com/JaderoChan/GlobalHotkey
cd GlobalHotkey
cmake -S ./ -B ./build -G "MinGW Makefiles"
cd build
make
```

### 可用选项

- `GBHK_BUILD_SHARED` 指定是否生成动态库，默认为OFF。若启用，在项目中使用动态库时指定 `GLOBAL_HOTKEY_SHARED` 宏以获得更好的性能。
- `GBHK_DISABLE_REGISTER` 指定是否禁用生成注册型热键，默认为OFF。
- `GBHK_DISABLE_HOOK` 指定是否禁用生成Hook型热键，默认为OFF。
- `GBHK_BUILD_EXAMPLE` 是否生成示例程序，默认值取决于项目是否为主项目。
- `GBHK_EXAMPLE_USE_HOOK` 指定示例程序使用的热键类型，默认为OFF。

## 概念

- 对热键进行集中管理的对象被称为 **热键管理器 GlobalHotkeyManager**。
- 注册型热键代表由平台/系统进行管理的热键系统。
- Hook型热键代表由库通过Hook或注入技术进行按键监听实现的热键系统，其不存在热键冲突问题，可用于某些特殊场合。

## 使用

最基本的流程是：

1. 决定你需要使用的热键系统（注册型/Hook型）
2. 获取对应的 **热键管理器** 实例对象。
3. 调用 **热键管理器** 的 `start` 函数以启动。
4. 利用相关接口以添加、删除与变更热键。
5. 在程序结束或不需要使用热键系统时调用 `end` 函数以结束 **热键管理器**。

```cpp
GlobalHotkeyManager& ghm = RegisterGHM::getInstance();  // 获取 热键管理器 实例对象。

ghm.start();    // 启动 热键管理器。

// 添加、删除与变更热键。
KeyCombination kc1(MODI_CTRL | MODI_SHIFT, 'G');
KeyCombination kc2(MODI_ALT, 'J');
KeyCombination kc3(MODI_ALT, 'H');
ghm.add(kc1, callback1);
ghm.add(kc2, callback2);
ghm.replace(kc2, kc3);
ghm.remove(kc3);

// ...

ghm.end();    // 结束 热键管理器。
```

## 注意

- 目前仅支持Windows平台。
- **注册型热键管理器**的热键的增加、删除与变更必须在其启动后进行。
- `GBHK_DISABLE_REGISTER` 与 `GBHK_DISABLE_HOOK` 不可同时设为ON。
- 当 `GBHK_DISABLE_HOOK` 设为ON时，不可启用 `GBHK_EXAMPLE_USE_HOOK` 选项。
- 热键的回调函数不应执行繁重的任务，以免出现线程阻塞等异常情况，合理的做法是使用异步机制或消息队列（例如Qt中的信号槽）。
在Windows平台下使用Hook热键管理器时，应保证回调函数的执行时间在限定范围内。（详细信息参见 [Windows LowLevelKeyboard](https://learn.microsoft.com/zh-cn/windows/win32/winmsg/lowlevelkeyboardproc) ，其Remarks中提到Timeout时间为1000毫秒）
- 对于Windows平台下的Hook热键管理器，用户应保证安装Hook的线程（在此处，即是调用Hook热键管理器start函数的线程）具有消息循环。（详细信息参见 [Windows LowLevelKeyboard](https://learn.microsoft.com/zh-cn/windows/win32/winmsg/lowlevelkeyboardproc) ）

## 示例

[示例代码](example/example1.cpp)
