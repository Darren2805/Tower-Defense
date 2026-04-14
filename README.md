# Tower Defense | C++ & Allegro 5

A 2D tower defense game built using **C++** and the **Allegro 5** game development library.
Players strategically place turrets to defend against waves of enemies across multiple stages while managing resources, speed, and survival.

---

## Overview

This project demonstrates core concepts in **game development**, including:

* Real-time game loops
* Scene management system
* Object-oriented design
* Event-driven input handling
* Resource and asset management

The game features multiple UI scenes, different turret and enemy types, and a working scoreboard system.

---

## Features

### Game System

* Scene-based architecture:

  * Start Menu
  * Stage Select
  * Settings
  * Gameplay Scene
  * Win Scene
  * Lose Scene
  * Scoreboard Scene
* 2 playable stages
* Score tracking system
* Adjustable game speed
* Debug mode toggle

---

### Turrets

* Machine Gun Turret
* Laser Turret
* Missile Turret
* Shotgun Turret

Each turret has unique behavior and attack patterns.

---

### Enemies

* Soldier
* Plane
* Tank
* BigEnemy
* Enemy5

Different enemies introduce varied difficulty and strategy.

---

### Controls

#### Mouse

* Left Click → Select / place turret
* Left Click (invalid area) → Cancel placement

#### Keyboard

* **Q** → Machine Gun
* **W** → Laser
* **E** → Missile
* **R** → Shotgun
* **0–9** → Change game speed
* **TAB** → Toggle debug mode

---

## Project Structure

```text
Tower-Defense/
├── Bullet/
├── Enemy/
├── Engine/
├── Resource/
├── Scene/
├── Shared/
├── Turret/
├── UI/
├── CMakeLists.txt
├── main.cpp
├── windows.cmd
└── README.md
```

---

## Installation & Setup

### Windows (MSYS2 / CMake)

#### Requirements

* C++14 compatible compiler
* CMake ≥ 3.27
* Allegro 5

#### Allegro setup (Windows)

Expected directories:

```text
C:/allegro/include
C:/allegro/lib
```

---

### 🔨 Build (Windows)

#### Option 1 — Using script

```bash
windows.cmd
```

#### Option 2 — Manual CMake build

```bash
cmake -S . -B build
cmake --build build -j
```

---

### Linux / macOS

Install Allegro packages:

* allegro-5
* allegro_main-5
* allegro_font-5
* allegro_dialog-5
* allegro_primitives-5
* allegro_image-5
* allegro_ttf-5
* allegro_audio-5
* allegro_acodec-5

Then build:

```bash
cmake -S . -B build
cmake --build build -j
```

---

## How to Play

1. Launch the game
2. Select a stage
3. Place turrets strategically
4. Defend against incoming enemies
5. Manage money and survive all waves
6. Win by clearing all enemies
7. Lose if enemies reach the end

---

## Scoreboard System

* Scores stored in:

```text
Resource/scoreboard.txt
```

* Supports sorting and pagination
* Displays top scores across sessions

---

## Assets

All game resources are stored in:

```text
Resource/
```

Includes:

* Images
* Audio files
* Fonts
* Maps
* Enemy wave configurations
