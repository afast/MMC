#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "RAND.c"

using namespace std;

double tiempo_transcurrido(struct timeval *a, struct timeval *b) {
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

void estimarIntegral(int rep, double &estimador, double &sigma2) {
  estimador = 0;
  double T=0, x1, x2, x3, x4, x5, x6, x7, x8, x9, phix;
  for (int i=1; i <= rep; i++) {
    x1= getRandNumber(); // sorteo de coordenada x_1
    x2= getRandNumber(); // sorteo de coordenada x_2
    x3= getRandNumber(); // sorteo de coordenada x_3
    x4= getRandNumber(); // sorteo de coordenada x_4
    x5= getRandNumber(); // sorteo de coordenada x_5
    x6= getRandNumber(); // sorteo de coordenada x_6
    x7= getRandNumber(); // sorteo de coordenada x_7
    x8= getRandNumber(); // sorteo de coordenada x_8
    x9= getRandNumber(); // sorteo de coordenada x_9

    phix = sqrt(x1*x1 + x2*x2 + x3*x3 + x4*x4 + x5*x5 + x6*x6 + x7*x7 + x8*x8 + x9*x9);
    estimador += phix;

    if (i > 1)
      T = T + (1 - 1/i) * pow(phix - estimador/(i-1), 2);
  }
  estimador = estimador/rep; // normalización de la estimación
  sigma2 = T/(rep-1);

  return;
}
int main(int argc, char* argv[]) {
  double S, V, sigma2;
  int i, n;
  struct timeval start, end;

  if (argc != 2)
    cout << "Uso: ./ej42 <archivo>" << endl;

  /* --------------------- parte a --------------------- */
  init_rand(argv[1]);

  S = V = 0;
  n = 100000;

  gettimeofday(&start, NULL);
  estimarIntegral(n, S, sigma2);
  V = sigma2/n;

  gettimeofday(&end, NULL);

  cout << "Estimacion: " << S << endl << "Desviacion Estandar: " << fixed << sqrt(V) << endl;
  cout << "Tiempo de calculo: " << tiempo_transcurrido(&end, &start) << " segundos." << endl;

  double phiInverse = 2.5758293035489;
  
  double inicio = S - phiInverse * pow(sigma2/n,0.5);
  double fin = S + phiInverse * pow(sigma2/n,0.5);

  cout << "Inicio intervalo: " << inicio << endl;
  cout << "Fin intervalo: " << fin << endl;
  return 0;
}
