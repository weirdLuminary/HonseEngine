# Honse Engine

## Overview

**Honse Engine** is a small hobby 2D game engine project. It is not intended to be used to make games by people other than myself, but can be used as opportunities for learning. It uses the GNU AGPLv3 license.

It combines many systems, such as ECS, batch rendering via a manually written OpenGL renderer with object wrappers, audio system based on FMOD, resource manager, and a multithreading system.

## Libraries

Honse Engine uses chunks of code from a few helper libraries, all of which can be found online. 

| Library | Source |
| --- | --- |
| LECS | Custom-made; only present in the current project. |
| GLFW | [Official website](https://www.glfw.org/) |
| GLM | [Github repository](https://github.com/g-truc/glm) |
| STB (Image utilities) | [Github repository](https://github.com/nothings/stb/) |
| GLAD | [Github repository](https://github.com/Dav1dde/glad) |
| FMOD | [Official website](https://www.fmod.com/) |


## TODO

**Graphics:**
 - (Later) Post-processing
 - (Later) Render multiple shaders 
 - Animated sprites

**Localization:**
 - Create localization string
 - *TBA*

**General:**
 - Scene Management (how to load/unload/save or serialize/deserialize)
 - Audio system
 - Configuration/save file creation
 - Event system