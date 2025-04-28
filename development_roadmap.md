# 📝 Xonix Game Development Roadmap

## 🌟 Overview
This document outlines the development approach for creating an enhanced version of the classic Xonix game using C++ and SFML. The game will feature both single and multiplayer modes, with various gameplay mechanics as specified in the project requirements.

## 🔧 Development Environment Setup
1. **Windows (Development)**
   - Install MinGW or MSYS2 for C++ compilation
   - Install SFML library and set up include paths
   - Configure CMake for cross-platform building
   - Set up Git for version control

2. **Linux (Deployment Target)**
   - Ensure code compatibility with Linux environment
   - Test with the following packages:
     - cmake
     - build-essential
     - libsfml-dev
     - make

## 📊 Core Game Components

### 1. Game Board Structure
- Use 2D arrays to represent game board (avoiding vectors)
- Implement tile states (empty, filled, under construction)
- Create functions for board manipulation and rendering

### 2. Game Entities
- **Player(s)**: Position tracking, movement handling, collision detection
- **Enemies**: Position tracking, movement patterns, collision detection
- **Tiles**: State tracking (empty, filled, under construction)
- **Power-ups**: Tracking available power-ups and their effects

### 3. Game States
- Main menu
- Game play (single/multiplayer)
- Game over screen
- Scoreboard display

## 📈 Implementation Phases

### Phase 1: Basic Infrastructure (Week 1)
- Set up project structure with CMake for cross-platform compatibility
- Create window and game loop with SFML
- Implement basic rendering functions
- Design game board representation using arrays

### Phase 2: Core Gameplay Mechanics (Week 2)
- Implement player movement and trail creation
- Develop area filling algorithm
- Add basic enemy movement
- Implement collision detection
- Create game state handling functions

### Phase 3: Game Features (Week 3)
- Add difficulty levels
- Implement enemy behavior patterns
- Create scoring system
- Develop movement counter
- Implement timer functionality
- Add power-up system

### Phase 4: Menu System and UI (Week 4)
- Create start menu
- Implement level selection
- Develop game over screen
- Design and implement UI elements
- Create scoreboard display

### Phase 5: Two-Player Mode (Week 5)
- Extend game logic for second player
- Implement player interaction rules
- Add separate controls and scoring
- Modify power-up effects for multiplayer

### Phase 6: Finalization (Week 6)
- Implement file-based scoreboard system
- Add sound effects (optional bonus feature)
- Fix bugs and optimize performance
- Perform cross-platform testing
- Complete documentation and project report

## 🔄 Cross-Platform Compatibility
1. **Code Considerations**
   - Use forward slashes for file paths
   - Avoid Windows-specific functions
   - Use SFML's cross-platform functionality
   - Implement file operations with platform-agnostic methods

2. **Build System**
   - Use CMake for consistent building across platforms
   - Create clear build instructions for both Windows and Linux
   - Test builds on both platforms before finalizing

## 🧰 Implementation Strategy

### Data Structures
- Use static or dynamic arrays instead of vectors
- Implement custom linked lists for scoreboard management
- Create simple queue implementation for power-up inventory

### Functions Over Classes
- Use procedural programming approach
- Group related functionality into separate source files
- Use function pointers for different enemy movement patterns
- Implement state management through function-based state machine

### Memory Management
- Carefully allocate and deallocate dynamic memory
- Use appropriate scope for variables
- Implement proper cleanup in error scenarios

## ⏱️ Testing Strategy
1. Implement unit testing for core game functions
2. Test cross-platform compatibility regularly
3. Perform gameplay testing for balancing difficulty
4. Validate user input handling and edge cases
5. Test file operations for scoreboard functionality

## 📋 Final Checklist
- [ ] All required features implemented
- [ ] Code adheres to programming constraints
- [ ] Game runs on both Windows and Linux
- [ ] Documentation is complete
- [ ] Project report (4-6 pages) completed
- [ ] Code is well-commented and follows good practices
- [ ] Submission package prepared according to requirements

## 📝 Note on Library Usage
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

## 🔚 Conclusion
This roadmap provides a structured approach to developing the Xonix game using fundamental programming concepts without relying on classes or advanced data structures. By following this plan and maintaining cross-platform compatibility, we aim to create a fully functional game that meets all project requirements.
