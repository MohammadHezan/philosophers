*This project was developed as part of the 42 curriculum by mhaizan.*

# 🍝 42 Philosophers - Concurrency & Synchronization

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue.svg" alt="C Language" />
  <img src="https://img.shields.io/badge/OS-Linux%20%2F%20macOS-lightgrey.svg" alt="OS Support" />
  <img src="https://img.shields.io/badge/School-42-black.svg" alt="42 School" />
</p>

## 📖 Overview

The **Philosophers** project is a practical exercise in managing concurrent programming, based on the classic "Dining Philosophers" problem by Edsger Dijkstra. It serves as an introduction to the complexities of multi-threading, shared memory, and synchronization primitives.

### The Problem
A group of philosophers sits at a round table with a large bowl of spaghetti in the center. They alternate between three states:
- **Eating**: Requires holding two forks (the one on their left and the one on their right).
- **Sleeping**: Happens immediately after eating, returning the forks to the table.
- **Thinking**: Happens after waking up and lasts until they can eat again.

With exactly one fork between each philosopher, they must efficiently share resources. If everyone grabs the fork to their right simultaneously, they will all starve waiting for the left fork (a **deadlock**). 

### The Objective
Write a simulation in **C** using `pthreads` and `mutexes` that guarantees:
1. No philosopher starves (unless mathematically inevitable due to the provided arguments).
2. No deadlocks occur.
3. No data races exist (monitored via thread sanitizers).
4. The output logs perfectly reflect the state of the simulation without overlapping.

---

## ⚙️ Technical Architecture

This implementation is highly optimized to meet the strict timing and synchronization requirements of the 42 curriculum:

- **Thread Management (`pthread`)**: Every philosopher operates as an independent thread. A separate **Monitor Thread** continuously oversees the simulation to detect deaths with precision.
- **Micro-Polling Sleep (`ft_usleep`)**: The standard `usleep()` function is notoriously imprecise. This program uses a custom polling loop that wakes up in 0.5ms increments to accurately track time and immediately halt if a death is registered.
- **Deadlock Prevention (Staggered Starts)**: To prevent a circular wait, even-numbered philosophers are intentionally delayed at the start of the simulation by a small fraction of their eating time. This ensures forks are picked up asynchronously.
- **Log Synchronization**: A dedicated `print_mutex` protects the standard output. Once the monitor thread detects a death, it flags the simulation as over while holding the mutex, ensuring absolutely no states are printed after a philosopher has died.

---

## 🚀 Usage Guide

### Compilation
A `Makefile` is provided in the `philo/` directory.

To build the executable, run:
```bash
cd philo
make
```

### Execution
Run the program with the following arguments:
```bash
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```

**Arguments Details:**
1. `number_of_philosophers`: Total philosophers and forks.
2. `time_to_die` (ms): Max time a philosopher can survive without eating.
3. `time_to_eat` (ms): Time required to finish a meal (holding two forks).
4. `time_to_sleep` (ms): Time spent sleeping.
5. `[number_of_times_each_philosopher_must_eat]` *(Optional)*: The simulation will end if all philosophers eat this many times. If left blank, the simulation runs infinitely or until a death.

### Testing Examples

* **Infinite Survival:**
  ```bash
  ./philo 5 800 200 200
  ```
  *No philosopher should die.*

* **Inevitable Starvation:**
  ```bash
  ./philo 4 310 200 100
  ```
  *A philosopher will die around the 310ms mark.*

* **Single Philosopher:**
  ```bash
  ./philo 1 800 200 200
  ```
  *Will die at exactly 800ms since there is only one fork.*

* **Victory Condition:**
  ```bash
  ./philo 5 800 200 200 7
  ```
  *Simulation ends safely after everyone eats 7 times.*

---

## 📚 Study Resources

- [Code Vault: pthreads](https://www.youtube.com/playlist?list=PLGU1kcPKHMKi41Py2kqxdvqYE3M9VhCHe) - An essential YouTube playlist for understanding threads and mutexes.
- [Philosophers Visualizer](https://rom98759.github.io/Philosophers-visualizer/) - A great tool to visualize the problem and test argument outcomes.
- [POSIX Threads Programming](https://hpc-tutorials.llnl.gov/posix/) - Detailed documentation on thread behavior.

### AI Usage Disclosure
Artificial Intelligence was utilized as a development aid during this project to:
- Draft and structure this `README.md` file.
- Perform code review to ensure strict compliance with 42's Norminette formatting.
- Refine the mathematical precision of the custom `ft_usleep` function to guarantee <10ms accuracy during edge-case timing tests.
