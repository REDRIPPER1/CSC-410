# CSC 410 A2_3

This folder contains three C programs that use pthreads.

## Requirements

- GCC or another C compiler with pthread support.
- Linux, WSL, or another POSIX system.

## Neuromancer Cyber Heist Game

`neuromancer.c` uses a mutex and a condition variable to protect the game state
and ensure players take turns hacking.

Compile and run:

```bash
gcc neuromancer.c -lpthread -o neuromancer
./neuromancer
```

## Finding the Average

`average.c` uses a barrier to ensure all threads finish calculating their
partial sums before the total and average are calculated.

Compile and run:

```bash
gcc average.c -lpthread -o average
./average
```

## Paradox

`paradox.c` compares two summing approaches. Provide the desired number of
threads as a command-line argument. Run the program with 1, 2, 4, 8, and 12
threads:

```bash
gcc -O2 paradox.c -lpthread -o paradox
./paradox 1
./paradox 2
./paradox 4
./paradox 8
./paradox 12
```
