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

int totalVentas(listaJson* l){
    if(l->venta_id==0){return 0;}
    listaJson *temp = l;
    int suma = 0;
    while(temp!=NULL){
        suma +=  temp->total;
        temp = temp->siguiente;
    }
    return suma;
}

typedef struct listaMes {
    int mes;
    int total;
    struct listaMes* siguiente;
} listaMes;

typedef struct listaAnio {
    int anio;
    listaMes* meses;
    struct listaAnio* siguiente;
} listaAnio;

void agregarTotalMes(listaMes** lista, int mes, int total) {
    listaMes* temp = *lista;
    listaMes* anterior = NULL;

    while (temp != NULL && temp->mes < mes) {
        anterior = temp;
        temp = temp->siguiente;
    }

    if (temp != NULL && temp->mes == mes) {
        temp->total += total;
    } else {
        listaMes* nuevoMes = (listaMes*)malloc(sizeof(listaMes));
        nuevoMes->mes = mes;
        nuevoMes->total = total;
        nuevoMes->siguiente = temp;

        if (anterior == NULL) {
            *lista = nuevoMes;
        } else {
            anterior->siguiente = nuevoMes;
        }
    }
}

void agregarTotalAnio(listaAnio** lista, int anio, int mes, int total) {
    listaAnio* temp = *lista;
    listaAnio* anterior = NULL;

    while (temp != NULL && temp->anio < anio) {
        anterior = temp;
        temp = temp->siguiente;
    }

    if (temp != NULL && temp->anio == anio) {
        agregarTotalMes(&(temp->meses), mes, total);
    } else {
        listaAnio* nuevoAnio = (listaAnio*)malloc(sizeof(listaAnio));
        nuevoAnio->anio = anio;
        nuevoAnio->meses = NULL;
        nuevoAnio->siguiente = temp;

        agregarTotalMes(&(nuevoAnio->meses), mes, total);

        if (anterior == NULL) {
            *lista = nuevoAnio;
        } else {
            anterior->siguiente = nuevoAnio;
        }
    }
}

void imprimirTotales(listaJson* lista) {
    listaAnio* listaAgrupada = NULL;
    listaJson* temp = lista;
    int anio, mes;

    // Agrupar las ventas por año y mes
    while (temp != NULL) {
        sscanf(temp->fecha, "%d-%d", &anio, &mes);
        agregarTotalAnio(&listaAgrupada, anio, mes, temp->total);
        temp = temp->siguiente;
    }

    // Imprimir los totales agrupados
    listaAnio* tempAnio = listaAgrupada;
    while (tempAnio != NULL) {
        printf("Año %d\n", tempAnio->anio);
        listaMes* tempMes = tempAnio->meses;
        while (tempMes != NULL) {
            printf("\tMes %d: Total = %d\n", tempMes->mes, tempMes->total);
            tempMes = tempMes->siguiente;
        }
        tempAnio = tempAnio->siguiente;
    }

    // Liberar la memoria usada
    tempAnio = listaAgrupada;
    while (tempAnio != NULL) {
        listaMes* tempMes = tempAnio->meses;
        while (tempMes != NULL) {
            listaMes* mesAEliminar = tempMes;
            tempMes = tempMes->siguiente;
            free(mesAEliminar);
        }
        listaAnio* anioAEliminar = tempAnio;
        tempAnio = tempAnio->siguiente;
        free(anioAEliminar);
    }
}

typedef struct listaCategoria {
    char categoria[50];
    int total;
    struct listaCategoria* siguiente;
} listaCategoria;

void agregarTotalCategoria(listaCategoria** lista, char* categoria, int total) {
    listaCategoria* temp = *lista;
    listaCategoria* anterior = NULL;

    while (temp != NULL && strcmp(temp->categoria, categoria) < 0) {
        anterior = temp;
        temp = temp->siguiente;
    }

    if (temp != NULL && strcmp(temp->categoria, categoria) == 0) {
        temp->total += total;
    } else {
        listaCategoria* nuevaCategoria = (listaCategoria*)malloc(sizeof(listaCategoria));
        strcpy(nuevaCategoria->categoria, categoria);
        nuevaCategoria->total = total;
        nuevaCategoria->siguiente = temp;

        if (anterior == NULL) {
            *lista = nuevaCategoria;
        } else {
            anterior->siguiente = nuevaCategoria;
        }
    }
}

void ordenarCategoriasPorTotal(listaCategoria** lista) {
    if (*lista == NULL) return;

    listaCategoria* sorted = NULL;
    listaCategoria* current = *lista;

    while (current != NULL) {
        listaCategoria* siguiente = current->siguiente;
        if (sorted == NULL || current->total > sorted->total) {
            current->siguiente = sorted;
            sorted = current;
        } else {
            listaCategoria* temp = sorted;
            while (temp->siguiente != NULL && temp->siguiente->total > current->total) {
                temp = temp->siguiente;
            }
            current->siguiente = temp->siguiente;
            temp->siguiente = current;
        }
        current = siguiente;
    }

    *lista = sorted;
}

void imprimirTop5Categorias(listaJson* lista) {
    listaCategoria* listaCategorias = NULL;
    listaJson* temp = lista;

    // Agrupar por categoría
    while (temp != NULL) {
        agregarTotalCategoria(&listaCategorias, temp->categoria, temp->total);
        temp = temp->siguiente;
    }

    // Ordenar las categorías por el total de ventas
    ordenarCategoriasPorTotal(&listaCategorias);

    // Imprimir el top 5
    printf("Top 5 Categorías por Precio Total:\n");
    listaCategoria* tempCategoria = listaCategorias;
    int contador = 0;
    while (tempCategoria != NULL && contador < 5) {
        printf("%d. Categoría: %s - Total: %d\n", contador + 1, tempCategoria->categoria, tempCategoria->total);
        tempCategoria = tempCategoria->siguiente;
        contador++;
    }

    // Liberar la memoria usada
    while (listaCategorias != NULL) {
        listaCategoria* categoriaAEliminar = listaCategorias;
        listaCategorias = listaCategorias->siguiente;
        free(categoriaAEliminar);
    }
}


void analizarDatos(listaJson* l) {
    int opcion;
    do {
        
        printf("\n--- Sub menu Analisis de datos ---\n");
        printf("1. Total de todas las ventas\n");
        printf("2. Total de ventas mensuales y anuales\n");
        printf("3. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:{
                printf("\n El total de la suma es: %d\n", totalVentas(l));
                break;
            }
            case 2:{
                imprimirTotales(l);
                break;
            }
            case 3:{
                return 1;
            }
            default:{
                break;
            }

        }
    }while (1);

    



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
                analizarDatos(lista);
                break;
            case 4:
                analisisTemporal();
                break;
            case 5:
                imprimirTop5Categorias(lista);
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