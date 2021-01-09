#include "cola.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo nodo_t;
struct nodo
{
    void* dato;
    nodo_t* siguiente;
};

static nodo_t* nodo_crear(void* dato, nodo_t* siguiente)
{
    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if(nuevo_nodo == NULL)
        return NULL;
    nuevo_nodo->dato = dato;
    nuevo_nodo->siguiente = siguiente;
    return nuevo_nodo;
}
struct cola
{
    nodo_t* primero;
    nodo_t* ultimo;
};

cola_t* cola_crear()
{
    cola_t* cola = malloc(sizeof(cola_t));
    if (cola == NULL)
        return NULL;
    
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

void cola_destruir(cola_t* cola, void (*destruir_dato)(void *))
{
    if(cola == NULL)    //Verificar que exista la cola
        return;
    
    while(cola->primero != NULL)
    {
        if(destruir_dato != NULL)
            destruir_dato(cola->primero->dato);
        nodo_t* dato_a_eliminar = cola->primero;
        cola->primero = cola->primero->siguiente;
        free(dato_a_eliminar);
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t* cola)
{
    if(cola == NULL)    //Verificar que exista la cola
        return false;
    
    return cola->primero == NULL;
}

void* cola_ver_primero(const cola_t* cola)
{
    if(cola == NULL || cola->primero == NULL)    //Verificar que exista la cola
        return NULL;
    
    return cola->primero->dato;
}

bool cola_encolar(cola_t* cola, void* valor)
{
    if(cola == NULL)    //Verificar que exista la cola
        return false;
    //
    //Crear el nodo nuevo, no va a apuntar a nada
    //
    nodo_t* nuevo_nodo = nodo_crear(valor, NULL);
    if(nuevo_nodo == NULL)
        return false;
    //
    //CASO BORDE COLA VACIA
    //
    if(cola->primero == NULL)
    {
        cola->primero = nuevo_nodo;
    }
    else
        cola->ultimo->siguiente = nuevo_nodo;
    //
    //Enganchar el nodo a la lista, el nuevo nodo pasa a ser el ultimo 
    //
    cola->ultimo = nuevo_nodo;
    return true;
}

void* cola_desencolar(cola_t* cola)
{
    if(cola == NULL || cola->primero == NULL)    //Verificar que exista la cola
        return NULL;
    
    void* dato = cola->primero->dato;
    nodo_t* antiguo_primero = cola->primero;
    cola->primero = cola->primero->siguiente;
    free(antiguo_primero);
    return dato; 
}
