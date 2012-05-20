#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "mt19937ar.c"

using namespace std;

bool existe_camino_rec(int s, int t, int** aristas, int cantNodos, int *visitados){

  if (s==t) {
    return true;
  } else {
    for (int i = 0; i<cantNodos; i++) {
      if ((aristas[s][i]) && (!visitados[i])) { // Si esta conectado al nodo actual y no fue visitado aun
        visitados[i]=1;
        if (existe_camino_rec(i,t,aristas,cantNodos,visitados))
          return true;
      }
    }
    return false;
  }
}

bool existe_camino(int s, int t, int** aristas, int cantNodos) {

  int *visitados = new int[cantNodos];
  for(int i=0;i<cantNodos;i++) //setear todos los nodos como no visitados
    visitados[i]=0;

  visitados[s]=1;
  bool result = existe_camino_rec(s,t,aristas,cantNodos,visitados);
  delete visitados;
  return result;
}

double tiempo_transcurrido(struct timeval *a, struct timeval *b) {
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int sorteo(){
  if(genrand_real1()>=0.5)
    return 1;
  else
    return 0;
}

void sortear_aristas(int** aristas, int cantNodos, int** aristasResultantes, int &cantAristasResultantes) {

  cantAristasResultantes = 0;

  for (int i=0; i<cantNodos; i++) {
    for (int j=0; j<cantNodos; j++) {
      if ((aristas[i][j]) && (sorteo()==1)) {
        aristasResultantes[i][j] = 1;
        cantAristasResultantes++;
      } else
        aristasResultantes[i][j] = 0;
    }
  }
}

void calcular_ci(int cantReplicaciones, int cantNodos, int cantAristas, int** aristas, int s, int t, double sigma, double *C, double *V, double *iniIntervalo, double *finIntervalo){

  int **aristasResultantes;
  int cantAristasResultantes;
  int *S = new int[cantAristas+1];
  double ac, pprima, qprima, nprima, w1a, w2a; //Usado en el calculo de intervalo de confianza de Agresti-Coull

  //Agresti-Coull
  ac = 1.95996398454005;
  nprima = cantReplicaciones + ac*ac;

  aristasResultantes = new int*[cantNodos];
  for (int k=0; k<cantNodos; k++) {
    aristasResultantes[k] = new int[cantNodos];
    for (int j=0; j<cantNodos; j++)
      aristasResultantes[k][j]=0;
  }

  double r = pow(2,cantAristas);
  for (int k=0; k<=cantAristas; k++)
    S[k] = 0;

  for (int j = 0; j<cantReplicaciones; j++) {
    sortear_aristas(aristas,cantNodos, aristasResultantes, cantAristasResultantes);
    if (existe_camino(s,t,aristasResultantes,cantNodos))
      S[cantAristasResultantes]++;
  }

  for (int k=0; k<=cantAristas; k++) {
    cout << "S" << k << ": " << S[k] << endl;
    C[k] = (double)(r*S[k]/cantReplicaciones);
    V[k] = (double)(C[k]*(r-C[k])/(cantReplicaciones-1));

    //Agresti-Coull
    pprima= (S[k] + (ac*ac)/2) / nprima;
    qprima= 1-pprima;
    iniIntervalo[k] = r*(pprima - (ac*sqrt(pprima*qprima))/sqrt(nprima));
    finIntervalo[k] = r*(pprima + (ac*sqrt(pprima*qprima))/sqrt(nprima));
  }
  delete S;
  for (int k=0; k<cantNodos; k++)
    delete aristasResultantes[k];
  delete aristasResultantes;
}

void imprimir_estimaciones(int n,double sigma, int cantNodos, int cantAristas, int** aristas, int s, int t){
  struct timeval start, end;
  init_genrand(time(NULL)); // inicialización del generador de número aleatorios

  double *C, *V, *iniIntervalo, *finIntervalo;
  C = new double[cantAristas+1];
  V = new double[cantAristas+1];
  iniIntervalo = new double[cantAristas+1];
  finIntervalo = new double[cantAristas+1];

  gettimeofday(&start, NULL);

  calcular_ci(n, cantNodos, cantAristas, aristas, s, t, sigma, C, V, iniIntervalo, finIntervalo);

  gettimeofday(&end, NULL);
  for (int i=0; i<=cantAristas; i++){
    cout << "C" << i << endl;
    cout << "Estimacion: " << fixed << C[i] << endl;
    cout << "Varianza: " << fixed << V[i] << endl;
    cout << "Desviacion Estandar: " << fixed << sqrt(V[i]) << endl;
    cout << "Intervalo de Confianza(Agresti-Coull) 95% - (" << fixed << iniIntervalo[i] << ", " << finIntervalo[i] << ")" << endl;
  }

  cout << "-------------------------------------" << endl;
  cout << "Tiempo de calculo: " << tiempo_transcurrido(&end, &start) << " segundos." << endl << endl;
  delete C;
  delete V;
  delete iniIntervalo;
  delete finIntervalo;
}

int main(int argc, char* argv[]) {
  int n = 1000;
  double sigma = 0.95; // confianza requerida

  // inicializacion de datos del problema
  int cantNodos = 8;
  int** aristas = new int*[cantNodos];
  for(int i = 0; i<cantNodos; i++){
    aristas[i] = new int[cantNodos];
    for(int j=0; j<cantNodos; j++)
      aristas[i][j] = 0;
  }

  //seteo aristas
  aristas[0][1] = aristas[0][2] = aristas[1][2] = aristas[1][3] = aristas[2][4] = aristas[3][4] = aristas[3][5] = aristas[4][6] = aristas[5][6] = aristas[5][7] = aristas[6][7] = 1;
  int cantAristas = 11;
  int s,t; // nodos fuente y terminal
  s=0;
  t=7;

  imprimir_estimaciones(n,sigma, cantNodos, cantAristas, aristas, s, t);

  for(int i = 0; i<cantNodos; i++)
    delete aristas[i];
  delete aristas;
  return 0;
}
