# prog4-game-engine

A modular game engine project built for the Programming 4 course.

GitHub Repository: [https://github.com/alessandromanzini/prog4-game-engine](https://github.com/alessandromanzini/prog4-game-engine)

## 🎮 Controls Overview

---

### 🕹️ Gameplay

| Action        | Keyboard          | Controller           |
|---------------|-------------------|----------------------|
| Move          | `A` / `D`         | D-Pad Left / Right   |
| Jump          | `Space`           | A Button             |
| Attack        | `K`               | X Button             |
| Skip Level    | `F1`              | –                    |
| Toggle Mute   | `F2`              | –                    |

### 📋 Menu Navigation

| Action        | Keyboard          | Controller           |
|---------------|-------------------|----------------------|
| Join Game     | `J`               | X Button             |
| Leave Game    | `Q`               | B Button             |
| Mode Select   | `A` / `D`         | D-Pad Left / Right   |
| Confirm       | `E`               | A Button             |
| Quit          | `Escape`          | Start Button         |

### 🏆 Leaderboard Input

| Action           | Keyboard             | Controller             |
|------------------|----------------------|------------------------|
| Name Selection   | `W`, `A`, `S`, `D`   | D-Pad (All Directions) |
| Confirm          | `E`                  | A Button               |

---

This game offers a fluid and intuitive control scheme designed for both keyboard and controller users. Once linked to an ID and the relative bindings, the input mappings remain consistent and responsive across multiple devices. Through the use of the Controller class, the user can reserve a specific device accessing all and only the inputs of that device.

The input system allows every action to be flexibly bound to custom logic using the compile-time visitor mechanism based on `std::variant`. This means that developers can link any input action to methods with any parameter types—ensuring maximum flexibility with type safety, minimal boilerplate and correct type cohercion. 
### 🔧 Input Modifiers

The system supports **modifiers**, which are additional conditions attached to input actions that alter how inputs are interpreted. For example, **NEGATE** can act reverse the input value received, while **SWIZZLE** changes the axis the input is being fed to, allowing to receive a 4-directional input in a single callback function that is only called once per frame.

### 🔧 Input Triggers

The system also features **triggers**, allowing the user to decide when a certain action should be performed... either on **TRIGGER**, **PRESS**, or **RELEASE**.

