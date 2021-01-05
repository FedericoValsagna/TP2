#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stdio.h>

///////////////////////////////////////
// Definicion de los tipos de datos //
/////////////////////////////////////

/* La lista esta planteada como una lista de punteros genericos */

struct lista;
typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/////////////////
// Primitivas //
////////////////


//  Crea una lista
//  Post: Devuelve una lista
lista_t *lista_crear(void);

//  Devuelve true si la lista no contiene elementos, false en caso contrario.
//  Pre: La lista se encuentra creada, de no ser el caso devuelve false.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un elemento al inicio de la lista y devuelve true. Devuelve falso en caso de error.
// Pre: La lista fue creada, de no ser el caso devuelve false.
// Post: Se inserta el elemento al inicio de la lista
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un elemento al final de la lista y devuelve true. Devuelve falso en caso de error.
// Pre: La lista fue creada, de no ser el caso devuelve false.
// Post: Se inserta el elemento al final de la lista
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Devuelve el primer elemento de la lista y lo elimina de ella. Devuelve NULL si la lista está vacía.
// Pre: La lista fue creada, de no ser el caso devuelve false.
// Post: Se devuelve el primer elemento de la lista y este es eliminado de la lista.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el primer elemento de la lista sin modificarla, NULL si la lista no contiene elementos
// Pre: La lista fue creada, de no ser el caso devuelve NULL.
// Post: Se devuelve el primer elemento de la lista sin modificarla.
void *lista_ver_primero(const lista_t *lista);

// Devuelve el ultimo elemento de la lista sin modificarla, NULL si la lista no contiene elementos
// Pre: La lista fue creada, de no ser el caso devuelve NULL.
// Post: Se devuelve el ultimo elemento de la lista sin modificarla.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista sin modificarla.
// Pre: La lista fue creada, de no ser el caso devuelve 0.
// Post: Se devuelve la cantidad de elemtos que contiene la lista sin modificarla.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: La lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: Se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Recorre todos los elementos de la lista, aplicando la función que se le pase.
// Recibe un parametro extra que se le pasa a la funcion pasada.
// Pre: La lista fue creada. Visitar es una función que se le aplica a cada dato. Extra es un parametro que recibe la funcion pasada
// Post: Se le aplica la función a todos los elementos de la lista
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);




// Crea un iterador para recorrer la lista
// Pre: La lista fue creada. Devuelve NULL en caso contrario
// Post: Devuelve un iterador de lista
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza al siguiente elemento de la lista, devuelve true si se logro avanzar a otro item, false si llego al final
// Pre: El iterador fue creado, en caso contrario devueve false
// Post: El iterador se encuentra ahora en el siguiente elemento de la lista, devolviendo true si logra avanzar, false en caso de haber llegado al final
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve sin modificar la lista y el iterador el elemento actual por el cual se esta iterando
// Pre: El iterador fue creado, devuelve NULL en caso contrario.
// Post: Devuelve el item actual que se esta iterando sin modificar la lista y el iterador. Devuelve NULL en caso de estar en el final de la lista
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Pre: El iterador fue creado, devuelve false en caso contrario
// Post: Devuelve true si no quedan elementos por recorrer en la lista
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador sin modificar la lista
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un nuevo elemento en la lista atras del elemento actual posicionado, el nuevo actual pasa a ser el nuevo item. Devuelve true, false en caso de error
// Pre: El iterador y la lista fueron creadas, devuelve false en caso contrario
// Post: Existe un nuevo elemento de la lista, previo al actual. El nuevo actual es el nuevo elemento insertado. Devuelve true, false en caso de error
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra un elemento de la lista y lo devuelve, el actual pasa a ser el siguiente de la lista. Si no hay elementos en la lista devuelve NULL.
// Pre: El iterador y la lista fueron creadas, devuelve NULL en caso contrario.
// Post: Se devuelve el elemento eliminado de la lista, el nuevo actual es el siguiente elemento, si era el ultimo el actual ahora es NULL.
void* lista_iter_borrar(lista_iter_t *iter);
#endif

