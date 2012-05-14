#include "mt19937ar.c"
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <cmath>

using namespace std;

double segundos_transcurridos(struct timeval *a, struct timeval *b)
{
  return
    (double)(a->tv_sec + (double)a->tv_usec/1000000) -
    (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

double phi(double x1, double x2){

    if (sqrt(pow(x1-0.5,2) + pow(x2-0.5,2)) < 0.5) {
        return sin(x1*x2)/pow(x1*x2,2);
    }else{
        return 0;
    }

}

int main()
{
    init_genrand(time(NULL));

    int n;

    cout << "Ingrese la cantidad de iteraciones\n";
    cin >> n;
    cout << "La cantidad de iteraciones es " << n << "\n";

    struct timeval start, end;

    gettimeofday(&start, NULL);;
    
    double S = 0;
    double T = 0;

    for(int i=1;i<=n;i++){
        double x1 = genrand_real2();
        double x2 = genrand_real2();

        double p = phi(x1,x2);
        S += p;

        if(i>1){
            T += (1-1/i)*pow(p-S/(i-1),2);
        }
    }

    double C=S/n;
    double Vphi = T/(n-1);
    double VC = Vphi/n;

    gettimeofday(&end, NULL);

    cout << "Valor estimador = " << C << "\n";
    cout << "Varianza phi = " << Vphi << "\n";
    cout << "Varianza estimador = " << VC << "\n";

    double phiInverse = 1.959963985;

    double inicio = C - phiInverse * pow(Vphi/n,0.5);
    double fin = C + phiInverse * pow(Vphi/n,0.5);

    cout << "Inicio intervalo = " << inicio << "\n";
    cout << "Fin intervalo = " << fin << "\n";

    double secs = segundos_transcurridos(&end, &start); //Calcular segundos

    //Calculo del tiempo insumido
    cout << "Tiempo= " << secs << " segundos\n";

    return 0;
}
