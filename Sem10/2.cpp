#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

// Función 1: Llenar aleatoriamente el vector con una distribución uniforme entre [-1.5, 4.5]
void llenar_vector(std::vector<double>& vec, int seed) {
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution(-1.5, 4.5);

    for (auto& elemento : vec) {
        elemento = distribution(generator);
    }
}

// Función 2: Calcular la suma total de los elementos del vector en paralelo y retornar su promedio
double promedio(const std::vector<double>& vec) {
    double suma = 0.0;
    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < vec.size(); ++i) {
        suma += vec[i];
    }

    return suma / vec.size();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Dígite: " << argv[0] << " <semilla>" << std::endl;
        return 1;
    }

    int seed = std::stoi(argv[1]);
    size_t num_elementos = 10000000;
    std::vector<double> vec(num_elementos);

    // Llenar el vector con valores aleatorios
    llenar_vector(vec, seed);

    // Calcular el promedio en paralelo
    double prom = promedio(vec);

    std::cout << "El promedio del vector de " << num_elementos << " con la semilla " << seed << " es: "<< prom << std::endl;

    return 0;
}
