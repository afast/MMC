#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include "mt19937ar.c"

using namespace std;

bool existe_camino_rec(int s, int t, char** aristas, int cantNodos, char* visitados){

    if(s==t){
        return true;
    }else{
        for(int i = 0; i<cantNodos;i++){
            if((aristas[s][i])&&(!visitados[i])){ // Si esta conectado al nodo actual y no fue visitado aun
                visitados[i]=1;
                if(existe_camino_rec(i,t,aristas,cantNodos,visitados)){
                    return true;
                }
            }
        }
        return false;
    }

}

bool existe_camino(int s, int t, char** aristas, int cantNodos){

    char visitados[cantNodos];
    for(int i=0;i<cantNodos;i++){ //setear todos los nodos como no visitados
        visitados[i]=0;
    }

    visitados[s]=1;
    return existe_camino_rec(s,t,aristas,cantNodos,visitados);

}

double tiempo_transcurrido(struct timeval *a, struct timeval *b) {
  return (double)(a->tv_sec + (double)a->tv_usec/1000000) - (double)(b->tv_sec + (double)b->tv_usec/1000000);
}

int sorteo(){
    if(genrand_real1()>=0.5){
        return 1;
    }else{
        return 0;
    }
}

void sortear_aristas(char** aristas,int cantNodos,char** aristasResultantes,int &cantAristasResultantes){

    cantAristasResultantes = 0;

    for(int i = 0; i<cantNodos; i++){
        for(int j=0;j<cantNodos;j++){
            if((aristas[i][j])&&(sorteo()==1)){
                aristasResultantes[i][j]=1;
                cantAristasResultantes++;
            }else{
                aristasResultantes[i][j]=0;
            }

        }
    }
}

void calcular_ci(int cantReplicaciones, int cantNodos, int cantAristas, char** aristas, int s, int t, int i, double sigma, double &C, double &V, double &iniIntervalo, double &finIntervalo){

    char** aristasResultantes;
    int cantAristasResultantes;

    aristasResultantes = new char*[cantNodos];
    for(int k=0; k < cantNodos ; k++){
        aristasResultantes[k]=new char[cantNodos];
        for(int j=0;j<cantNodos;j++){
            aristasResultantes[k][j]=0;
        }
    }

    double r = pow(2,cantAristas);
    int S = 0;

    for(int j = 0; j<cantReplicaciones;j++){
        sortear_aristas(aristas,cantNodos, aristasResultantes, cantAristasResultantes);
        if((cantAristasResultantes==i)&&(existe_camino(s,t,aristasResultantes,cantNodos))){
            S++;
        }
    }

    C = r*S/cantReplicaciones;
    V = C*(r-C)/(cantReplicaciones-1);

    //faltan iniIntervalo, finIntervalo
}

void imprimir_estimaciones(int n,double sigma, int cantNodos, int cantAristas, char** aristas, int s, int t){
    struct timeval start, end;
    init_genrand(time(NULL)); // inicialización del generador de número aleatorios

    double C,V,iniIntervalo, finIntervalo;

    for(int i=1;i<=cantAristas;i++){

        gettimeofday(&start, NULL);

        calcular_ci(n, cantNodos, cantAristas, aristas, s, t, i, sigma, C, V, iniIntervalo, finIntervalo);

        gettimeofday(&end, NULL);
        cout << "i: " << i << endl;
        cout << "Estimacion: " << fixed << C << endl;
        cout << "Varianza: " << fixed << V << endl;
        cout << "Desviacion Estandar: " << fixed << sqrt(V) << endl;
        cout << "Inicio intervalo de confianza: " << fixed << iniIntervalo << endl;
        cout << "Fin intervalo de confianza: " << fixed << finIntervalo << endl;

        cout << "Tiempo de calculo: " << tiempo_transcurrido(&end, &start) << " segundos." << endl << endl;

    }
}

int main(int argc, char* argv[]) {
    int n = 1000;
    double sigma = 0.95; // confianza requerida

    // inicializacion de datos del problema
    int cantNodos = 8;
    char** aristas = new char*[cantNodos];
    for(int i = 0; i<cantNodos; i++){
        aristas[i]=new char[cantNodos];
        for(int j=0;j<cantNodos;j++){
            aristas[i][j]=0;
        }
    }

    //seteo aristas
    aristas[0][1]=aristas[0][2]=aristas[1][2]=aristas[1][3]=aristas[2][4]=aristas[3][4]=aristas[3][5]=aristas[4][6]=aristas[5][6]=aristas[5][7]=aristas[6][7]=1;
    int cantAristas = 11;
    int s,t; // nodos fuente y terminal
    s=0;
    t=7;

    imprimir_estimaciones(n,sigma, cantNodos, cantAristas, aristas, s, t);

    return 0;
}
