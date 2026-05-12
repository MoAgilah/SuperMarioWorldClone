# SuperMarioWorldClone

A modern **C++ 2D platformer project** inspired by *Super Mario World*, built using the modular [SFMLEngine](https://github.com/MoAgilah/SFMLEngine) framework.

The project focuses on scalable gameplay architecture, reusable systems, AI-driven automation, collision handling, animation systems, and state-driven game design while rebuilding and extending concepts originally explored in *AI-Game-Controller*.

> **Status:** Active development — gameplay systems, AI controllers, world logic, and engine integration are being incrementally expanded.

---

## 🧠 Overview

SuperMarioWorldClone explores how a larger-scale platformer can be structured using modular engine systems and clean gameplay separation.

The project demonstrates player movement systems, collision handling, animation pipelines, tilemap-driven worlds, state management, and AI-ready input systems designed to support both manual and automated gameplay.

---

## ⚙️ Key Features

- Player movement and platforming controls
- Collision and physics systems
- Tilemap and level management
- Animation and sprite rendering systems
- State-driven architecture
- Scene and menu management
- Collectables and scoring systems
- Modular gameplay architecture
- Shared player input abstraction for AI integration
- AI-ready controller systems
- Built on top of SFMLEngine

---

## 🤖 AI Integration

The project is designed to support future AI-driven gameplay using systems evolved from the *AI-Game-Controller* project.

Planned AI systems include:

- Shared input mapping between human and AI players
- NEAT-inspired evolutionary learning
- Environment-aware movement and navigation
- Adaptive gameplay behaviour
- AI debugging and visualisation overlays

The AI controller interacts with the same gameplay input systems used by manual players to ensure consistent physics and movement behaviour.

---

## 🧱 Architecture

The project separates engine functionality, gameplay systems, rendering, and AI behaviour into modular components.

```text
SuperMarioWorldClone/
├── Game/
│   ├── Code/         → Gameplay systems and logic
│   ├── Resources/    → Sprites, audio, and assets
│   └── Levels/       → Tilemaps and world data
├── SFMLEngine/       → Engine submodule
└── README.md
```

### Core Systems

- **Player & PlayerState** — layered movement and gameplay state handling
- **MainMenuState** — menu and mode selection flow
- **PauseMenuState** — global pause and navigation overlay
- **AI Controller** — automated gameplay input abstraction
- **Tilemap Systems** — level loading and collision support
- **Animation Systems** — sprite and state-driven animation handling

---

## 🛠 Technologies

- C++
- SFML
- SFMLEngine
- Visual Studio
- Object-oriented programming
- State-driven architecture
- Collision detection and physics
- AI gameplay systems

---

## 🚀 Getting Started

### Clone with Submodules

```bash
git clone https://github.com/MoAgilah/SuperMarioWorldClone.git
cd SuperMarioWorldClone
git submodule update --init --recursive
```

### Build

1. Open the Visual Studio solution.
2. Select **Debug** or **Release**.
3. Build and run the project.

A standalone executable will be provided once manual debug mode integration is completed.

---

## 🔭 Future Work

- Complete manual debugging mode integration
- Expand AI controller systems and adaptive learning
- Add additional worlds and level transitions
- Introduce enemies and environmental hazards
- Integrate audio and music systems
- Add save/load functionality
- Implement AI debugging overlays and visualisation tools
- Expand gameplay systems and engine tooling

---
