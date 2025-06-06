#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

// string.h compara codigos entre las materias

#define NUM_MATERIAS 15

typedef struct Materia Materia;

struct Materia {
    char* codigo;
    char* nombre;
    int numPrevias;
    char** previas;
    int dependenciasRestantes;
    sem_t puedeCursar;
};

// Lista de materias
Materia materias[NUM_MATERIAS];

// Para encontrar índice de una materia
int buscarIndice(const char* codigo) {
    for (int i = 0; i < NUM_MATERIAS; i++)
        if (strcmp(materias[i].codigo, codigo) == 0)
            return i;
    return -1;
}

// Hilo por materia
void* cursarMateria(void* arg) {
    Materia* mat = (Materia*)arg;

    // Esperar a que todas las dependencias se liberen
    sem_wait(&mat->puedeCursar);

    // Simular cursado
    printf("Cursando: %s (%s)\n", mat->nombre, mat->codigo);

    // Liberar materias que dependen de esta
    for (int i = 0; i < NUM_MATERIAS; i++) {
        Materia* m = &materias[i];
        for (int j = 0; j < m->numPrevias; j++) {
            if (strcmp(m->previas[j], mat->codigo) == 0) {
                m->dependenciasRestantes--;
                if (m->dependenciasRestantes == 0) {
                    sem_post(&m->puedeCursar);
                }
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_MATERIAS];

    // Materias de la letra del OBL, si se quieren mas, se cambia la constante y agega en la siguiente
    // posicion con sus dependencias, y se modifican otras si alguna depende de ella
    materias[0] = (Materia){"IN", "Introducción a la Ingeniería Nuclear", 0, NULL, 0};
    materias[1] = (Materia){"M1", "Matemáticas I", 0, NULL, 0};
    materias[2] = (Materia){"F1", "Física I", 0, NULL, 0};
    materias[3] = (Materia){"QG", "Química General", 0, NULL, 0};
    materias[4] = (Materia){"TD", "Termodinámica", 2, (char*[]){"F1", "QG"}, 2};
    materias[5] = (Materia){"M2", "Matemáticas II", 1, (char*[]){"M1"}, 1};
    materias[6] = (Materia){"FN", "Física Nuclear", 2, (char*[]){"F1", "M2"}, 2};
    materias[7] = (Materia){"MF", "Mecánica de Fluidos", 2, (char*[]){"TD", "M2"}, 2};
    materias[8] = (Materia){"MIN", "Materiales en Ingeniería Nuclear", 2, (char*[]){"QG", "FN"}, 2};
    materias[9] = (Materia){"RR", "Reactividad de Reactores", 2, (char*[]){"FN", "M2"}, 2};
    materias[10] = (Materia){"PR", "Protección Radiológica", 2, (char*[]){"FN", "MIN"}, 2};
    materias[11] = (Materia){"SR", "Sistemas de Reactores", 2, (char*[]){"RR", "MF"}, 2};
    materias[12] = (Materia){"INU", "Instrumentación Nuclear", 2, (char*[]){"FN", "PR"}, 2};
    materias[13] = (Materia){"CB", "Cálculo de Blindajes", 2, (char*[]){"PR", "MIN"}, 2};
    materias[14] = (Materia){"SN", "Seguridad Nuclear", 2, (char*[]){"PR", "SR"}, 2};

    // Inicializar semáforos
    for (int i = 0; i < NUM_MATERIAS; i++) {
        sem_init(&materias[i].puedeCursar, 0, 0);
    }

    // Inicializar dependencias restantes y lanzar hilos
    for (int i = 0; i < NUM_MATERIAS; i++) {
        if (materias[i].numPrevias == 0) {
            // ya puede cursarse
            sem_post(&materias[i].puedeCursar);  
        }
        materias[i].dependenciasRestantes = materias[i].numPrevias;
        pthread_create(&hilos[i], NULL, cursarMateria, &materias[i]);
    }

    // Esperar finalización
    for (int i = 0; i < NUM_MATERIAS; i++) {
        pthread_join(hilos[i], NULL);
        sem_destroy(&materias[i].puedeCursar);
    }

    printf("Termino el plan de estudios!\n");
    return 0;
}