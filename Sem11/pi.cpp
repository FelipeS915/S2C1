#include <iostream>
#include <cmath>
#include <random>
#include <mpi.h>
#include <chrono>

double calculate_pi(long long N, int pid, int np) {

    // Generación de números aleatorios
    std::mt19937 gen(pid);
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    long long local_count = 0;
    for (long long i = 0; i < N; ++i) {
        double x = dis(gen);
        double y = dis(gen);
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    long long global_count = 0;
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double pi = 4.0 * static_cast<double>(global_count) / static_cast<double>(N * np);

    return pi;
}

int main(int argc, char** argv) {
    int np, pid;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    long long N = 0;

    if (argc > 1) {
        N = std::atoll(argv[1]);
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();

    double pi = calculate_pi(N, pid, np);

    std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    double total_elapsed_time;
    MPI_Reduce(&elapsed_time, &total_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (pid == 0) {
        double serial_elapsed_time = calculate_pi(N, 0, 1); // Calcula el tiempo serial en el proceso 0
        double speedup = serial_elapsed_time / total_elapsed_time;
        double efficiency = speedup / np;
        double relative_error = std::fabs(pi - M_PI) / M_PI;

        std::cout << np << "," << speedup << "," << efficiency << "," << relative_error << std::endl;
    }

    MPI_Finalize();

    return 0;
}
