*This project has been created as part of the 42 curriculum by mhaizan.*

## Description
The **Philosophers** project is a classic concurrency and synchronization problem based on Edsger W. Dijkstra's famous Dining Philosophers problem. 

The simulation models a group of philosophers sitting around a circular table doing three things: eating, thinking, and sleeping. The catch is that there is a bowl of spaghetti in the middle of the table, and to eat it, a philosopher must hold exactly two forks (one from their left, and one from their right). Since there are exactly as many forks as there are philosophers, they must share.

The primary goal of this project is to build a highly optimized, multi-threaded C program that perfectly manages resources (forks) using Mutexes (`pthread_mutex_t`). The code must run efficiently and safely to ensure that:
- **No data races occur.**
- **No deadlocks happen** (where everyone grabs one fork and stares at each other forever).
- **No philosopher starves to death** (unless the simulation timing mathematically demands it).

## Instructions

### Compilation
The project includes a `Makefile`. To compile the source code into the final executable, navigate to the root directory and run:
```bash
make
```
Other available rules:
- `make clean` — Removes object files (`.o`).
- `make fclean` — Removes both object files and the `philo` executable.
- `make re` — Cleans and recompiles the project from scratch.

### Execution
Run the `philo` executable with the following mandatory arguments:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

**Arguments Overview:**
- `number_of_philosophers`: The number of philosophers at the table (and the number of forks).
- `time_to_die` (in milliseconds): The maximum time a philosopher can survive without eating. If a philosopher hasn’t started eating before this limit is reached since their last meal or the start of the simulation, they die.
- `time_to_eat` (in milliseconds): The time a philosopher spends eating (they lock two forks during this entire duration).
- `time_to_sleep` (in milliseconds): The time a philosopher spends sleeping after dropping their forks.
- `[number_of_times_each_philosopher_must_eat]` *(optional)*: If provided, the simulation ends in victory when every philosopher has eaten at least this many times. If not provided, the simulation runs infinitely until someone dies.

**Usage Example:**
```bash
# 5 philosophers, they die if they don't eat within 800ms. 
# They take 200ms to eat, 200ms to sleep. 
# The simulation stops when everyone has eaten 7 times.
./philo 5 800 200 200 7
```

## Resources

**Classic References on Concurrency:**
- [POSIX Threads (pthreads) Documentation](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Mutex Locks and Concurrency in C](https://hpc-tutorials.llnl.gov/posix/)
- [The Dining Philosophers Problem Overview (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

**AI Usage Report:**
Artificial Intelligence was utilized lightly as a debugging and formatting tool to streamline the development process. Specifically, AI assisted with:
- Checking code formatting to ensure compliance with the strict 42 Norminette standards.
- Identifying and explaining minor edge-case scheduling delays caused by the WSL environment.
- Generating the initial template for this README file.
