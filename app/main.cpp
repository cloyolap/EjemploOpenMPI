#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // libreria para el uso de time() hora,min..etc

/* cabecera de las llamadas a MPI */
#include <mpi.h> 

/**
 * Manda un mensaje desde todos los procesos con
 * rango distinto de 0 al proceso 0. El proceso 0
 * los saca por pantalla.
 * Entrada: ninguna
 * Salida: contenido de los mensajes recibidos por 
 * el proceso 0, que los imprime
 */
int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int nom_largo; /* largo del nombre */
    int tag = 0; /* etiqueta del mensaje */

    
    /* Obtenemos la hora de la maquina   */
    time_t tTiempo; //Declaro una variable para la Estructura que maneja el tiempo
    struct tm *mihora; //Declaro un apuntador para la estructura del tiempo
    char strFechaHora[100]; //Cadena para obtener la informaciòn de la fecha
    tTiempo = time(NULL); // Obtenemos el tiempo
    mihora = localtime(&tTiempo); // Obtemos el tiempo local
    strftime(strFechaHora,100, "%Y-%m-%d %H:%M:%S",mihora); //Formateamos el aputandor hacia la cadena

    /* Obtemos el nombre del localhost 
    de los procesadores
   */
    char nombre[MPI_MAX_PROCESSOR_NAME];

    char mensaje[100]; /* mensaje [100]  */
    MPI_Status estado; /* devuelve estado al recibir*/

    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

    /* Averiguamos el número de procesos que estan 
     * ejecutando nuestro porgrama 
     */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    /* Averiguamos el nombre de nuestra maquina
    que se estan ejecutando
     */
    MPI_Get_processor_name(nombre, &nom_largo);

    if (mi_rango != 0) {
	//int seg = mihora->tm_sec;
        /* Crea mensaje */
        sprintf(mensaje,"%s %s",nombre,strFechaHora);

        dest = 0;
        /* Usa strlen+1 para que la marca /0 se transmita */
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,
                dest, tag, MPI_COMM_WORLD);
    } else { /* mi_rango == 0 */
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(mensaje, 100, MPI_CHAR, fuente,
                    tag, MPI_COMM_WORLD, &estado);
            printf("%s\n", mensaje);
        }
    }

    /* Termina con MPI. Recordemos que después de 
     * esta llamada no podemos llamar a funciones 
     * MPI, ni siquiera de nuevo a MPI_Init 
     */
    MPI_Finalize();
    return EXIT_SUCCESS;
}
