#include <stdio.h>

void importarDatos() {
    printf("Función de importación de datos ejecutada.\n");
}

void procesarDatos() {
    printf("Función de procesamiento de datos ejecutada.\n");
}

void analizarDatos() {
    printf("Función de análisis de datos ejecutada.\n");
}

void analisisTemporal() {
    printf("Función de análisis temporal ejecutada.\n");
}

void mostrarEstadisticas() {
    printf("Función de estadísticas ejecutada.\n");
}

int main() {
    int opcion;

    do {
        printf("\n--- Menú ---\n");
        printf("1. Importación de datos\n");
        printf("2. Procesamiento de datos\n");
        printf("3. Análisis de datos\n");
        printf("4. Análisis temporal\n");
        printf("5. Estadísticas\n");
        printf("6. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                importarDatos();
                break;
            case 2:
                procesarDatos();
                break;
            case 3:
                analizarDatos();
                break;
            case 4:
                analisisTemporal();
                break;
            case 5:
                mostrarEstadisticas();
                break;
            case 6:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while(opcion != 6);

    return 0;
}