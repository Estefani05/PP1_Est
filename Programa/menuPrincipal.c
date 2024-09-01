#include <stdio.h>
#include "manejoJson.h"

listaJson* importarDatos() {
    char base_filename[256];
    
    // Pedir al usuario el nombre del archivo (sin la extensión)
    printf("Introduce el nombre del archivo: ");
    scanf("%255s", base_filename); // Leer el nombre del archivo, limitar a 255 caracteres para evitar desbordamientos

    listaJson* l = read_json_from_file(base_filename);
    printLista(l);
    printf("Función de importación de datos ejecutada.\n");
    return l;
}

listaJson* procesarDatos(listaJson* l) {
    int opcion;
    do {
        
        printf("\n--- Sub menu procesar datos ---\n");
        printf("1. Completar datos faltantes\n");
        printf("2. Eliminar datos duplicados\n");
        printf("3. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:{
                printLista(l);
                printf("\nelija el id de venta:\n");
                int id;
                scanf("%d", &id);
                completarDatos(id, l);
                break;
            }case 2:{
                printLista(l);
                printf("\nelija el id de venta:\n");
                int id;
                scanf("%d", &id);
                l = borrarDuplicados(id, l);
                printLista(l);
                break;
            }case 3:
                return l;
         
            default:
                printf("Opción no válida.\n");
        }
    } while(opcion != 3);

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
    listaJson* lista;
    
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
                lista = importarDatos();
                break;
            case 2:
                if(lista == NULL){
                    printf("No hay datos para procesar\n\n");
                    break;
                }
                lista = procesarDatos(lista);
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
                export_list_to_json_file(lista);
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while(opcion != 6);

    return 0;
}