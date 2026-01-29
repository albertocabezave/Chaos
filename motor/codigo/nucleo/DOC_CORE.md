Time Module: Technical Document
1. Purpose of the Module
The Time Module is responsible for providing the engine with precise, stable, and secure control over the simulation time. Its primary goal is to decouple the simulation behavior from the Operating System (OS) clock, ensuring consistent results regardless of machine performance or external events.
 * What it is NOT: It does not measure calendar dates, does not depend on the system wall clock, and does not control rendering or FPS.
 * Responsibility: Its sole responsibility is generating a reliable Delta Time for the engine logic.
2. Problem Statement
Modern operating systems use high-precision clocks that can:
 * Advance too fast if the program does not limit its execution.
 * Be affected by manual time changes or NTP (Network Time Protocol) synchronization.
 * Produce extremely large time deltas after pauses, system hangs, or freezes.
Using these raw values directly in a simulation causes sudden position jumps, physics engine "explosions," unstable numerical results, and non-reproducible behavior. The Time Module exists to shield the simulation from these conditions.
3. Design Principles
3.1 Monotonic Clock
The module uses std::chrono::steady_clock, which:
 * Never moves backward.
 * Is independent of the system date/time.
 * Is ideal for measuring durations reliably.
3.2 Real Time vs. Simulation Time Separation
The module distinguishes between Real Time (measured by the system clock) and Simulation Time (the time the engine chooses to process). The engine is not obligated to follow real time blindly.
3.3 Maximum Delta Clamping
To prevent extreme jumps, the system imposes a Maximum Delta limit.
 * Example: If the system reports a 2-second lag, the engine may only accept a maximum of 0.1 seconds. This ensures numerical and visual stability.
3.4 Pausing without Thread Blocking
The system allows pausing the simulation by returning a Delta Time of zero without stopping program execution. This keeps the UI active and the renderer running while preventing system lockups.
4. Responsibilities
| What it DOES | What it DOES NOT do |
|---|---|
| Measures time elapsed between frames. | Control FPS. |
| Calculates the Simulation Delta. | Sleep or block threads. |
| Clamps the maximum allowed Delta. | Render graphics or process input. |
| Manages Pause/Resume states. | Handle game/business logic. |
| Provides time scaling (Slow-mo/Fast-forward). | Know OS-specific implementation details. |
5. Public Interface
 * update(): Calculates time elapsed since the last call.
 * getDelta(): Returns the simulation delta in seconds.
 * pause() / resume(): Controls simulation flow without stopping the process.
 * isPaused(): Returns the current state.
 * setTimeScale(): Allows for safe slow-motion or fast-forwarding.
6. Computational Foundations
6.1 Hardware Clocks and TSC (Time Stamp Counter)
At the lowest level, the engine leverages the CPU cycle counter via C++ abstractions. Unlike the Real-Time Clock (RTC) which suffers from thermal drift, the Monotonic Clock ensures ticks are constant, preventing the engine from "traveling back in time" due to network syncs.
6.2 Time Quantization and Floating Point
The simulation translates nanosecond ticks (integers) into seconds (double). 64-bit doubles are chosen to minimize truncation errors in long-duration accumulations—critical for industrial simulations that may run for weeks.
6.3 Scheduling Jitter Compensation
OS schedulers (like Debian's) are not "hard" real-time. If the OS delays giving control back to the process, the Accumulator acts as a "buffer." It absorbs these time bumps so that the simulation logic (e.g., an industrial model) doesn't feel the computer's "stuttering."
6.4 Numerical Stability (Nyquist-Shannon Principle)
By enforcing a Fixed Timestep, we ensure the physics sampling frequency is higher than the frequency of the events we simulate. This prevents forces or pressures in the simulator from resonating and causing NaN (Not a Number) errors or infinities.
> Determinism: Thanks to this architecture, whether you run this engine on an old PC or a high-end workstation, the result of the calculation will be identical. In industry, we cannot allow a well to "explode" on one computer and not on another due to a millisecond error.
> 
Would you like me to help you implement the update() function in C++ following these principles, or perhaps create a header file for this module?
