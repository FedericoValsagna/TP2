#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

#define CAPACIDAD_MINIMA 8
#define FACTOR_REDIMENSION 2
#define DOBLE_FACTOR_REDIMENSION  (FACTOR_REDIMENSION * 2)
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
bool redimensionar(pila_t* pila, size_t cantidad);

pila_t *pila_crear()
{
    pila_t *pila = malloc(CAPACIDAD_MINIMA * sizeof(pila_t*));

    if (pila == NULL)
        return NULL;
    
    void* datos = malloc(CAPACIDAD_MINIMA * sizeof(void*));
    if(datos == NULL)
    {
        free(pila);
        return NULL;
    }
    size_t cantidad = 0;
    size_t capacidad = CAPACIDAD_MINIMA;

    pila->datos = datos;
    pila->cantidad = cantidad;
    pila->capacidad = capacidad;

    return pila;
}

void pila_destruir(pila_t *pila)
{
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila)
{
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t* pila, void* valor)
{
    //
    //Si es necesario expandir la pila
    //
    if(pila->cantidad == pila->capacidad)
    {
        if(!redimensionar(pila, pila->cantidad * FACTOR_REDIMENSION))
            return false;
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;   
    return true;
}

void* pila_ver_tope(const pila_t* pila)
{
    //
    //Si la pila se encuentra vacia
    //
    if(pila->cantidad == 0)
        return NULL;
    
    void* tope = pila->datos[pila->cantidad - 1];
    return tope;
}

void* pila_desapilar(pila_t* pila)
{
    //
    //Si la pila se encuentra vacia
    //
    if(pila->cantidad == 0)
        return NULL;
    
    void* elemento = pila->datos[pila->cantidad - 1];
    pila->cantidad--;

    //
    //Si hay que achicar la pila
    //
    if(pila->cantidad <= pila->capacidad / DOBLE_FACTOR_REDIMENSION && pila->capacidad > CAPACIDAD_MINIMA)
        redimensionar(pila, pila->capacidad / FACTOR_REDIMENSION);
    
    return elemento;
}



bool redimensionar(pila_t* pila, size_t nueva_capacidad)
{
    void** nuevo_arreglo = realloc(pila->datos, sizeof(void*) * nueva_capacidad);
    
    if(nuevo_arreglo == NULL)
        return false;
    
    pila->datos = nuevo_arreglo;
    pila->capacidad = nueva_capacidad;
    return true;
}