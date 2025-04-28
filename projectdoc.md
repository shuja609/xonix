
---

# 📜 Project Title: **Xonix Game – Advanced Version**

## 🎯 Objective
Develop an enhanced version of the Xonix game using **C++** and **SFML**, incorporating both **single-player** and **two-player** modes with additional features like dynamic difficulty, scoring systems, power-ups, and a file-based scoreboard.  
All development must strictly follow good programming practices, avoid plagiarism, and prioritize creativity and proper documentation.

---

## 🏗 Development Environment
- **Language**: C++
- **Graphics Library**: SFML
- **Tools Required**:
  - `cmake`
  - `build-essential`
  - `libsfml-dev`
  - `make`
- **Platform**: Linux

### Building Instructions:
```bash
sudo apt update
sudo apt install cmake build-essential libsfml-dev make
mkdir build
cd build
cmake ..
make
```
Executable will be available in the `build/` folder.

---

## 📦 Project Submission
- **Deliverables**: 
  - Source code
  - Buildable project
  - Project Report (minimum 4–6 pages)
  - Final `.zip` file

- **File Naming Format**:
  ```
  ROLL-NUMBER.DEPARTMENT.SECTION.zip
  (e.g., 23i0001.DS.B.zip)
  ```

- **Submission Platform**: Google Classroom

---

## 🛠 Project Features and Requirements

### 1. Basic Gameplay Mechanics (Already Implemented)
- Movement: Up, Down, Left, Right.
- Building tiles: no reversal while building (reverse movement causes death).
- Collision: If enemy touches under-construction tile → player dies.

---

### 2. New Functional Requirements

#### A. Game Menus
- **Start Menu**:
  - Start Game
  - Select Level
  - Scoreboard
- **End Menu**:
  - Display final score (highlight if new high score)
  - Options: Restart | Main Menu | Exit

#### B. Difficulty & Enemy Count
- Level Selection:
  - Easy – 2 enemies
  - Medium – 4 enemies
  - Hard – 6 enemies
  - Continuous Mode – 2 enemies initially, +2 every 20 seconds.

#### C. Movement Counter
- Track and display the number of moves (each building movement counts).

#### D. Enemy Behavior
- Track and display elapsed game time.
- Every 20 seconds:
  - Increase enemy movement speed.
- After 30 seconds:
  - Half of the enemies switch to **geometric movement patterns** (not random):
    - **Pattern 1**: Zig-zag motion
    - **Pattern 2**: Circular motion
  - Implement patterns in separate, well-documented functions.

#### E. Scoring and Reward System
- 1 tile captured = 1 point.
- Capturing >10 tiles in one move:
  - Earn **2× points**.
  - **Bonus Reward Counter**:
    - After 3 bonuses: Threshold lowers to 5 tiles.
    - After 5 bonuses: Capturing 5+ tiles = **4× points**.
- **Power-ups**:
  - Unlock at 50 points (and 70, 100, 130, then every 30 points).
  - Power-up: Freeze all enemies for 3 seconds.
  - Unused power-ups are stackable in inventory.

#### F. Scoreboard System
- Maintain a `.txt` file:
  - Top 5 high scores with elapsed time.
  - Automatically update after game over if eligible.

---

### 3. Two-Player Mode
- **Controls**:
  - Player 1: Arrow Keys
  - Player 2: WASD
- **Rules**:
  - Shared game board and timer.
  - Individual scores and inventories.
  - Interaction Rules:
    - Both players building: collision = both die.
    - One player touching another's construction = toucher dies.
    - Player touching another player (who’s not building): builder dies.
- **Power-ups**: Freeze both enemies **and** opponent for 3 seconds.
- **Game End**: 
  - When both players are eliminated.
  - Winner = higher score.

---

### 4. Additional Documentation
- **Project Report** (4–6 pages minimum):
  - Overall Project Overview
  - Game Development Strategy
  - Workflow Diagram (use Draw.io, Figma, Lucidchart)
  - Task Division (if team project)
  - Implementation Progress Report for Each Feature
  - Creativity or Special Notes

---

### 5. Bonus Features (Optional for Extra Marks)
- Sound effects (capturing tiles, activating power-ups, earning bonuses).
- Background color change upon power-up activation.

---

## 📋 Programming Constraints
**Restricted Libraries** (NOT Allowed):
- `<vector>`
- `<algorithm>`
- Built-in list, queue, stack data structures

**Allowed Libraries**:
- `<SFML/Graphics.hpp>`
- `<time.h>`
- `<cmath>`
- `<cstdlib>`
- `<string>`
- `<cstring>`

---

## 🔥 Important Notes
- **Zero tolerance** for **plagiarism**.
- Use **meaningful variable names**, **comments**, **indentation**, and **readable code**.
- **Effort matters more** than complete functionality.
- Creativity, originality, and adherence to guidelines can earn **bonus marks**.

---

# ✅ Summary Checklist
| Task                         | Status   |
| ----------------------------- | -------- |
| Setup Build Environment       | ⬜        |
| Implement Start & End Menus    | ⬜        |
| Implement Difficulty Levels    | ⬜        |
| Implement Movement Counter     | ⬜        |
| Enhance Enemy Behavior         | ⬜        |
| Implement Scoring System       | ⬜        |
| Create and Maintain Scoreboard | ⬜        |
| Develop Two-Player Mode        | ⬜        |
| Add Bonus Features             | ⬜        |
| Write Final Report             | ⬜        |
| Prepare .zip File for Submission | ⬜      |

---

How to Build a Game on Linux: 
Download source code, open terminal in the same folder, and execute the following commands 
·         
·         
·         
·         
·         
·         
·         
sudo apt update 
sudo apt install cmake 
sudo apt install build-essential 
sudo apt install libsfml-dev 
mkdir build 
cd build 
cmake .. 
· make (use “sudo apt install make” if not already installed on your system) 
Now open build folder and there is an executable file with name xonix. After First 
compilation only make command need to compile code.