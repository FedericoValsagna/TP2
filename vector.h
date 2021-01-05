#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct vector vector_t;


/*******************************************************************
 *                    PRIMITIVAS DEL VECTOR
 ******************************************************************/


vector_t *vector_crear(void** datos, size_t cantidad);

vector_t* vector_crear_vacio();

void vector_destruir(vector_t* vector, void (*destruir_elemento)(void *e));

/* Devuelve el elemento en el indice pasado por parametro */
void* vector_obtener(vector_t* vector, size_t pos);

/* Se guarda al FINAL lugar del vector el elemento pasado por parametro, devuelve true/false si hubo o no errores a la hora de guardar */
bool vector_guardar(vector_t* vector, void* valor);
/* Se elimina el ULTIMO elemento del vector */
void* vector_eliminar(vector_t* vector);
/* Se swappean los elementos de los indices pasados por parametro */
void vector_swap(vector_t* vector, size_t index1, size_t index2);
/* Devuelve la cantidad de elementos que hay en el vector */
size_t vector_cantidad(vector_t* vector);

#endif
