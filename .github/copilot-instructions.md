
---

# ✨ GitHub Copilot Instructions for Xonix Game Project

## 📌 Purpose
These instructions help GitHub Copilot assist in generating **consistent**, **well-structured**, and **requirement-compliant** code for the **Xonix Game Enhancement Project**.

---

## 🛠️ Development Rules for Copilot
- **Programming Language:** C++
- **Graphics Library:** Only use `SFML/Graphics.hpp`.
- **Standard Libraries Allowed:** `<time.h>`, `<cmath>`, `<cstdlib>`, `<string>`, `<cstring>`.
- **Restricted:** ❌ Do NOT use `<vector>`, `<algorithm>`, built-in `list`, `queue`, `stack`, or any STL container-based utilities.
- **Code Complexity:** Use only **fundamentals of C++**, no advanced concepts.
- **Style:** Follow **clean code principles**:
    - Meaningful variable and function names.
    - Proper indentation.
    - Adequate comments explaining complex sections.
    - No magic numbers — use constants.

---

## 🧩 Features to Implement
> Implement each feature clearly, modularly, and separately.

### 1. Game Menus
- Create a **Start Menu**:
    - Options: Start Game, Select Level, Scoreboard.
- Create an **End Menu**:
    - Show Final Score (highlight if new high score).
    - Options: Restart, Main Menu, Exit Game.

### 2. Game Modes
- Single Player Mode.
- Two Player Mode (Player 1: Arrow Keys; Player 2: WASD).

### 3. Difficulty Levels
- **Easy:** 2 enemies.
- **Medium:** 4 enemies.
- **Hard:** 6 enemies.
- **Continuous Mode:** +2 enemies every 20 seconds.

### 4. Movement Counter
- Count moves **only** when a player starts **building tiles**.

### 5. Enemy Behavior
- Increase enemy speed every 20 seconds.
- After 30 seconds, half of the enemies must follow a **geometric pattern**:
    - **Zig-zag movement**.
    - **Circular movement**.
- Each pattern must be implemented in a **separate function** with clear comments.

### 6. Scoring System
- 1 point per captured tile.
- Bonus:
    - Capture >10 tiles in one move = 2x points.
    - After 3 bonuses, reduce threshold to 5 tiles.
    - After 5 bonuses, 5+ tiles = 4x points.
- Power-ups at 50, 70, 100, 130, +30 scores:
    - Freeze enemies (and other player) for 3 seconds.

### 7. Scoreboard
- Save top 5 high scores to a `.txt` file.
- Include player score + time.
- Update and sort automatically on game over.

### 8. Two Player Specific Rules
- Same board.
- Collision handling between players.
- Power-up freezes **both** enemies and **opponent**.
- Winner decided based on score when both eliminated.

### 9. Bonus Features (Optional for Extra Marks)
- Add sound effects.
- Change background color during power-up activation.

---

## 📋 Code Organization
- Divide code into **multiple files** if necessary (e.g., `Menu.h`, `Enemy.h`, `Player.h`, `Scoreboard.h`, etc.).
- Create **separate functions** for:
    - Menu handling.
    - Enemy movements.
    - Score handling.
    - Power-up management.
- Avoid writing everything inside the `main()` function.

---

## 📄 Report Writing Instructions
> Include these in a `Report.pdf` file.

- **Project Workflow:** Task distribution, timeline, issues faced.
- **Game Development Approach:** Design decisions.
- **Workflow Diagram:** Use `draw.io`, `Figma`, or `Lucidchart`.
- **Feature Implementation Status:** Describe if fully or partially completed.
- **Rationale for Creativity:** If you introduce anything new.

---

## ✅ Submission Checklist
- All source files, resource files (images, sounds), executable, and the report.
- Pack everything in a `.zip` file named:
    ```
    ROLL-NUMBER.DEPARTMENT.SECTION.zip
    (Example: 23i0001.DS.B.zip)
    ```
- Submit on **Google Classroom**.

---

# 🏆 Copilot Key Focus
**Prioritize:**  
- Feature completion ✅
- Clean code ✅
- Manual creativity where required ✅
- NO plagiarism or copy-paste from online examples ❌

---

