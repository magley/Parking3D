# Parking3D

Parking3D is a simulation/game made from scratch in C++ and OpenGL.

![demo](./docs/demo.gif)

Most of the textures were downloaded from the internet. The SFX are from Half Life and FNaF.

## Engine

Parking3D is written as a game engine.

It's built from "subsystems", each being a global variable available in the `glo` namespace. The subsystems can be `#include`-ed individually so as to reduce compile time in incremental builds.

- The **audio** subsystem can play and loop sound effects.

- The **camera** subsystem represents the abstract camera whose transformations are used in vertex processing.

- The **entity** subsystem implements an array of sparse entities.

- The **event** subsystem handles events which are implemented as flags.

- The **game** subsystem handles higher level logical constructs specific for `Parking3D`

- The **input** and **window** subsystems provide a facade to a subset of GLFW's API for keyboard and mouse input and handling windows.

- The **resource** subsystem uses the resource manager. Resources are identified by their filename and are loaded on demand. At the moment, resources cannot be freed.

## Graphics features

- Multiple point lights and spotlights
- Diffuse maps (textures)
- Specular, emission and normal maps
- Non-opaque objects
- TV static effect
- 2D HUD and minimal 2D animated textures
- Alternative render targets