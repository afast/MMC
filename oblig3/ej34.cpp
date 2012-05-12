#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "mt19937ar.c"

using namespace std;

double tiempo_transcurrido(struct timeval *a, struct timeval *b) {
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int main(int argc, char* argv[]) {
  double S, V, sigma2;
  double T=0, phix, x1, x2, x3, x4, x5;
  int i, n;
  struct timeval start, end;

  /* --------------------- parte a --------------------- */
  init_genrand(time(NULL)); // inicialización del generador de número aleatorios
  S = V = 0;
  n = 1000000;

  gettimeofday(&start, NULL);
  for (int i=1; i <= n; i++) {
    x1= genrand_real1(); // sorteo de coordenada x_1
    x2= genrand_real1(); // sorteo de coordenada x_2
    x3= genrand_real1(); // sorteo de coordenada x_3
    x4= genrand_real1(); // sorteo de coordenada x_4
    x5= genrand_real1(); // sorteo de coordenada x_5
    phix = x1 * x2*x2 * pow(x3, 3) * pow(x4, 4) * pow(x5, 5);
    S += phix;

    if (i > 1)
      T = T + (1 - 1/i) * pow(phix - S/(i-1), 2);
  }
  S = S/n; // normalización de la estimación
  sigma2 = T/(n-1);
  V = sigma2/n;

  gettimeofday(&end, NULL);

  cout << "Estimacion: " << S << endl << "Desviacion Estandar: " << fixed << sqrt(V) << endl;
  cout << "Tiempo de calculo: " << tiempo_transcurrido(&end, &start) << " segundos." << endl;

  /* --------------------- parte b --------------------- */
  double delta, e;
  delta = 0.05;
  epsilon2 = 0.0005*0.0005;
  z2 = pow(1.95996398454005, 2); // (phi^(-1)(1-delta/2))^2
  int Nnuevo = (int) ceil(z2*sigma2/epsilon2);

  cout << "Numero de replicaciones para (epsilon, delta) = (0.0005, 0.05): " << Nnuevo << endl;

  return 0;
}
