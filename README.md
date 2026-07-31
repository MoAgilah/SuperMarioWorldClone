# SuperMarioClone

A modern C++ application that demonstrates modular platforming systems, layered application architecture and reusable engine integration.

SuperMarioClone is built as an application layer on top of EngineSFMLBackend and EngineInterface, recreating the platform simulation originally developed within AIGameController as a standalone, reusable application. Rather than combining platform mechanics, rendering, artificial intelligence and engine functionality into a single project, the application separates world management, character systems, physics, animation and gameplay into modular components that consume reusable engine services through well-defined architectural boundaries.

The project focuses on software engineering rather than recreating a commercial platform game, demonstrating how a modern platforming framework can be structured using scalable architecture, separation of concerns and reusable engine systems while providing the foundation for the next generation of AIGameController.

---

## The Engineering Problem

The original AIGameController tightly coupled platform simulation and artificial intelligence within the same application. While effective as a research project, this architecture limited reuse of both the gameplay systems and the AI framework, making future development increasingly difficult.

SuperMarioClone addresses this challenge by rebuilding the platform simulation as an independent application above EngineSFMLBackend and EngineInterface. This allows gameplay systems to evolve independently of artificial intelligence while providing stable interfaces that will allow the AIGameController to be reintroduced as a reusable submodule rather than remaining a standalone implementation.

---

## Application Architecture

<p align="center">
  <img width="700" alt="SuperMarioClone" src="https://github.com/user-attachments/assets/8070a3b2-7b34-4338-9968-cd2b577e9223" />
</p>

The application is organised into modular systems responsible for world management, scene composition, platform simulation, character behaviour and supporting utilities. Beneath the application layer, EngineSFMLBackend and EngineInterface are integrated as Git submodules, providing reusable engine services while remaining completely independent from gameplay implementation.

The long-term architecture also positions AIGameController as an optional submodule capable of controlling characters through shared gameplay interfaces without requiring changes to the platform simulation.

---

## Architecture Goals

The project was designed to demonstrate:

- Modular platforming architecture
- Layered application design
- Separation of gameplay and engine systems
- World and scene management
- Platform simulation systems
- Character and animation architecture
- Future AI module integration
- Scalable application structure

Although demonstrated through a side-scrolling platformer, these engineering principles are transferable to simulation software, robotics, interactive visualisations and larger C++ applications.

---

## Key Features

- Layered platforming architecture built on reusable engine modules
- Modular world and scene management
- Physics-driven character movement
- Collision detection and response
- Character animation systems
- Tile-based level framework
- Git submodule integration with EngineSFMLBackend and EngineInterface
- Designed to support future AIGameController integration as a reusable module

---

## Technologies & Engineering Practices

### Development

- C++20
- SFML 3
- Visual Studio 2022
- Git
- Git Submodules

### Engineering Practices

- Layered Architecture
- Modular Gameplay Systems
- Separation of Concerns
- State-Driven Design
- Scene Management
- Platform Simulation
- Object-Oriented Design
- Composition
- Reusable Components
- Engine Integration

---

## Architecture Highlights

- Platform simulation isolated from rendering implementation
- Modular world and scene management
- Dedicated character movement and animation systems
- Reusable collision and physics architecture
- Clean separation between application, engine and backend layers
- EngineSFMLBackend and EngineInterface integrated as reusable submodules
- Designed for future AIGameController integration through shared interfaces

---

## Engineering Outcome

SuperMarioClone demonstrates how a modern C++ platforming application can be developed on top of a reusable engine architecture while maintaining clear separation between gameplay systems and engine functionality.

By rebuilding the platform simulation independently of the original AIGameController, the project establishes a reusable gameplay framework capable of supporting future artificial intelligence, additional gameplay features and larger worlds without compromising the modular engine architecture. The result is a cleaner, more extensible foundation that validates both EngineSFMLBackend and EngineInterface while preparing the next generation of AIGameController as a reusable application module.

---
