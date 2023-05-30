#include "mpi.h"
#include <cstdio>
#include <cstdlib>

/* Problem parameters */
double f(double x);
double integral_serial(double xmin, double xmax, double n);
void integral_mpi(double xmin, double xmax, double n, int pid, int np);

int main(int argc, char **argv)
{
  const int numberRects = std::atoi(argv[1]);
  const double lowerLimit = 2.0;
  const double upperLimit = 5.0;

  /* MPI Variables */

  /* problem variables */
  int pid, np;

  /* MPI setup */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  integral_mpi(lowerLimit, upperLimit, numberRects, pid, np);

  /* finish */
  MPI_Finalize();

  return 0;
}

double integral_serial(double xmin, double xmax, double n)
{
  double area = 0.0;
  double width = (xmax - xmin)/n;

  for (int i = 0; i < n; ++i) {
    double at = xmin + i*width + width/2.0; // center in the middle
    double heigth = f(at);
    area = area + width*heigth;
  }
  return area;
}

void integral_mpi(double xmin, double xmax, double n, int pid, int np)
{
  /* Adjust problem size for sub-process */
  double range = (xmax - xmin) / np;
  double lower = xmin + range*pid;

  /* Calculate area for subproblem */
  double area = integral_serial(lower, lower+range, n);

  /* Collect info and print results */
  MPI_Status status;
  int tag = 0;
  if (0 == pid) { /* Master */
    double total = area;
    for (int src = 1; src < np; ++src) {
      MPI_Recv(&area, 1, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, &status);
      total += area;
    }
    fprintf(stderr, "The area from %g to %g is : %25.16e\n", xmin, xmax, total);
  }
  else { /* slaves only send */
    int dest = 0;
    MPI_Send(&area, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
  }
}


double f(double x) {
  return x*x;
}
