#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <cstdio>

int main() {
    size_t vector_size = 200000000;
    int max_threads = std::thread::hardware_concurrency();
    std::ofstream results_file("results.txt");

    results_file << "threads,policy,execution_time,speedup,parallel_efficiency\n";

    for (int policy = 0; policy < 3; ++policy) {
        for (int threads = 1; threads <= max_threads; ++threads) {
            std::string command = "./prueba " + std::to_string(vector_size) + " " + std::to_string(threads) + " " + std::to_string(policy);
            std::cout << "Ejecutando: " << command << std::endl;

            // Ejecutar el programa y leer la salida
            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "Error al abrir el pipe" << std::endl;
                return 1;
            }

            // Leer la salida del comando y escribirla en el archivo
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                results_file << buffer;
            }

            pclose(pipe);
        }
    }

    results_file.close();
    return 0;
}
