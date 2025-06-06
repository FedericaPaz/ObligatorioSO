#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int array1[5] = {1, 2, 3, 4, 5};
int array2[5] = {1, 2, 3, 4, 5};
int resultados[5];
int suma = 0;
int mult = 1;

void* hilo_suma(void* x){
    int pos = *((int*)x);
    resultados[pos] = array1[pos] + array2[pos];
    printf("Hilo %d : Suma %d + %d = %d \n", pos+1, array1[pos], array2[pos], resultados[pos]);
    return NULL;
}

void* hilo_mult(void* x){
    suma = resultados[0]+resultados[1]+resultados[2];
    printf("Hilo 4 : Suma total = %d \n", suma);
    mult = suma * array1[3] * array2[3];
    printf("Hilo 4 : Multiplicación %d * %d * %d = %d \n", suma, array1[3], array2[3], mult);
    return NULL;
}

void* hilo_final(void* x){
    int resultado_final = mult + array1[4] + array2[4];
    printf("Hilo 5 : %d + %d + %d = %d \n", mult, array1[4], array2[4], resultado_final);
    printf("Resultado final: %d \n", resultado_final);
    return NULL;
}

int main (){
    pthread_t hilos[5];
    int pos[3] = {0,1,2};
    
    for(int i = 0; i<3; i++){
        pthread_create(&hilos[i], NULL, hilo_suma, &pos[i]);
    }

    for(int i = 0; i<3; i++){
        pthread_join(hilos[i], NULL);
    }

    pthread_create(&hilos[3], NULL, hilo_mult, NULL);
    pthread_join(hilos[3], NULL);

    pthread_create(&hilos[4], NULL, hilo_final, NULL);
    pthread_join(hilos[4], NULL);

    return 0;
}