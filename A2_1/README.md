# CSC 410 A2_1
This folder contains three C programs for the process-management assignments.

## Files

- `starwars.c` - Coding Task 1: creates four child processes representing Luke,
  Han, Chewbacca, and Leia. Each child adjusts its own copy of the shield power
  and the parent waits for all children.
- `seq_sum.c` - Sequential array-sum reference program.
- `process_sums.c` - Coding Task 3: divides an array among child processes and
  uses pipes to send partial sums to the parent.

## Requirements

- GCC or another C11-compatible compiler.
- Linux, WSL, or another POSIX system supporting `fork()`, `pipe()`, and
  `waitpid()`.

## Coding Task 1: Star Wars Processes

Compile and run:

```bash
gcc starwars.c -o starwars
./starwars
```

The starting shield power is `50%`. The children use their own copies of the
variable and report these levels:

- Luke: `75%` (`+25%`)
- Han: `70%` (`+20%`)
- Chewbacca: `80%` (`+30%`)
- Leia: `65%` (`+15%`)

The parent waits for every child, but its own shield power remains `50%`
because normal child processes do not share writable memory with the parent.

## Coding Task 3: Parallel Array Sum

Compile the sequential reference program:

```bash
gcc seq_sum.c -o seq_sum
```

Compile the parallel program for 1 through 4 child processes:

```bash
gcc -Dprocess=1 process_sums.c -o process_sums_1
gcc -Dprocess=2 process_sums.c -o process_sums_2
gcc -Dprocess=3 process_sums.c -o process_sums_3
gcc  -Dprocess=4 process_sums.c -o process_sums_4
```

Run the programs with the array size as the argument:

```bash
./seq_sum 100000000
./process_sums_1 100000000
./process_sums_2 100000000
./process_sums_3 100000000
./process_sums_4 100000000
```

Each program should print:

```text
Total sum = 5000000050000000
```

The `-Dprocess=N` option selects the number of child processes at compile
time. The parallel program gives each child one chunk, collects the partial
sums through pipes, waits for all children, and closes unused file descriptors
in both parent and child processes.
