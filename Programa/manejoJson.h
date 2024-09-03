#ifndef OBJETOS_JSON_H
#define OBJETOS_JSON_H

#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listaJson{
    int venta_id;
    char fecha[50];
    int producto_id;
    char nombre[50];
    char categoria[50];
    int cantidad;
    int precio;
    int total;

    struct listaJson * siguiente;

} listaJson;


int buscar(int id, listaJson* l){
    if(l==NULL){return 0;}
    listaJson *temp = l;
    while(temp!=NULL){
        if (temp->venta_id == id){return 1;}
     
        temp = temp->siguiente;
    }

    return 0;
}

char* getNombre(int id, listaJson* l){
    if(l==NULL){return NULL;}
    listaJson *temp = l;
    while(temp!=NULL){
        if (temp->venta_id == id){return temp->nombre;}
     
        temp = temp->siguiente;
    }

    return NULL;
}

//entrada: id de venta y datos del Json en forma de lista, retorna cantidad de ventas realizadas por articulo y el precio unitario
void completarDatos (int id_obj, listaJson* l){
    if(buscar(id_obj, l)==0){return;}
    listaJson *temp = l;
    char* nombre = getNombre(id_obj, l);
    int cantidad = 0;
    int precios = 0;
    while (temp != NULL){
        if (strcmp(temp->nombre, nombre)==0){
            cantidad++;
            precios += temp->precio;
        }
        temp = temp->siguiente;

    }

    precios = precios / cantidad; //media de precios = suma de precios/cant de objetos
    printf("cantidad: %d - precio: %d\n", cantidad, precios);
}

//eliminar los datos dublicados con el id identificar 
listaJson* borrarDuplicados(int id_excepcion, listaJson * cabeza) {
    listaJson * current = cabeza;
    listaJson * prev = NULL;
    listaJson * temp = NULL;
    char* nombre_excepcion = getNombre(id_excepcion, cabeza);

    while (current != NULL) {
        // Si el nodo actual tiene el nombre que queremos eliminar y no es el nodo con el ID especificado
        if (strcmp(current->nombre, nombre_excepcion) == 0 && current->venta_id != id_excepcion) {
            // Eliminar el nodo actual
            if (prev == NULL) {
                // Caso especial: eliminar el primer nodo
                listaJson * temp = cabeza;
                cabeza = cabeza->siguiente;
                free(temp);
                current = cabeza; // Continuar con el nuevo primer nodo
            } else {
                // Eliminar nodo en medio o final
                prev->siguiente = current->siguiente;
                free(current);
                current = prev->siguiente; // Continuar con el siguiente nodo
            }
        } else {
            // Mover a la siguiente posición en la lista
            prev = current;
            current = current->siguiente;
        }
    }

    return cabeza;
}


//devuelve los datos del json en una lista
void printLista(listaJson* l){
    if(l->venta_id==0){return;}
    listaJson *temp = l;
    while(temp!=NULL){
        printf("\tVentaID: %d\n", temp->venta_id);
        printf("\tFecha: %s\n", temp->fecha);
        printf("\tProducto ID: %d\n", temp->producto_id);
        printf("\tNombre: %s\n", temp->nombre);
        printf("\tCategoria: %s\n", temp->categoria);
        printf("\tCantidad: %d\n", temp->cantidad);
        printf("\tPrecio: %d\n", temp->precio);
        printf("\tTotal:%d\n\n", temp->total);
        temp = temp->siguiente;
    }
}

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
struct json_object* node_to_json_object(listaJson *node) {
    struct json_object *jobj = json_object_new_object();
    
    json_object_object_add(jobj, "venta_id", json_object_new_int(node->venta_id));
    json_object_object_add(jobj, "fecha", json_object_new_string(node->fecha));
    json_object_object_add(jobj, "producto_id", json_object_new_int(node->producto_id));
    json_object_object_add(jobj, "nombre", json_object_new_string(node->nombre));
    json_object_object_add(jobj, "categoria", json_object_new_string(node->categoria));
    json_object_object_add(jobj, "cantidad", json_object_new_int(node->cantidad));
    json_object_object_add(jobj, "precio", json_object_new_int(node->precio));
    json_object_object_add(jobj, "total", json_object_new_int(node->total));
    
    return jobj;
}

// Función para escribir un array JSON en un archivo
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

// Función para exportar la lista a un archivo JSON, pidiendo el nombre del archivo al usuario
void export_list_to_json_file(listaJson *cabeza) {
    char base_filename[256];
    
    // Pedir al usuario el nombre del archivo (sin la extensión)
    printf("Introduce el nombre del archivo (sin la extensión): ");
    scanf("%255s", base_filename); // Leer el nombre del archivo, limitar a 255 caracteres para evitar desbordamientos

    // Crear un array JSON
    struct json_object *json_array = json_object_new_array();
    
    // Recorrer la lista y agregar cada nodo al array JSON
    listaJson *current = cabeza;
    while (current != NULL) {
        struct json_object *json_object = node_to_json_object(current);
        json_object_array_add(json_array, json_object);
        current = current->siguiente;
    }
    
    // Crear el nombre del archivo con la extensión .json
    char filename[256];
    snprintf(filename, sizeof(filename), "%s.json", base_filename);
    
    // Escribir el array JSON al archivo
    write_json_array_to_file(filename, json_array);
    
    // Liberar la memoria del array JSON
    json_object_put(json_array);
}

listaJson* insertarAlFinal(listaJson * cabeza, listaJson * nodo) {
    if (cabeza == NULL) {
        // Si la lista está vacía, el nuevo nodo es la cabeza
        return nodo;
    }

    // Si la lista no está vacía, encontrar el último nodo
    listaJson * actual = cabeza;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Insertar el nuevo nodo al final
    actual->siguiente = nodo;
    nodo->siguiente = NULL; // El nuevo último nodo debe apuntar a NULL

    return cabeza;
}

// Función para leer y mostrar los atributos de cada JSON en el array
listaJson* read_json_from_file(const char *filename) {
    printf("%s\n", filename);
    struct json_object *json_array = read_or_create_json_array(filename);
    listaJson* l = NULL;

    // Verificar si es un array
    if (json_object_get_type(json_array) == json_type_array) {
        int array_len = json_object_array_length(json_array);
        for (int i = 0; i < array_len; i++) {
            listaJson* n = (listaJson*)malloc(sizeof(listaJson));
            
            struct json_object *obj = json_object_array_get_idx(json_array, i);

            // Extraer e imprimir los atributos
            struct json_object *id_obj, *fecha_obj, *proID_obj, *name_obj, *categoria_obj, *cantidad_obj, *precio_obj, *total_obj;
            if (json_object_object_get_ex(obj, "id", &id_obj)) {
                n->venta_id = json_object_get_int(id_obj);
                
            }

            if (json_object_object_get_ex(obj, "fecha", &fecha_obj)) {
                strncpy(n->fecha, json_object_get_string(fecha_obj), 50);
                
            }

            if (json_object_object_get_ex(obj, "proID", &proID_obj)) {
                n->producto_id = json_object_get_int(proID_obj);
                

            }

            if (json_object_object_get_ex(obj, "name", &name_obj)) {
                strncpy(n->nombre, json_object_get_string(name_obj), 50);
                    
            }

            if (json_object_object_get_ex(obj, "categoria", &categoria_obj)) {
                strncpy(n->categoria, json_object_get_string(categoria_obj), 50);
                
            }
            
            if (json_object_object_get_ex(obj, "cantidad", &cantidad_obj)) {
                
                    n->cantidad = json_object_get_int(cantidad_obj);
                
            }

            if (json_object_object_get_ex(obj, "precio", &precio_obj)) {
                n->precio = json_object_get_int(precio_obj);
                
            }

            if (json_object_object_get_ex(obj, "total", &total_obj)) {
                n->total = json_object_get_int(total_obj);
                
            }
            l=insertarAlFinal(l,n);
            
        }
    }

    // Liberar la memoria
    json_object_put(json_array);
    return l;
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

#endif 