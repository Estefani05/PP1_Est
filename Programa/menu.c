#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Función para leer el archivo JSON o crear uno nuevo si no existe
struct json_object *read_or_create_json_array(const char *filename) {
    FILE *file = fopen(filename, "r");
    struct json_object *json_array;

    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (length > 0) {
            char *json_data = malloc(length + 1);
            fread(json_data, 1, length, file);
            json_data[length] = '\0';
            fclose(file);

            // Parsear el JSON leído
            json_array = json_tokener_parse(json_data);
            free(json_data);

            if (json_object_get_type(json_array) != json_type_array) {
                json_array = json_object_new_array();
            }
        } else {
            fclose(file);
            json_array = json_object_new_array(); // Archivo vacío, crear un nuevo array
        }
    } else {
        json_array = json_object_new_array(); // Archivo no existe, crear un nuevo array
    }

    return json_array;
}

// Función para escribir el array JSON en el archivo
void write_json_array_to_file(const char *filename, struct json_object *json_array) {
    // Abrir el archivo en modo escritura (sobrescribe el archivo)
    FILE *file = fopen(filename, "w");
    if (file) {
        const char *json_string = json_object_to_json_string(json_array);
        fprintf(file, "%s", json_string);
        fclose(file);
    } else {
        printf("No se pudo abrir el archivo para escribir.\n");
    }
}

// Función para leer y mostrar los atributos de cada JSON en el array
void read_json_from_file(const char *filename) {
    struct json_object *json_array = read_or_create_json_array(filename);

    // Verificar si es un array
    if (json_object_get_type(json_array) == json_type_array) {
        int array_len = json_object_array_length(json_array);
        for (int i = 0; i < array_len; i++) {
            struct json_object *obj = json_object_array_get_idx(json_array, i);

            // Extraer e imprimir los atributos
            struct json_object *id_obj, *fecha_obj, *proID_obj, *name_obj, *categoria_obj, *cantidad_obj, *precio_obj, *total_obj;
            if (json_object_object_get_ex(obj, "id", &id_obj)) {
                printf("ID: %d\n", json_object_get_int(id_obj));
            }

            if (json_object_object_get_ex(obj, "fecha", &fecha_obj)) {
                printf("Fecha: %s\n", json_object_get_string(fecha_obj));
            }

            if (json_object_object_get_ex(obj, "proID", &proID_obj)) {
                printf("Producto ID: %d\n", json_object_get_int(proID_obj));
            }

            if (json_object_object_get_ex(obj, "name", &name_obj)) {
                printf("Nombre del producto: %s\n", json_object_get_string(name_obj));
            }

            if (json_object_object_get_ex(obj, "categoria", &categoria_obj)) {
                printf("Categoria: %s\n", json_object_get_string(categoria_obj));
            }
            
            if (json_object_object_get_ex(obj, "cantidad", &cantidad_obj)) {
                printf("Cantidad: %d\n", json_object_get_int(cantidad_obj));
            }

            if (json_object_object_get_ex(obj, "precio", &precio_obj)) {
                printf("Precio unitario: %d\n", json_object_get_int(precio_obj));
            }

            if (json_object_object_get_ex(obj, "total", &total_obj)) {
                printf("Total: %d\n", json_object_get_int(total_obj));
            }


            printf("\n");
        }
    }

    // Liberar la memoria
    json_object_put(json_array);
}

// Función para obtener el siguiente ID
int get_next_id(const char *filename) {
    struct json_object *json_array = read_or_create_json_array(filename);

    // Verificar si el array está vacío
    if (json_object_array_length(json_array) == 0) {
        json_object_put(json_array);
        return 1; // Archivo vacío, devolver ID 1
    }

    // Obtener el último objeto del array
    struct json_object *last_obj = json_object_array_get_idx(json_array, json_object_array_length(json_array) - 1);

    // Obtener el ID del último objeto
    struct json_object *id_obj;
    int last_id = 1;
    if (json_object_object_get_ex(last_obj, "id", &id_obj)) {
        last_id = json_object_get_int(id_obj);
    }

    json_object_put(json_array);

    return last_id + 1; // Devolver el siguiente ID
}

int main() {
    char option;
    const char *filename = "output.json";

    while (1) {
        printf("Seleccione una opción:\n");
        printf("a) Agregar al JSON\n");
        printf("b) Ver JSON\n");
        printf("c) Salir\n");
        printf("Opción: ");
        scanf(" %c", &option);

        if (option == 'a') {
            int proID;
            char name[50];
            char categoria[50];
            int cantidad;
            int precio;
            int total;

            printf("Ingrese el ID del producto: ");
            scanf("%d", &proID);
            printf("Ingrese el nombre del producto: ");
            scanf("%s", name);
            printf("Ingrese la categoria del producto: ");
            scanf("%s", categoria);
            printf("Ingrese la cantidad: ");
            scanf("%d", &cantidad);
            printf("Ingrese el precio unitario: ");
            scanf("%d", &precio);
            printf("Ingrese el total: ");
            scanf("%d", &total);

            // Obtener el siguiente ID
            int next_id = get_next_id(filename);
            
            //Obtener fecha
            time_t t = time(NULL); // Obtener el tiempo actual
            struct tm tm = *localtime(&t); // Convertir a estructura tm

            // Crear un arreglo de caracteres para almacenar la fecha
            char fecha[11]; // Formato "yyyy-mm-dd" + terminador nulo

            // Formatear la fecha y almacenarla en el arreglo
            snprintf(fecha, sizeof(fecha), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);


            // Leer el array JSON existente o crear uno nuevo
            struct json_object *json_array = read_or_create_json_array(filename);

            // Crear un nuevo objeto JSON
            struct json_object *json_obj = json_object_new_object();
            json_object_object_add(json_obj, "id", json_object_new_int(next_id));

            json_object_object_add(json_obj, "fecha", json_object_new_string(fecha));

            json_object_object_add(json_obj, "proID", json_object_new_int(proID));
            json_object_object_add(json_obj, "name", json_object_new_string(name));
            json_object_object_add(json_obj, "categoria", json_object_new_string(categoria));
            
            json_object_object_add(json_obj, "cantidad", json_object_new_int(cantidad));
            json_object_object_add(json_obj, "precio", json_object_new_int(precio));
            json_object_object_add(json_obj, "total", json_object_new_int(total));

            

            // Agregar el nuevo objeto al array
            json_object_array_add(json_array, json_obj);

            // Escribir el array actualizado al archivo
            write_json_array_to_file(filename, json_array);

            // Liberar la memoria
            json_object_put(json_array);

        } else if (option == 'b') {
            // Leer y mostrar el contenido del archivo JSON
            read_json_from_file(filename);

        } else if (option == 'c') {
            // Salir del programa
            printf("Saliendo...\n");
            break;
        } else {
            printf("Opción no válida, intente de nuevo.\n");
        }
    }

    return 0;
}