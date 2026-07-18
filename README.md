# MasterMind-SBA

MasterMind 猜数字游戏 — C++ 控制台版。

## 玩法

系统生成一组不重复的 4 位数字（0-9），玩家每次猜 4 位数字，系统给出提示：

- **A**：数字和位置都正确
- **B**：数字正确但位置不对

例如：答案是 `1234`，猜 `1324` → 提示 `2A2B`。

## 编译运行

```bash
g++ main.cpp -o mastermind
./mastermind
```

Windows 下用 MinGW 或 Visual Studio 编译均可。
