
# 100 Prisoners Problem Solution with Multithreading

This repository contains a C program that simulates the 100 Prisoners Problem, using multithreading to model each prisoner’s independent actions. This program compares two main strategies for solving the problem: a "smart" loop-based strategy and a "random" strategy. Each strategy has two variations—one using a global mutex and the other using drawer-specific locks for concurrency control.

## Problem Description

The 100 Prisoners Problem is a probability puzzle where:
- 100 prisoners must each find their own number hidden in one of 100 drawers.
- Each prisoner is allowed to open up to 50 drawers.
- For the prisoners to succeed and be freed, every prisoner must find their assigned number within the 50-drawer limit.

The program implements two primary strategies to explore this problem and includes variations for managing concurrency:

1. **Smart (Loop-Based) Strategy**: Each prisoner follows a sequence of drawers based on the contents of each drawer, forming a loop that often leads to their number efficiently.
2. **Random Strategy**: Each prisoner selects drawers at random up to 50 times, aiming to find their number without a predefined path.

## Strategies

### 1. Smart (Loop-Based) Strategy

In this strategy, each prisoner begins by opening the drawer corresponding to their own number (e.g., prisoner 1 opens drawer 1).
- If they do not find their number, they follow the number found in each drawer as the next drawer to open.
- This loop-based strategy increases the probability of success by leveraging a deterministic search path.

Two variations of the smart strategy are implemented:
- **Global Lock (`smart_global`)**: Uses a single global mutex (`lock_global`) to control access, ensuring thread safety across all drawers.
- **Drawer-Specific Lock (`smart_drawer`)**: Each drawer has its own lock (`lock_drawer`), reducing contention by only locking drawers currently in use.

### 2. Random Strategy

In this strategy, each prisoner opens drawers at random up to 50 times.
- The prisoner selects a new random drawer if they do not find their number immediately.

Two variations of the random strategy are implemented:
- **Global Lock (`random_global`)**: Controls access using a single global mutex (`lock_global`), ensuring thread safety during drawer access.
- **Drawer-Specific Lock (`random_drawer`)**: Each drawer has a specific lock (`lock_drawer`), allowing concurrent access to different drawers and reducing thread contention.

## Program Structure

- **`drawers` Array**: Stores the random distribution of numbers across drawers.
- **`prisoners` Array**: Tracks whether each prisoner successfully found their number.
- **Multithreading**: Uses POSIX threads to simulate each prisoner’s search concurrently.

## Key Functions

- **`run_threads(int n, void* (*proc)(void*))`**: Creates and manages threads for each prisoner, running the specified strategy.
- **`timeit(int n, void* (*proc)(void *))`**: Measures the execution time of a strategy for performance comparison.
- **`random_global(void* number)`** and **`random_drawer(void* number)`**: Implement the random strategy with global and drawer-specific locking, respectively.
- **`smart_global(void* number)`** and **`smart_drawer(void* number)`**: Implement the smart loop-based strategy with global and drawer-specific locking, respectively.

## Command-Line Options

The program includes two adjustable parameters that can be set via command-line options:

- `-n <num_of_games>`: Specifies the number of simulation games to run (default is 1 if not set).
- `-s <seed_for_random>`: Sets the seed for the random number generator, allowing for reproducible results (default seed is used if not set).

Example usage:
```bash
./100prisoners -n 10 -s 42
```

## Requirements

- C compiler (e.g., GCC) with POSIX thread support.
- Standard libraries: `<pthread.h>`, `<stdlib.h>`, `<time.h>`, `<stdio.h>`.

## Compilation and Execution

1. Compile the code:
   ```bash
   gcc -o 100prisoners 100prisoners.c -lpthread
   ```

2. Run the compiled program with optional command-line parameters:
   ```bash
   ./100prisoners -n <num_of_games> -s <seed_for_random>
   ```

## Output

The program outputs:
- Results indicating whether each prisoner succeeded in finding their assigned number.
- Execution time for each strategy, allowing performance comparison between the different approaches.

## Concurrency and Safety

- **Global Mutex (`lock_global`)**: Ensures thread safety for strategies that use a single shared lock.
- **Drawer-specific Mutexes (`lock_drawer`)**: Each drawer has a unique lock, reducing contention and allowing greater concurrency.

## License

This project is open-source and available under the MIT License. See the LICENSE file for details.
