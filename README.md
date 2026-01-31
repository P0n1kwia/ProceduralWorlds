#  ProceduralWorlds
##  Key Features

* **Multithreaded Terrain Generation:** Dynamic chunk-based terrain rendering. The system offloads heavy noise calculations to background threads to maintain high FPS during exploration.
* **L-Systems Vegetation:** A robust implementation of Lindenmayer systems. Uses string-based axioms and production rules to generate complex, organic tree structures.
* **Infinite Exploration:** Procedural seeding allows for virtually endless worlds generated on the fly.

##  Technical Stack
* **Language:**  [C++]
* **Graphics API:** [OpenGL ]


##  Roadmap & Work in Progress

### **TODO**
- [ ] **Level of Detail (LOD):** Implementing distance-based mesh simplification to optimize rendering.
- [ ] **Skybox Integration:** Adding a cubemap-based environmental background.
- [ ] **Chunk Stitching:** Resolving "seams" and gaps between terrain segments of different LOD levels.

### **Advanced Simulations**
- [ ] **Hydraulic Erosion:** Simulating rainfall and sediment transport to create realistic mountain ridges and valleys.

### **Known Issues & Bug Fixes**
- [ ] **L-Systems Persistence:** Fixing a bug where trees fail to regenerate on chunk reload.
- [ ] **Window Management:** Resolving Alt-Tab focus loss in fullscreen mode.

##  Media


##  Getting Started
1. Clone the repository.
2. Open the project in [Your IDE].
3. Run the "Main" scene/entry point.