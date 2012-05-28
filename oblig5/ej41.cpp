#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <cmath>
#include "Generador.cpp"

double segundos_transcurridos(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int puntoPertenece(double x1,double x2,double x3,double x4){
    if((5*x1 + 10*x2 <= 4) && (x3 + x4 <= 1) && (2*x1 - 3*x2 >= 0) && // restricciones extra
        (sqrt(pow(x1-0.3,2) + pow(x2-0.2,2) + pow(x3-0.7,2) + pow(x4-0.8,2)) < 0.5)){ // pertenencia al circulo 
        return 1;
    }else{
        return 0;
    }
}

int main(int argc, char* argv[]) {

  if (argc != 4) {
    cout << "Invocar con <archivo> <numero de iteraciones> <ignorar primer elem {0,1}>" << endl;
    return 0;
  }
  Generador generador;

  char * nombreArchivo = argv[1];
  istringstream args(argv[2]);
  int n, ignorar;
  struct timeval start, end;

  args >> n; // Configurar iteraciones
  args.str(argv[3]);
  args.clear();
  args.seekg(0);
  args >> ignorar; // Configurar el ignorar primer numero de la linea o no
  
  cout << "Archivo usado: " << nombreArchivo << endl;
  cout << "Iteraciones: " << n << endl;

  generador.iniciar(nombreArchivo, ignorar==1);
  generador.normalizar();

  gettimeofday(&start, NULL);;
  
  double X = 0;
  double V = 0;

  for(int i=0;i<n;i++){
      double x1 = generador.getNextRand();
      double x2 = generador.getNextRand();
      double x3 = generador.getNextRand();
      double x4 = generador.getNextRand();
      
      if(puntoPertenece(x1,x2,x3,x4))
          X += 1;

  }

  generador.cerrar();
  X=X/n;

  V = X*(1-X)/(n-1);

  gettimeofday(&end, NULL);

  cout << "Volumen = " << X << "\n";
  cout << "Varianza = " << V << "\n";
  cout << "Desviación Estándar = " << sqrt(V) << "\n";

  double secs = segundos_transcurridos(&end, &start); //Calcular segundos

  //Calculo del tiempo insumido
  cout << "Tiempo= " << secs << " segundos\n";

  return 0;
}
