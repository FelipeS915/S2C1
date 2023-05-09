#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <numeric>
#include <execution>
#include <thread>
#include <tbb/global_control.h>

// Para compilar:
// g++ -std=c++17 parallel_metrics_algs.cpp -ltbb -o ejecutable
//
// Para ejecutar:
// ./ejecutable 200000000 "#num-threads" "politica ejecucion 0-seq 1-par 2-par_unseq"
//
// ej
// ./ejecutable 200000000 4 1
//
// Estructura de salida:
// #threads,politica_ejecucion,tiempo_ejecucion,speedup,parallel_efficiency \n
//
int main(int argc, char *argv[]) {

    size_t vector_size = std::stoul(argv[1]);
    int num_threads = std::stoi(argv[2]);
    int politica_ejecucion = std::stoi(argv[3]);
    int num_simulaciones = 5;
    auto rule = " ";
    double tiempo_par = 0.0;
    // Generar un vector de números enteros desde 0 hasta vector_size-1
    std::vector<int> v(vector_size);
    std::iota(v.begin(), v.end(), 0);

    double total_speedup = 0.0;
    double total_eficiencia = 0.0;

    for (int i = 0; i < num_simulaciones; ++i) {
        // Medir el tiempo de ejecución en modo secuencial
        auto start = std::chrono::high_resolution_clock::now();
        auto sum_seq = std::accumulate(v.begin(), v.end(), 0.0);
        auto end = std::chrono::high_resolution_clock::now();
        double tiempo_seq = std::chrono::duration<double>(end - start).count();

        // Controlar el numero de threads usados en la ejecucion en paralelo
        tbb::global_control control(tbb::global_control::max_allowed_parallelism, num_threads);

        // Medir el tiempo de ejecución en modo paralelo
        start = std::chrono::high_resolution_clock::now();
        int sum_par;
        switch (politica_ejecucion) {
            case 0:
                sum_par = std::reduce(std::execution::seq, v.begin(), v.end());
                rule = "seq";
                break;
            case 1:
                sum_par = std::reduce(std::execution::par, v.begin(), v.end());
                rule = "par";
                break;
            case 2:
                sum_par = std::reduce(std::execution::par_unseq, v.begin(), v.end());
                rule = "par_unseq";
                break;
            default:
                std::cerr << "Opción de política de ejecución no válida" << std::endl;
                return 1;
        }
        end = std::chrono::high_resolution_clock::now();
        tiempo_par = std::chrono::duration<double>(end - start).count();

        // Calcular el speedup y la eficiencia de la versión paralela
        double speedup = tiempo_seq / tiempo_par;
        double eficiencia = speedup / num_threads;

        total_speedup += speedup;
        total_eficiencia += eficiencia;
    }

    // Calcular los promedios de speedup y eficiencia
    double promedio_speedup = total_speedup / num_simulaciones;
    double promedio_eficiencia = total_eficiencia / num_simulaciones;

    // Imprimir los resultados
    std::cout << num_threads << "," << rule << "," << tiempo_par << "," << promedio_speedup << "," << promedio_eficiencia << std::endl;
    return 0;
}
