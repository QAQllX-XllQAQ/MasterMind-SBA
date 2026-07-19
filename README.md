# MasterMind-SBA

A C++ console Mastermind (Bulls and Cows) game.

## Gameplay

A secret code is generated (or set by Player 1 in PvP mode). The guesser has a limited number of rounds to deduce the correct digits and positions.

Each guess is scored with two metrics:

- **Position Correct** — correct digit in the correct position
- **Code Correct** — correct digit but wrong position

Guess all positions correctly to win.

## Features

| Feature | Description |
|---|---|
| **Standard Mode** | System generates a random code |
| **PvP Mode** | Player 1 sets the code, Player 2 guesses |
| **Difficulty Settings** | Code length 1–9, digit range 1–9, round limit adjustable, repeat toggle |
| **Save System** | 3 slots, binary serialization, save/load in-game |
| **Pause Menu** | Esc → Resume / Save / Exit |
| **Konami Code** | ↑↑↓↓←→←→BA — reveals the code during gameplay |

## Build

Windows — MinGW or Visual Studio.

```bash
g++ main.cpp -o mastermind.exe
mastermind.exe
```

## Controls

| Key | Action |
|---|---|
| `↑` / `↓` | Navigate menus |
| `←` / `→` | Adjust values / switch save slot mode |
| `Enter` | Confirm / submit guess |
| `Backspace` | Delete last digit |
| `Esc` | Back / pause menu |
| `1`–`9` | Input digits |
