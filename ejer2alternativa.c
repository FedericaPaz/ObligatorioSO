#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//semáforos por materia con dependencia
sem_t TD, M2, FN, MF, MIN, RR, PR, SR, INU, CB, SN;

void* IN(void* x) {
  printf("IN: Introducción a la Ingeniería Nuclear\n");
  return NULL;
}

void* M1(void* x) {
  printf("M1: Matemáticas 1\n");
  sem_post(&M2); //permite M2
  return NULL;
}

void* F1(void* x) {
  printf("F1: Física 1\n");
  sem_post(&TD); //para TD
  sem_post(&FN); //para FN
  return NULL;
}

void* QG(void* x) {
  printf("QG: Química General\n");
  sem_post(&TD); //para TD
  sem_post(&MIN); //para MIN
  return NULL;
}

void* func_TD(void* x) {
  sem_wait(&TD); //espera a que F1 termine
  sem_wait(&TD); //espera a que QG termine
  printf("TD: Termodinámica\n");
  sem_post(&MF); //para MF
  return NULL;
}

void* func_M2(void* x) {
  sem_wait(&M2); //espera a que M1 termine
  printf("M2: Matemáticas 2\n");
  sem_post(&FN); //para FN
  sem_post(&MF); //para MF
  sem_post(&RR); //para RR
  return NULL;
}

void* func_FN(void* x) {
  sem_wait(&FN); //espera a que F1 termine
  sem_wait(&FN); //espera a que M2 termine
  printf("FN: Física Nuclear\n");
  sem_post(&MIN); //para MIN
  sem_post(&RR); //para RR
  sem_post(&PR);  //para PR
  sem_post(&INU); //para INU
  return NULL;
}

void* func_MF(void* x) {
  sem_wait(&MF); //espera a que TD termine
  sem_wait(&MF); //espera a que M2 termine
  printf("MF: Mecánica de Fluidos\n");
  sem_post(&SR); //para SR
  return NULL;
}

void* func_MIN(void* x) {
  sem_wait(&MIN); //espera a que FN termine
  sem_wait(&MIN); //espera a que QG termine
  printf("MIN: Materiales en Ing. Nuclear\n");
  sem_post(&PR); //para PR
  sem_post(&CB); //para CB
  return NULL;
}

void* func_RR(void* x) {
  sem_wait(&RR); //espera a que M2 termine
  sem_wait(&RR); //espera a que FN termine
  printf("RR: Reactividad de Reactores\n");
  sem_post(&SR); //para SR
  return NULL;
}

void* func_PR(void* x) {
  sem_wait(&PR); //espera a que FN termine
  sem_wait(&PR); //espera a que MIN termine
  printf("PR: Protección Radiológica\n");
  sem_post(&INU); //para INU
  sem_post(&CB);  //para CB
  sem_post(&SN); //para SN
  return NULL;
}

void* func_SR(void* arg) {
  sem_wait(&SR); //espera a que MF termine
  sem_wait(&SR); //espera a que RR termine
  printf("SR: Sistemas de Reactores\n");
  sem_post(&SN); //para SN
  return NULL;
}

void* func_INU(void* arg) {
  sem_wait(&INU); //espera a que PR termine
  sem_wait(&INU); //espera a que MIN termine
  printf("INU: Instrumentación Nuclear\n");
  return NULL;
}

void* func_CB(void* arg) {
  sem_wait(&CB); //espera a que PR termine
  sem_wait(&CB); //espera a que MIN termine
  printf("CB: Cálculo de Blindajes\n");
  return NULL;
}

void* func_SN(void* arg) {
  sem_wait(&SN); //espera a que PR termine
  sem_wait(&SN); //espera a que SR termine
  printf("SN: Seguridad Nuclear\n");
  return NULL;
}

int main() {
  pthread_t hilos[15];

  //inicializar semaforos
  sem_init(&TD, 0, 0);
  sem_init(&M2, 0, 0);
  sem_init(&FN, 0, 0);
  sem_init(&MF, 0, 0);
  sem_init(&MIN, 0, 0);
  sem_init(&RR, 0, 0);
  sem_init(&PR, 0, 0);
  sem_init(&SR, 0, 0);
  sem_init(&INU, 0, 0);
  sem_init(&CB, 0, 0);
  sem_init(&SN, 0, 0);

  //crear hilos
  pthread_create(&hilos[0], NULL, IN, NULL);
  pthread_create(&hilos[1], NULL, M1, NULL);
  pthread_create(&hilos[2], NULL, F1, NULL);
  pthread_create(&hilos[3], NULL, QG, NULL);
  pthread_create(&hilos[4], NULL, func_TD, NULL);
  pthread_create(&hilos[5], NULL, func_M2, NULL);
  pthread_create(&hilos[6], NULL, func_FN, NULL);
  pthread_create(&hilos[7], NULL, func_MF, NULL);
  pthread_create(&hilos[8], NULL, func_MIN, NULL);
  pthread_create(&hilos[9], NULL, func_RR, NULL);
  pthread_create(&hilos[10], NULL, func_PR, NULL);
  pthread_create(&hilos[11], NULL, func_SR, NULL);
  pthread_create(&hilos[12], NULL, func_INU, NULL);
  pthread_create(&hilos[13], NULL, func_CB, NULL);
  pthread_create(&hilos[14], NULL, func_SN, NULL);

  pthread_join(hilos[0], NULL); 
  pthread_join(hilos[1], NULL); 
  pthread_join(hilos[2], NULL); 
  pthread_join(hilos[3], NULL); 
  pthread_join(hilos[4], NULL); 
  pthread_join(hilos[5], NULL); 
  pthread_join(hilos[6], NULL); 
  pthread_join(hilos[7], NULL); 
  pthread_join(hilos[8], NULL); 
  pthread_join(hilos[9], NULL); 
  pthread_join(hilos[10], NULL); 
  pthread_join(hilos[11], NULL); 
  pthread_join(hilos[12], NULL); 
  pthread_join(hilos[13], NULL); 
  pthread_join(hilos[14], NULL); 

  return 0;
}
