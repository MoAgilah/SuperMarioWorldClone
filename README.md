# 🍄 SuperMarioWorldClone

A modern C++ reimagining of *Super Mario World*, rebuilt using [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine) and expanding on concepts from [**AI-Game-Controller**](https://github.com/MoAgilah/AI-Game-Controller).  
This project demonstrates the new engine’s scalability with enhanced player controls, AI-driven automation, and robust state management systems.

> **Status:** In active development — gameplay systems, AI controllers, and world logic are being incrementally rebuilt on the new architecture.

---

## 🎯 Goals & Purpose

- Showcase the **capabilities of SFMLEngine** in handling more complex, state-driven games beyond Pong.  
- Demonstrate the **integration of an AI controller** that mirrors human control inputs via the shared PlayerState system.  
- Provide a testbed for **expanding AI behaviour**, such as adaptive learning, environment awareness, and procedural decision-making.  
- Serve as a **blueprint for scalable 2D platformer architecture**, featuring clean code separation between engine, logic, and assets.  
- Continue the evolution of ideas first explored in **AI-Game-Controller**, now reimagined through a modular engine and improved abstractions.

---

## 📦 Download & Run

You can download the latest standalone executable here:  
👉 [**Download Standalone Executable**](https://raw.githubusercontent.com/MoAgilah/SuperMarioWorldClone/master/Bin/SuperMarioWorldClone.zip)

After downloading:

- Extract the `.zip` file.  
- Open the folder and run the executable:  
  - **Windows:** `SuperMarioWorldClone.exe`  
- Follow the instructions below to start and play.

---

## 🕹️ How to Start / Select

When you launch the game:

1. A **Loading Screen** will appear briefly while assets and levels load.  
2. The **Main Menu** will then display:  
   - **Play Manually** — control Mario directly.  
   - **AI Mode** — let the controller play automatically. *(WIP)*  
3. Use **↑ / ↓** to navigate and **Enter** to confirm your selection.

### Menu Flow
- Choose your **mode** → select **difficulty** (if available) → load the current world.  
- During play, you can pause, resume, or return to the title screen via the **Pause Menu**.

---

## 🌍 Global Actions

| Action                  | Key         |
|--------------------------|-------------|
| Open / Close Pause Menu  | **Space**   |
| Confirm / Select         | **Enter**   |
| Navigate Up / Down       | **↑ / ↓**   |
| Navigate Left / Right    | **← / →**   |
| Exit Game (any time)     | **Esc**     |

---

## ⏸️ Pause Menu

Accessible during gameplay by pressing **Space**.

**Options:**
- **Resume** – return to the game.  
- **To Title** – return to the main menu.  
- **Quit** – close the game entirely.  

Use **↑ / ↓** to navigate and **Enter** to select.

---

## 🎮 Player Controls

| Action | Key |
|--------|-----|
| Move Left / Right | ← / → |
| Jump | **A** |
| Spin Jump | **S** |
| Crouch | ↓ |
| Run / Hold | **Shift** |
| Pause | **Space** |
| Confirm / Select | **Enter** |
| Exit Game | **Esc** |

> These inputs are mirrored internally by the AI controller, ensuring identical movement and physics handling between human and automated play.

---

## 🤖 Future AI Mode

The **AI Mode** will leverage the updated **AI Controller** system introduced in [**AI-Game-Controller**](https://github.com/MoAgilah/AI-Game-Controller), rebuilt to take advantage of [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine)’s state-driven design.

### Planned Features
- **NEAT-inspired neural evolution** — AI agents will learn movement, timing, and survival through adaptive fitness-based learning.  
- **Shared input mapping** — AI uses the same input events as human players (*Jump, SpinJump, Move*, etc.).  
- **Environment awareness** — the agent will analyse tiles, enemies, and platforms to plan movement.  
- **Scalable training environments** — future worlds will serve as progressively challenging testbeds for evolving agents.


## 📁 Repository Structure
```
Pong/
├── Game/
│ ├── Code/ # Game-specific source code (entities, logic, main loop)
│ └── Resources/ # Game-specific assets (textures, sounds, etc.)
├── SFMLEngine/ # Submodule: the SFMLEngine repository
└── README.md
```

## ⚙️ Getting Started

### 1. Clone with Submodules
```
git clone https://github.com/MoAgilah/SuperMarioWorldClone.git
cd Pong SuperMarioWorldClone
git submodule update --init --recursive
```
### 2. Build

#### 🧰 Using Visual Studio
Simply open the solution (if provided) and build your desired configuration (**Debug** or **Release**).

## 🧩 How It Works

**SuperMarioWorldClone** is powered by the modular [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine), handling rendering, input, timing, and state transitions.  
The game logic sits above this layer, focusing purely on character control, AI decision-making, and scene progression.

---

### 🔗 Key Systems

- **Player & PlayerState** — A layered state machine controlling movement (*Lateral, Inclined, Vertical, Crouched, Dieing*) with shared physics for both manual and AI control.  
- **MainMenuState** — Entry point for selecting between **Manual Play** and **Automated AI Control**.  
- **PauseMenuState** — Global pause overlay with **Resume**, **Title**, and **Quit** actions.  
- **AI Controller** — Issues high-level movement intents (e.g. *MoveLeft*, *Jump*, *SpinJump*) through the same interface as manual input, ensuring parity between human and automated control.  

