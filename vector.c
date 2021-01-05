#include "vector.h"
#include <stdio.h>
#define CAPACIDAD_MINIMA 8
#define FACTOR_REDIMENSION 2
void swap(int *x, int *y);


struct vector {
    void** datos;
    size_t capacidad;
    size_t cantidad;
};

vector_t* vector_crear_vacio(){
    vector_t* vector = malloc(sizeof(vector_t));
    if(!vector){
        return NULL;
    }
    vector->datos = malloc(sizeof(void*) * CAPACIDAD_MINIMA);
    if(!vector->datos){
        free(vector);
        return NULL;
    }
    vector->cantidad = 0;
    vector->capacidad = CAPACIDAD_MINIMA;

    for(int i = 0; i < CAPACIDAD_MINIMA; i++){
        vector->datos[i] = NULL;
    }
    return vector;
}

vector_t *vector_crear(void** datos, size_t cantidad) {
    vector_t* vector = malloc(sizeof(vector_t));
    if(!vector){
        return NULL;
    }
    vector->datos = calloc(sizeof(void*), (cantidad > CAPACIDAD_MINIMA ? cantidad : CAPACIDAD_MINIMA));
    if(!vector->datos){
        free(vector);
        return NULL;
    }
    vector->cantidad = cantidad;
    vector->capacidad = cantidad > CAPACIDAD_MINIMA ? cantidad : CAPACIDAD_MINIMA;

    for(int i = 0; i < cantidad; i++){
        vector->datos[i] = datos[i];
    }
    return vector;
}

void vector_destruir(vector_t *vector, void (*destruir_elemento)(void *e)){
    if(destruir_elemento){
        for(int i = 0; i < vector->cantidad; i++){
            destruir_elemento(vector->datos[i]);
        }
    }
    free(vector->datos);
    free(vector);
}

bool vector_redimensionar(vector_t *vector, size_t nueva_capacidad) {
    void** datos_nuevo = realloc(vector->datos, nueva_capacidad * sizeof(void*));
    if (!datos_nuevo) 
        return false;

    vector->datos = datos_nuevo;
    vector->capacidad = nueva_capacidad;
    return true;
}

void* vector_obtener(vector_t *vector, size_t pos){
    return vector->datos[pos];
}

bool vector_guardar(vector_t *vector, void*  valor){
    if(vector->cantidad >= vector->capacidad && !vector_redimensionar(vector, vector->capacidad * FACTOR_REDIMENSION)){
        return false;
    }    
    vector->datos[vector->cantidad] = valor;
    vector->cantidad++;
    return true;
}

void* vector_eliminar(vector_t* vector){
    if(vector->cantidad <= vector->capacidad / (FACTOR_REDIMENSION * 2) && vector->cantidad <= CAPACIDAD_MINIMA){
        vector_redimensionar(vector, vector->capacidad / FACTOR_REDIMENSION);
    }
    void* dato = vector->datos[vector->cantidad - 1];
    vector->datos[vector->cantidad - 1] = NULL;
    vector->cantidad--;
    return dato;

}

void vector_swap(vector_t* vector, size_t index1, size_t index2){
    void* provisorio = vector->datos[index1];
    vector->datos[index1] = vector->datos[index2];
    vector->datos[index2] = provisorio;
}

size_t vector_cantidad(vector_t *vector){
    return vector->cantidad;
}