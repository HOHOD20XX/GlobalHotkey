# Global Hotkey

[English](README_EN.md)

## 特点

- 可用于任意类型的程序（甚至是控制台程序）

  例如Qt、MFC、命令行等等。

- 支持使用Hook实现的按键监听替代注册型热键。
- 支持Hook按键监听的去抖动。
- 良好的异常处理机制。
- 虚拟键枚举。
- 组合热键支持。

## 编译

```console
$ git clone https://github.com/JaderoChan/GlobalHotkey
$ cd GlobalHotkey
$ cmake -S ./ -B ./build
$ cd build
$ make
```

#### 可用选项

- `GBHK_BUILD_SHARED` 指定是否生成动态库，默认为OFF。若启用，在项目中使用动态库时指定 `GLOBAL_HOTKEY_SHARED` 宏以获得更好的性能。
- `GBHK_DISABLE_REGISTER` 指定是否禁用生成注册型热键，默认为OFF。
- `GBHK_DISABLE_HOOK` 指定是否禁用生成Hook型热键，默认为OFF。
- `GBHK_BUILD_EXAMPLE` 是否生成示例程序，默认值取决于项目是否为主项目。
- `BUILD_EXAMPLE_USE_HOOK` 指定示例程序使用的热键类型，默认为OFF。

## 概念

- 对热键进行集中管理的对象被称为 **热键管理器**，所有继承于 `GlobalHotkeyBase` 的子类都属于 **热键管理器**。
- 注册型热键代表由平台/系统进行管理的热键系统。
- Hook型热键代表由库通过Hook或注入技术进行按键监听实现的热键系统，其不存在热键冲突问题，可用于某些特殊场合。

## 使用

最基本的流程是

1. 决定你需要使用的热键系统（注册型/Hook型）
2. 获取对应的 **热键管理器** 实例对象。
3. 调用 **热键管理器** 的 `start` 函数以启动。
4. 利用相关接口以添加、删除与变更热键。
5. 在程序结束或不需要使用热键系统时调用 `end` 函数以结束 **热键管理器**。

```cpp
GlobalHotkeyBase* instance = RegGlobalHotkey::getInstance();  // 获取 热键管理器 实例对象。

instance->start();    // 启动 热键管理器。

// 添加、删除与变更热键。
KeyCombination kc1(CTRL | SHIFT, 'G');
KeyCombination kc2(ALT, 'J');
KeyCombination kc3(ALT, 'H');
instance->add(kc1);
instance->add(kc2);
instance->replace(kc2, kc3);
instance->remove(kc3);

// ...

instance->end();    // 结束 热键管理器。
```

## 注意

- 目前仅支持Windows平台。
- 热键的增加、删除与变更必须在相应的 **热键管理器** 启动后进行。
- `GBHK_DISABLE_REGISTER` 与 `GBHK_DISABLE_HOOK` 不可同时设为ON。
- 当 `GBHK_DISABLE_HOOK` 设为ON时，不可启用 `BUILD_EXAMPLE_USE_HOOK` 选项。

## 示例

[示例代码](example/example1.cpp)
