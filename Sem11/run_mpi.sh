#!/bin/bash

N=1200000000  # Valor de N que deseas utilizar

# Ejecutar para diferentes números de procesos
for np in 1 2 3 4 5 6 7 8; do
    echo "Ejecutando con $np procesos:"
    mpirun -np $np ./mpi_pi_approximation2 $N
    echo "-----------------------------"
done
