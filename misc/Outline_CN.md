# GlobalHotkey 大纲

## 基本概念

- 全局热键管理器（Global Hotkey Manager）：一个单例全局热键管理器，可以管理多个全局热键的增加、移除、替换，以及热键触发后的回调函数执行。

## 接口

### Key 模块

#### 枚举

- `ModifierFlag : int`

  修饰键枚举。可以通过位或运算符组合多个枚举值获得一个修饰键组合值，其可作为`int`类型数据对`Modifiers`进行构造。

- `KeyFlag : int`

  键枚举。字母键与数字键枚举值等同于其ASCII码值。

#### 字面值常量类

- `Modifiers` 修饰键组合类。
- `Key` 键类。

#### 函数

- `String modifiersString(Modifiers modifiers, bool isPrettySpace = false)`

  获得平台相关的给定修饰键组合的描述字符串。

  **参数**

  - `isPrettySpace`指示是否应该在各个修饰键字符串的连接符两侧插入空格符。

        例如：
        其为假`false`时，可能的结果是`Ctrl+Shift+A`
        其为真`true`时，可能的结果是`Ctrl + Shift + A`

- `String keyString(Key key)`

  获得平台相关的给定键的描述字符串。

- `Modifiers getModifiersFromString(String string)`

  从给定字符串中解析出修饰键组合。

- `Key getKeyFromString(String string)`

  从给定字符串中解析出键。

- `int getNativeModifiers(Modifiers modifiers)`

  获得平台相关的修饰键组合对应的原生修饰键值。

- `int getNativeKey(Key key)`

  获得平台相关的键对应的原生键值。

### KeyCombination 类

- `KeyCombination(Modifiers modifiers, Key key)`

  使用给定的修饰键组合和键构造一个组合键`KeyCombination`对象。

- `static KeyCombination fromString()`

  从给定字符串中解析出组合键。

- `Getter and Setter for modifiers, key`
- `Getter for native modifiers and key`

- `bool isValid()`

  检查组合键是否有效。

  具体来说，对于一个有效组合键，其应该满足以下两个条件：

        1. 修饰键组合至少含有一个有效修饰键值。
        2. 键值不为0。

- `String toString(bool isPrettySpace = false, bool showKeyValue = false)`

  获得平台相关的组合键的描述字符串。

  **参数**

  - `showKeyValue`指示是否应该在描述字符串中显示键的具体数值。（用于调试）

- `Hash and Equal Compare operator.`

### Hotkey Manager 单例类

- `int start()`

  启动热键管理器实例。

- `int end()`

  停止热键管理器实例。

  **注意**

  热键管理器被销毁时，其所维护的所有全局热键都会被移除。

- `int add(KeyCombination kc, void (*func)())`

  增加一个全局热键至热键管理器，其回调函数为无参空返回值函数。

- `int add(KeyCombination kc, void (*func)(void*), void* arg)`

  增加一个全局热键至热键管理器，其回调函数为有参空返回值函数。

- `int remove(KeyCombination kc)`

  从热键管理器中移除给定全局热键。

- `int replace(KeyCombination oldKc, KeyCombination newKc)`

  替换热键管理器中给定全局热键的热键组合。

- `bool isRunning()`

  检查热键管理器是否正在运行。

- `void setCycleTime(size_t millisecond)`

  设置热键管理器工作线程循环的周期时间（在工作线程中，单次循环的最大时间）。

  **参考值**

  5-100ms

  **注意**

  此函数并不决定热键管理器的工作线程单次循环的实际运行时间，实际运行时间受到系统性能、热键数量和热键触发回调函数执行时间等因素的影响，且只会大于等于设定时间。通常来讲，在系统性能良好，回调函数执行时间较短的情况下，实际运行时间与设定时间接近，在系统负载较高的情况下，实际运行时间可能长于设定时间。

- `void setDebouncedTime(size_t millisecond)` (Only for hook hotkey manager.)

  设置热键管理器的防抖时间。

  **参考值**

  50-150ms

### Key Board Hook 模块

#### 枚举

- `KeyState`

  键状态枚举。

#### 函数

- `int start()`

  启动键盘Hook。

- `int end()`

  停止键盘Hook。

- `int addKeyListener(int nativeKey, KeyState state, void (*func)()))`

  增加一个键监听器，其回调函数为无参空返回值函数。

- `int addKeyListener(int nativeKey, KeyState state, void (*func)(void*), void* arg)`

  增加一个键监听器，其回调函数为有参空返回值函数。

- `int removeKeyListener(int nativeKey, KeyState state)`

  移除一个键监听器。

- `int setKeyPressedEvent(void (*func)(int nativeKey)))`

  设置键按下事件。

- `int setKeyReleasedEvent(void (*func)(int nativeKey)))`

  设置键释放事件。
