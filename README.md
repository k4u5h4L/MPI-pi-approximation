# MPI programs

MPI programs to calculate/approximate Pi using Monte Carlo simulation.

## Usage:

-   Install MPI.

-   Compile your C program into an object file.

```
mpicc <filename>.c -o <object_file> -lm
```

-   Execute the program.

```
mpirun -np <number of processes> /path/to/object_file
```
