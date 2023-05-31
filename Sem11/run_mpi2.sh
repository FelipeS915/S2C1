#!/bin/bash

A=1200000000/10  # Valor de N que deseas utilizar

# Ejecutar para diferentes números de procesos
for N in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 50000000 100000000 500000000 1000000000 1200000000; do
    echo "Ejecutando con $np procesos:"
    mpirun -np 4 ./pi2 $N
    echo "-----------------------------"
done
