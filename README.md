# SpaceEngine 🚀

SpaceEngine is a custom 3D game engine and framework written entirely from scratch in **C++** and **OpenGL**. Developed for the PoliTo **Computer Graphics** project, it features a complete arcade-style Space Shooter game.

The project does not rely on commercial engines (like Unity or Unreal) but builds the entire rendering pipeline, memory management, collision system, and a scalable User Interface (UI) framework from the ground up.

---

## ✨ Technologies and Libraries
The project utilizes modern C++ libraries for media handling and mathematics:
* **OpenGL (Core Profile):** Graphics API for hardware-accelerated rendering.
* **GLFW & GLAD:** Window creation, OpenGL context management, and input handling.
* **GLM:** Mathematics library for 3D vectors and matrices.
* **Assimp:** Complex 3D model (`.obj`) importing.
* **FreeType:** Rasterization and rendering of TrueType (TTF) fonts for the UI.
* **OpenAL:** 3D audio management and spatialization.
* **stb_image:** Efficient texture loading (PNG, JPG).

---

## 🏛️ Architecture and Software Engineering
The core of the engine is designed with a strong focus on decoupling, performance, and clean code, leveraging several Design Patterns:

* **Deferred Memory Management:** The creation and destruction of game entities (enemies, bullets) use a *Deferred* approach (via `spawnQ` and `destroyQ` queues), processed at the end of the frame to prevent iterator invalidation and crashes during the Game Loop.
* **Observer Pattern:** The scoring system (`ScoreSys`) and the management of enemy spatial slots communicate in a decoupled way via events and callbacks, strictly adhering to the Single Responsibility Principle.
* **Fixed Time-Step Physics:** The Game Loop separates rendering from logical updates. Physics and collisions are calculated using a fixed-step time accumulator, ensuring deterministic behavior regardless of the client's framerate (FPS).
* **Data-Driven Game Director:** The enemy wave spawning system is implemented as a Data-Driven State Machine. Waves, spawn probabilities (weights), and physical limits (Slots) are configurable without altering hard-coded logic.
* **Hierarchical Spatial Hashing:** To optimize the *Broad-Phase* of collisions, the engine divides space into a multi-resolution grid, mapping entities via a Hash Table. This reduces collision checks from $O(N^2)$ to $O(1)$ access times, allocating memory only for space that is actually occupied.

---

## 🎨 Advanced Graphics Pipeline

* **Physically Based Rendering (PBR):** Custom shaders based on the Cook-Torrance BRDF equation. Lighting calculations support Albedo, Metallic, Roughness, Normal Mapping (with dynamic TBN matrix recalculation), and Ambient Occlusion.
* **HDR & Tone Mapping:** The scene is rendered into floating-point Frame Buffer Objects (FBO) and then mapped to the visible spectrum via an HDR exposure pass and Gamma Correction.
* **Bloom Post-Processing (MRT):** Using Multiple Render Targets, high-luminance pixels are extracted and blurred using a separable Gaussian Filter to create visually striking neon effects at a very low computational cost.
* **GLSL Subroutines:** Extensively used in the material system to dynamically change shader behavior at runtime (e.g., texture sampling vs. static value), eliminating heavy conditional `if/else` branching per pixel.
* **Optimized Depth-Testing:** The *Skybox* is rendered last by mathematically forcing $Z = W$ in the Vertex Shader, allowing Early-Depth Testing to discard fragments obscured by other objects and save fill-rate.

---

## 🖥️ Custom UI Framework
The engine includes a comprehensive system for building 2D menus:
* **Responsiveness:** Anchor-based system relying on the *Dirty Flag Pattern* to lazily recalculate element positioning only when the window resolution changes.
* **Lambda Callbacks** Interactive buttons are completely agnostic to the game state, receiving their logic (e.g., scene switching, volume adjustment) via Lambda functions.
* **Gamepad Support (Command Pattern):** The `UINavigator` system cleanly routes Keyboard and Controller(Joystick) inputs to navigate menus fluidly without the need for a mouse.

---

## 🛠️ Build Instructions (CMake)

Ensure you have **CMake** installed on your system. The project supports Out-Of-Source builds.

### 1. Configuration
Open the terminal in the project's root folder and run:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
### 2. Setup & Execution
⚠️ Important: To run the game, the executable needs its assets and dynamic libraries:
* Go to the output directory (e.g., build/Release/).
* Copy the entire assets/ folder next to the Main.exe file.
* Ensure all required .dll files (assimp.dll, freetype.dll, OpenAL32.dll, etc.) are in the same folder.
* Run Main.exe.
