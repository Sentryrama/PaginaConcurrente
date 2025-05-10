#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>


typedef struct Nodo_Red {
    double r_Sitio;
    double r_EIzq;
    double r_EArr;
    int errT1;
    int tipo_S; //Para Sitios, valores: 0 Pequeños, 1 Medianos, 2 Grandes
    int tipo_EIzq; //Para Enlaces Izquierdos, valores: 0 Pequeños, 1 Medianos, 2 Grandes
    int tipo_EArr; //Para Enlaces Arriba, valores: 0 Pequeños, 1 Medianos, 2 Grandes
} NODO_BSM;

//Variables globales para los intervalos
double x1_S, x2_S;
double x1_EIzq, x2_EIzq;
double x1_EArr, x2_EArr;
int hist_S[3];
int hist_EIzq[3];
int hist_EArr[3];

#define INT_MAX 50000000

void inicializarPoblacion(NODO_BSM ***, int , int , int , int , int, int);
void clasificaTamanio(int , NODO_BSM **);
double randomNormal(double , double );
int evaluarFitness(NODO_BSM **, int );
void exportarRedConColores(NODO_BSM **, int , const char *);

int main(int argc, char **argv) {
    int L;

    printf("Proporciona el tamaño de la red: ");
    scanf("%d", &L);

    // Estructuras para medir el tiempo
    struct timespec start, end;

    char *distribucion = argv[2];
    double lim_S1, lim_S2, lim_B1, lim_B2, mediaS, mediaE, varianza_S, varianza_E, desviacion_S, desviacion_E, traslape;

    printf("---------------------------------------------------\n");
    printf("Proporciona el limite inferior de los Sitios: ");
    scanf("%lf", &lim_S1);
    printf("Proporciona el limite superior de los Sitios: ");
    scanf("%lf", &lim_S2);
    mediaS = (double)(lim_S1 + lim_S2) / 2.0; // Corrección en el cálculo de la media
    printf("La media de los Sitios es: %f\n", mediaS);
    varianza_S = (double)((lim_S2 - lim_S1) * (lim_S2 - lim_S1)) / 12.0;
    printf("La varianza de los Sitios es: %f\n", varianza_S);
    desviacion_S = sqrt(varianza_S);
    printf("La desviacion de los Sitios es: %f\n", desviacion_S);    
    printf("---------------------------------------------------\n");
    printf("Proporciona el limite inferior de los Enlaces: ");
    scanf("%lf", &lim_B1);
    printf("Proporciona el limite superior de los Enlaces: ");
    scanf("%lf", &lim_B2);  
    mediaE = (double)(lim_B1 + lim_B2) / 2.0; // Corrección en el cálculo de la media
    printf("La media de los Enlaces es: %f\n", mediaE);          
    varianza_E = (double)((lim_B2 - lim_B1) * (lim_B2 - lim_B1)) / 12.0;
    printf("La varianza de los Enlaces es: %f\n", varianza_E);
    desviacion_E = sqrt(varianza_E);
    printf("La desviacion de los Enlaces es: %f\n", desviacion_E);     
    printf("---------------------------------------------------\n");  
    traslape = (double)(lim_B2 - lim_S1) / (lim_B2 - lim_B1);
    printf("El traslape de la red que vas a generar es: %f\n", traslape); 
    printf("---------------------------------------------------\n");
    srand(time(NULL));

    // Configuración del algoritmo genético
    int numCromosomas = 8; // Tamaño de la población
    int gen = 0; // Número de generaciones

    // Iniciar medición de tiempo
    clock_gettime(CLOCK_MONOTONIC, &start);      

    // Inicializar población
    NODO_BSM ***Poblacion = malloc(numCromosomas * sizeof(NODO_BSM **));
    for (int i = 0; i < numCromosomas; i++) {
        Poblacion[i] = malloc(L * sizeof(NODO_BSM *));
        for (int j = 0; j < L; j++) {
            Poblacion[i][j] = malloc(L * sizeof(NODO_BSM));
        }
    }

    // Reservar memoria para el mejor cromosoma
    NODO_BSM **RED_2D = malloc(L * sizeof(NODO_BSM *));
    for (int i = 0; i < L; i++) {
        RED_2D[i] = malloc(L * sizeof(NODO_BSM));
    }

    inicializarPoblacion(Poblacion, numCromosomas, L, mediaS, desviacion_S, mediaE, desviacion_E);

    exportarRedConColores(RED_2D, L, "red_colores.csv");

    // Liberar memoria
    for (int i = 0; i < numCromosomas; i++) {
        for (int j = 0; j < L; j++) {
            free(Poblacion[i][j]);
        }
        free(Poblacion[i]);
    }
    free(Poblacion);

    // Fin de medición de tiempo
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calcular tiempo en segundos
    double tiempoTotal = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Tiempo total de ejecución: %.2f segundos\n", tiempoTotal);  

    return 0;
}

void inicializarPoblacion(NODO_BSM ***Poblacion, int numCromosomas, int L, int mediaS, int desviacion_S, int mediaE, int desviacion_E) {

    for (int k = 0; k < numCromosomas; k++) {
        if (k == 0) {
            for (int i = 0; i < L; i++) {
                for (int j = 0; j < L; j++) {
                    Poblacion[k][i][j].r_Sitio = randomNormal(mediaS, desviacion_S);
                    Poblacion[k][i][j].r_EIzq = randomNormal(mediaE, desviacion_E);
                    Poblacion[k][i][j].r_EArr = randomNormal(mediaE, desviacion_E);
                    Poblacion[k][i][j].errT1 = 0;
                    Poblacion[k][i][j].errG = 0;
                    Poblacion[k][i][j].tipo_S = -1;
                    Poblacion[k][i][j].tipo_EIzq = -1;
                    Poblacion[k][i][j].tipo_EArr = -1;
                }
            }
            //Modulo para calcular los intervalos de división de los Sitios
            x1_S = (-0.44*desviacion_S)+mediaS;
            x2_S = (0.42*desviacion_S)+mediaS;
            //Modulo para calcular los intervalos de división de los Enlaces Izquierdos 
            x1_EIzq = (-0.44*desviacion_E)+mediaE;
            x2_EIzq = (0.42*desviacion_E)+mediaE;
            //Modulo para calcular los intervalos de división de los Enlaces Arriba
            x1_EArr = (-0.44*desviacion_E)+mediaE;
            x2_EArr = (0.42*desviacion_E)+mediaE;

            clasificaTamanio(L, Poblacion[k]);      
            printf("---------------------------------------------------\n");                              
        } else {
            // Réplica de la población anterior para cromosomas adicionales
            for (int i = 0; i < L; i++) {
                for (int j = 0; j < L; j++) {
                    Poblacion[k][i][j].r_Sitio = Poblacion[k - 1][i][j].r_Sitio;
                    Poblacion[k][i][j].r_EIzq = Poblacion[k - 1][i][j].r_EIzq;
                    Poblacion[k][i][j].r_EArr = Poblacion[k - 1][i][j].r_EArr;
                    Poblacion[k][i][j].errT1 = Poblacion[k - 1][i][j].errT1;
                    Poblacion[k][i][j].errG = Poblacion[k - 1][i][j].errG;
                    Poblacion[k][i][j].tipo_S = Poblacion[k-1][i][j].tipo_S;
                    Poblacion[k][i][j].tipo_EIzq = Poblacion[k-1][i][j].tipo_EIzq;
                    Poblacion[k][i][j].tipo_EArr = Poblacion[k-1][i][j].tipo_EArr;                    
                }
            }
        }
    }
}

//Metodo para clasificar tamaños de red de sitios y enlaces
void clasificaTamanio(int L, NODO_BSM **RED_2D){
    // Clasificar nodos en pequeños, medianos y grandes
    for (int i = 0; i < 3; i++) {
        hist_S[i] = 0; // Inicializar histograma de los sitios
    }

    // Clasificar nodos en pequeños, medianos y grandes
    for (int i = 0; i < 3; i++) {
        hist_EIzq[i] = 0; // Inicializar histograma de los enlaces izquierdos
    }

    // Clasificar nodos en pequeños, medianos y grandes
    for (int i = 0; i < 3; i++) {
        hist_EArr[i] = 0; // Inicializar histograma de los enlaces arriba
    }                        

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            //Clasificación de los sitios
            if (RED_2D[i][j].r_Sitio <= x1_S) {
                RED_2D[i][j].tipo_S = 0; // Pequeños
                hist_S[0]++;
            } else if (RED_2D[i][j].r_Sitio <= x2_S) {
                RED_2D[i][j].tipo_S = 1; // Medianos
                hist_S[1]++;
            } else {
                RED_2D[i][j].tipo_S = 2; // Grandes
                hist_S[2]++;
            }

            //Clasificación de los enlaces izquierdo
            if (RED_2D[i][j].r_EIzq <= x1_EIzq) {
                RED_2D[i][j].tipo_EIzq = 0; // Pequeños
                hist_EIzq[0]++;
            } else if (RED_2D[i][j].r_EIzq <= x2_EIzq) {
                RED_2D[i][j].tipo_EIzq = 1; // Medianos
                hist_EIzq[1]++;
            } else {
                RED_2D[i][j].tipo_EIzq = 2; // Grandes
                hist_EIzq[2]++;
            }

            //Clasificación de los sitios
            if (RED_2D[i][j].r_EArr <= x1_EArr) {
                RED_2D[i][j].tipo_EArr = 0; // Pequeños
                hist_EArr[0]++;
            } else if (RED_2D[i][j].r_EArr <= x2_EArr) {
                RED_2D[i][j].tipo_EArr = 1; // Medianos
                hist_EArr[1]++;
            } else {
                RED_2D[i][j].tipo_EArr = 2; // Grandes
                hist_EArr[2]++;
            }                                        
        }
    }

    printf("Histograma de colores para los Sitios: Amarillo(Pequeños) = %d, Azul(Medianos) = %d, Rojo(Grandes) = %d\n", hist_S[0], hist_S[1], hist_S[2]);
    printf("Histograma de colores para los Enlaces Izquierdos: Amarillo(Pequeños) = %d, Azul(Medianos) = %d, Rojo(Grandes) = %d\n", hist_EIzq[0], hist_EIzq[1], hist_EIzq[2]);
    printf("Histograma de colores para los Enlaces Arriba: Amarillo(Pequeños) = %d, Azul(Medianos) = %d, Rojo(Grandes) = %d\n", hist_EArr[0], hist_EArr[1], hist_EArr[2]);

}

// Función para generar números aleatorios con distribución normal
double randomNormal(double media, double varianza) {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2); // Distribución normal estándar
    return z * varianza + media; // Transformación a la distribución deseada
}

void exportarRedConColores(NODO_BSM **RED_2D, int L, const char *filename) {
    // Encontrar los valores mínimo y máximo de r_Sitio
    int hist_S_F[3] = {0, 0, 0};
    
    printf("Valores finales de sitios para intervalor: x1_S =  %f, x2_S = %f\n", x1_S, x2_S);

    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            if (RED_2D[i][j].r_Sitio <= x1_S) {
                hist_S_F[0]++;
            } else if (RED_2D[i][j].r_Sitio <= x2_S) {
                hist_S_F[1]++;
            } else {
                hist_S_F[2]++;
            }
        }
    }

    printf("Histograma de colores para los Sitios: Amarillo(Pequeños) = %d, Azul(Medianos) = %d, Rojo(Grandes) = %d\n", hist_S[0], hist_S[1], hist_S[2]);

    // Abrir archivo para escritura
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error al abrir el archivo para escritura.\n");
        return;
    }

    // Escribir encabezado
    fprintf(file, "x,y,r_Sitio,r_EIzq,r_EDer,r_EArr,r_EAbj,color\n");

    // Asignar colores basados en los intervalos dinámicos
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            const char *color;
            if (RED_2D[i][j].r_Sitio <= x1_S) {
                color = "yellow"; // Sitios pequeños
            } else if (RED_2D[i][j].r_Sitio <= x2_S) {
                color = "blue"; // Sitios medianos
            } else {
                color = "red"; // Sitios grandes
            }
            fprintf(file, "%d,%d,%f,%f,%f,%f,%f,%s\n", i, j, RED_2D[i][j].r_Sitio, RED_2D[i][j].r_EIzq, RED_2D[i][(j + 1) % L].r_EIzq, RED_2D[i][j].r_EArr,  RED_2D[(i + 1) % L][j].r_EArr,color);
        }
    }

    fclose(file);
    printf("Archivo %s generado con éxito.\n", filename);
}
