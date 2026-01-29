1. Engine Overview
​The main program (Engine) controls the simulation loop. A Time module measures real intervals between interactions and provides a simulation delta (in seconds). The Engine accumulates this delta and executes the Application logic in fixed steps to achieve stable and reproducible results.
​2. System Components and File Mapping
​Entry Point:
​motor/main.cpp – Creates the Engine and triggers run()
​Engine Core (Scheduler and Main Loop):
​motor/code/core/engine.h (Declarations)
​motor/code/core/engine.cpp (Loop implementation)
​Time Module (Interval measurement, clamping, scaling, and pausing):
​motor/code/core/time.h (Interface)
​motor/code/core/time.cpp (Implementation)
​Application (Basic Simulation):
​motor/code/core/app.h
​motor/code/core/app.cpp
​3. Execution Flow (Step-by-Step)
​main → creates Engine → Engine::run()
​In each iteration:
​I. Engine calls time.update() (which uses steady_clock to measure elapsed real time).
​II. Engine requests time.getDelta() and adds that value to the accumulator.
​III. While accumulator >= FIXED_STEP (e.g., 1/60 s), it executes {application.update(FIXED_STEP)} one or more times.
​IV. Result: The application logic always receives a constant FIXED_STEP, ensuring numerical stability.
​4. Hardware and Operating System Considerations
​CPU:
​The engine is CPU-bound during intensive numerical calculations. For a digital twin with complex models, higher core counts and faster clock speeds are beneficial.
​For offline runs, prioritize fast cores and large cache. You can run multiple experiments in parallel (each in a separate process) to leverage multi-core architecture.
​Memory (RAM):
​Models with many elements (meshes, plant states) consume RAM. Plan capacity based on model size and checkpoint frequency.
​Storage:
​Logs and checkpoints should be stored on a fast drive (SSD). Example: periodically saving states and time-series data.
​System Clock:
​The software uses std::steady_clock (which is monotonic and does not depend on the "wall clock"). OS clock synchronization is not required for simulation accuracy.
​Network:
​Designed for offline use; no real-time network dependency. Data transfer (uploads, backups) can be handled via physical media or an isolated network.
​OS Environment:
​Linux is recommended for long-running simulations due to its scripting, automation, and containerization capabilities. Use modern g++ (C++17).
​Backup and Redundancy:
​Regular backups of checkpoints and the source code repository must be configured.
​5. Troubleshooting Guide
​Common issues and where to find them:
