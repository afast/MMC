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

void estimarIntegral(int rep, double &estimador, double &sigma2) {
  estimador = 0;
  double T=0, x1, x2, x3, x4, x5, phix;
  for (int i=1; i <= rep; i++) {
    x1= genrand_real1(); // sorteo de coordenada x_1
    x2= genrand_real1(); // sorteo de coordenada x_2
    x3= genrand_real1(); // sorteo de coordenada x_3
    x4= genrand_real1(); // sorteo de coordenada x_4
    x5= genrand_real1(); // sorteo de coordenada x_5

    phix = x1 * x2*x2 * pow(x3, 3) * pow(x4, 4) * pow(x5, 5);
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

  /* --------------------- parte a --------------------- */
  init_genrand(time(NULL)); // inicialización del generador de número aleatorios
  S = V = 0;
  n = 1000000;

  gettimeofday(&start, NULL);
  estimarIntegral(n, S, sigma2);
  V = sigma2/n;

  gettimeofday(&end, NULL);

  cout << "Estimacion: " << S << endl << "Desviacion Estandar: " << fixed << sqrt(V) << endl;
  cout << "Tiempo de calculo: " << tiempo_transcurrido(&end, &start) << " segundos." << endl;

  /* --------------------- parte b --------------------- */
  double delta, epsilon2, z2;
  delta = 0.05;
  epsilon2 = 0.0005*0.0005;
  z2 = pow(1.95996398454005, 2); // (phi^(-1)(1-delta/2))^2
  int Nnuevo = (int) ceil(z2*sigma2/epsilon2);

  cout << "Numero de replicaciones para (epsilon, delta) = (0.0005, 0.05): " << Nnuevo << endl;
  
  /* --------------------- parte c --------------------- */
  /* *
   * Intervalos de confianza (z = invnorm(1 - delta/2))
   *
   * 1.- Nivel 0.90, delta = 0.10, z = 1.64485362695147
   * 2.- Nivel 0.95, delta = 0.05, z = 1.95996398454005
   * 3.- Nivel 0.99, delta = 0.01, z = 2.5758293035489
   *
   * */

  double w1, w2, z[3], valorExacto;
  int cubrimiento[3] = {0,0,0};
  valorExacto = (double) 1/720;

  z[0]= 1.64485362695147;
  z[1]= 1.95996398454005;
  z[2]= 2.5758293035489;

  for (int L=0; L<500; L++) {
    init_genrand(genrand_int32());
    estimarIntegral(Nnuevo, S, sigma2);
    for (int i=0; i<3; i++) {
      w1 = S - z[i]*sqrt(sigma2/Nnuevo);
      w2 = S + z[i]*sqrt(sigma2/Nnuevo); 
      if (w1 <= valorExacto && valorExacto <= w2)
        cubrimiento[i]++;
    }
  }

  cout << "Valor Exacto: " << valorExacto << endl;
  cout.precision(2);
  cout.fixed;
  cout << "Cobertura Empírica: " << endl;
  cout << "0.90 - " << ((double)cubrimiento[0]/500)*100 << "%" << endl;
  cout << "0.95 - " << ((double)cubrimiento[1]/500)*100 << "%" << endl;
  cout << "0.99 - " << ((double)cubrimiento[2]/500)*100 << "%" << endl;


  return 0;
}
