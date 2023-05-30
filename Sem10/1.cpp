# include <iostream>
# include <omp.h>
int main(int argc, char **argv)
{
        int id = omp_get_thread_num();
        int nth = omp_get_num_threads();
        std::cout << "id :" << id << "\n";
        std::cout << "nth :" << nth << "\n";
        std::cout << "&id :" << &id << "\n";
        std::cout << "&nth:" << &nth << "\n";
        # pragma omp parallel
        {
                int id = omp_get_thread_num();
                int nth = omp_get_num_threads();
                std::cout << "id :" << id << "\n";
                std::cout << "nth :" << nth << "\n";
                std::cout << "&id :" << &id << "\n";
                std::cout << "&nth:" << &nth << "\n";
        }
        return 0;
}


//La salida del programa demuestra cómo OpenMP crea threads y cómo los threads pueden acceder a variables compartidas y privadas.
