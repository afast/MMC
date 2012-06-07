/*

    Libreria de numeros aleatorios usando shuffling.
    Generadores congruentes lineales usados:
        - rand() implementacion gcc
        - ??

*/
#include <stdlib.h>
#include <time.h>
#include <cmath>
#define K 10000
#define M1 RAND_MAX
#define M2 RAND_MAX

using namespace std;

int i = 0;
int numbers[K];

int rand_2(){
    return rand();
} 

void init_rand(){
    srand ( time(NULL) );

    for(int j=0;j<K;j++){
        numbers[j]=rand();
    }
}

double getRandNumber(){
	
    int Yi = rand_2();
    int j = ceil(K*Yi/M2);
    int Zi = numbers[j];
    numbers[j]=rand();
	return ((double) Zi)/((double)M1);
}

