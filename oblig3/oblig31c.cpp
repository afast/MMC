#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "mt19937ar.c"

/* retorna "a - b" en segundos */
double timeval_diff(struct timeval *a, struct timeval *b) {
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) -(double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int main(int argc, char* argv[]) {
  double x1, x2, x3, x4, c1, c2, c3, c4, r, secs; // centro y radio del
  circulo, y v.a. para coordenadas
  double estimE, estimDesv; // variables auxiliares para estimar el area (volumen 4D) de la hyperesfera restringido por las ecuaciones
  double delta, beta, beta2, w1, w2; //Para Chebyshev
  double k, pprima, qprima, nprima; //Para Agresti-Coull
  int i, n; // auxiliar para bucle y nro. de replicaciones (tamaño de la muestra)
  struct timeval start, end; //Para calcular el tiempo que se demora en realizar los cálculos
  //Chebyshev
  delta= 0.05; //Confianza 95%
  beta= 1/sqrt(0.05); //delta a la -1/2
  beta2= 1/0.05;
  c1=0.3;
  c2=0.2;
  c3=0.7;
  c4=0.8;
  r=0.5;
