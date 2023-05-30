#include <iostream>
#include <cmath>
#include <random>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0)
            std::cout << "Error: debe proporcionar el número de muestras (N) como argumento." << std::endl;
        MPI_Finalize();
        return 1;
    }

    int N = std::stoi(argv[1]);

    // Semilla aleatoria basada en el rango del proceso
    std::mt19937 rng(rank);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    int local_count = 0;
    for (int i = 0; i < N; ++i) {
        double x = dist(rng);
        double y = dist(rng);
        if (x * x + y * y < 1.0)
            ++local_count;
    }

    if (rank != 0) {
        // Enviar el conteo local al proceso 0
        MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int global_count = local_count;

        // Recibir los conteos locales de los otros procesos y sumarlos
        for (int i = 1; i < size; ++i) {
            int received_count;
            MPI_Recv(&received_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_count += received_count;
        }

        double pi = 4.0 * static_cast<double>(global_count) / (N * size);
        double pi_diff = std::abs(pi - M_PI) / M_PI;

        std::cout.precision(15);
        std::cout << "N = " << N << ", Estimación de pi = " << std::scientific << pi;
        std::cout << ", Diferencia relativa = " << std::scientific << pi_diff << std::endl;
    }

    MPI_Finalize();
    return 0;
}
