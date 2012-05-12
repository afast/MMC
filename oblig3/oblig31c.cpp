#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "mt19937ar.c"

using namespace std;

/* retorna "a - b" en segundos */
double segundos_transcurridos(struct timeval *a, struct timeval *b) {
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) -(double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int puntoPertenece(double x1,double x2,double x3,double x4){
  if((5*x1 + 10*x2 <= 4) && (x3 + x4 <= 1) && (2*x1 - 3*x2 >= 0) && // restricciones extra
      (sqrt(pow(x1-0.3,2) + pow(x2-0.2,2) + pow(x3-0.7,2) + pow(x4-0.8,2)) < 0.5)) { // pertenencia al circulo 
    return 1;
  } else {
    return 0;
  }
}

int main(int argc, char* argv[]) {

  double x1, x2, x3, x4, c1, c2, c3, c4, r, secs; // centro y radio del circulo, y v.a. para coordenadas
  double X, V; // variables auxiliares para estimar el area (volumen 4D) de la hyperesfera restringido por las ecuaciones
  double delta, beta, beta2, w1c, w2c; // Usado en el calculo de intervalo de confianza de Chebyshev
  double k, pprima, qprima, nprima, w1a, w2a; //Usado en el calculo de intervalo de confianza de Agresti-Coull
  int i, n; // auxiliar para bucle y nro. de replicaciones (tamaño de la muestra)
  struct timeval start, end; //Para calcular el tiempo que se demora en realizar los cálculos

  n = 46110994; //La cantidad calculada en la parte b
  init_genrand(time(NULL)); // inicialización del generador de números aleatorios

  // Valores iniciales para Chebyshev
  delta = 0.05; // Intervalo de confianza 95%
  beta = 1/sqrt(0.05); //delta a la -1/2
  beta2 = 1/0.05; // beta al cuadrado
  c1 = 0.3;
  c2 = 0.2;
  c3 = 0.7;
  c4 = 0.8;
  r = 0.5;

  //Agresti-Coull
  k = 1.95996398454005;
  nprima = n + k*k; 

  X = 0;
  V = 0;
  
  gettimeofday(&start, NULL);

  for(int i=0;i<n;i++) {
    double x1 = genrand_real2();
    double x2 = genrand_real2();
    double x3 = genrand_real2();
    double x4 = genrand_real2();

    if(puntoPertenece(x1,x2,x3,x4))
      X += 1;
  }

  //X=X/n;
  //V = X*(1-X)/(n-1);

  // Chebyshev
  w1c= (X + (beta2)/2 - beta*sqrt(beta2/4 + X*(n - X)/n)) / (n + beta2);
  w2c= (X + (beta2)/2 + beta*sqrt(beta2/4 + X*(n - X)/n)) / (n + beta2);

  //Agresti-Coull
  pprima= (X + (k*k)/2) / nprima;
  qprima= 1-pprima;
  w1a= pprima - (k*sqrt(pprima*qprima))/sqrt(nprima);
  w2a= pprima + (k*sqrt(pprima*qprima))/sqrt(nprima);

  // Tiempo de fin
  gettimeofday(&end, NULL);

  cout << "Intervalos de confianza:" << endl;
  cout << "Chebyshev: (" << w1c << ", " << w2c << ") -- Ancho: " << fixed << w2c-w1c << endl;
  cout << "Agresti-Coull: (" << w1a << ", " << w2a << ") -- Ancho: " << fixed << w2a-w1a << endl;
  cout << "Tiempo de calculo: " << segundos_transcurridos(&end, &start) << endl;

  return 0;
}
