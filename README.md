#  Coup Game Project

## About

This project implements a **C++ console and GUI-based version** of the popular card game **Coup**.

 **Features**:
- 6 unique character roles with special abilities
- Strategic gameplay: coins, actions, eliminations
- Dual interfaces: command-line **and** graphical (Qt GUI)

Players gather coins, use abilities, and outsmart opponents — until only one player remains victorious.

---

## Project Structure

### Core Game Classes
- **Game.hpp / Game.cpp**: Manages game logic, turns, coin pool, pending actions, reset.
- **Player.hpp / Player.cpp**: Handles player data (name, coins, role, status), actions, and state.
- **Role.hpp / Role.cpp**: Base class for roles, defines core actions: gather, tax, bribe, arrest, sanction, coup.

### Character Role Classes
- **Baron**: Invest 3 coins → get 6, compensation when sanctioned.
- **General**: Protects from coups, recovers coins when arrested.
- **Governor**: Tax 3 coins, cancel other players' tax.
- **Judge**: Cancel bribes, punish attackers.
- **Merchant**: Bonus income, special arrest behavior.
- **Spy**: See others' coins, block arrests.

### User Interface
- **Main.cpp**: Console demo showing all mechanics.
- **gui.cpp**: Qt-based GUI with buttons, visuals, intuitive play.
- **test.cpp**: Unit tests (Doctest) for all mechanics, edge cases.
- **Makefile**: Easy compilation and run commands.

---

## Key Features

### Game Mechanics
- 6 roles, each with special abilities
- Turn-based play with resource (coin) management
- **10+ coins rule**: forced coup
- Action validation and pending action tracking

### Character Abilities

| Role      | Special Abilities                                    |
|-----------|------------------------------------------------------|
| Baron     | Invest (3→6), sanction compensation                 |
| General   | Coup prevention, arrest immunity                    |
| Governor  | Tax 3 coins, cancel others' tax                     |
| Judge     | Cancel bribes, sanction retaliation                 |
| Merchant  | Bonus income, special arrest handling               |
| Spy       | See coins, block arrests                            |

### Technical Highlights
- Object-Oriented Design: inheritance, polymorphism
- Smart Pointers for memory safety
- Exception Handling for rules enforcement
- GUI (Qt) and Console interfaces
- Comprehensive unit testing with Doctest

---

## How to Compile and Run

Use the provided **Makefile**.

| Command         | Description                                     |
|-----------------|-------------------------------------------------|
| `make main`     | Compile & run console demo                     |
| `make gui`      | Compile & launch Qt graphical interface        |
| `make test`     | Run unit tests with Doctest                    |
| `make all`      | Build all executables                          |
| `make valgrind` | Run memory leak check (Valgrind)               |
| `make clean`    | Remove compiled files                          |
| `make help`     | Show available commands                        |

---

## Usage Instructions

### 1️⃣ Console Demo
- Automatic simulation of game mechanics
- Roles performing actions: gather, tax, bribe, arrest, sanction, coup
- Shows coin management and special abilities
- Full playthrough to victory + automatic reset

### 2️⃣ GUI Interface
- Add 2–6 players with random roles
- Real-time player status, coins, turn display
- Action buttons:
  - **Basic**: Gather, Tax, Bribe
  - **Aggressive**: Arrest, Sanction, Coup
  - **Role-specific**: Unique abilities
- Dropdowns for target selection
- Game state validation: enables/disables buttons based on rules
- Victory screen when one player remains

### 3️⃣ Unit Tests
- Tests core mechanics, special abilities, edge cases
- Validates:
  - Player actions & state management
  - 10+ coin coup enforcement
  - Role-specific behaviors
  - Error handling and game cleanup
- All tests should pass (green ✔)

### 4️⃣ Memory Leak Check
- Valgrind analysis during program run
- Expect: `All heap blocks were freed -- no leaks`
- Smart pointer use ensures safe memory handling

### 5️⃣ Build All Components
- Compiles demo, GUI, tests
- Confirms successful builds

### 6️⃣ Clean the Project
- Removes compiled `.o` files and executables
- Keeps source code and docs clean

---

## Game Rules Summary

| Action     | Description                                  |
|------------|--------------------------------------------|
| Gather     | Collect 1 coin                             |
| Tax        | Collect 2 coins (Governor: 3 coins)        |
| Bribe      | Pay 4 coins for an extra turn             |
| Arrest     | Steal 1 coin from another player          |
| Sanction   | Pay 3 coins to block gather/tax           |
| Coup       | Pay 7 coins to eliminate a player         |

- **Special Rules**:
  - 10+ coins → must perform coup
  - Sanctioned players → no gather/tax
  - Last player alive wins

---
 *This project showcases advanced C++ concepts like inheritance, polymorphism, smart pointers, exception handling, and GUI programming with Qt.*
