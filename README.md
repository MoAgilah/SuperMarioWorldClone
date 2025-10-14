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
