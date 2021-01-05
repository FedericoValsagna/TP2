#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

  ////////////////
 //    Nodo    //
////////////////
typedef struct nodo nodo_t;
struct nodo
{
    void* dato;
    nodo_t* siguiente;
};

nodo_t* nodo_crear(void* dato, nodo_t* siguiente)
{
    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if(nuevo_nodo == NULL)
        return NULL;
    nuevo_nodo->dato = dato;
    nuevo_nodo->siguiente = siguiente;
    return nuevo_nodo;
}

  ////////////////
 //   Lista    //
////////////////


struct lista
{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t cantidad;
};

lista_t* lista_crear()
{
    lista_t* lista = malloc(sizeof(lista_t));
    if(lista == NULL)
        return NULL;
    
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->cantidad = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t* lista)
{
    if(lista == NULL)
        return NULL;
    
    return lista->cantidad == 0;
}

bool lista_insertar_primero(lista_t* lista, void* dato)
{
    if(lista == NULL)
        return false;
    //
    //Crear el nodo
    //
    nodo_t* nuevo_nodo = nodo_crear(dato, lista->primero);
    if(nuevo_nodo == NULL)
        return NULL;
    lista->primero = nuevo_nodo;
    lista->cantidad++;
    //  
    //Si la lista se encuentra vacia se debe establecer el nodo como el ultimo
    //
    if(lista->cantidad == 1)
        lista->ultimo = nuevo_nodo;

    return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato)
{
    if(lista == NULL)
        return false;
    //
    //Crear el nodo
    //
    nodo_t* nuevo_nodo = nodo_crear(dato, NULL);
    if(nuevo_nodo == NULL)
        return NULL;
    lista->cantidad++;
    //
    //CASO BORDE: Lista vacia. Se guarda como el primero y ultimo nodo en vez
    //
    if(lista->cantidad == 1)
        lista->primero = nuevo_nodo;
    else
        lista->ultimo->siguiente = nuevo_nodo;
    lista->ultimo = nuevo_nodo;
    return true;
}

void* lista_borrar_primero(lista_t* lista)
{
    if(lista == NULL || lista->primero == NULL)
        return NULL;
    
    lista->cantidad--;
    void* dato = lista->primero->dato;
    nodo_t* siguiente = lista->primero->siguiente;
    free(lista->primero);
    lista->primero = siguiente;

    if(lista->cantidad == 0)
        lista->ultimo = NULL;
    
    return dato;
}

void* lista_ver_primero(const lista_t* lista)
{
    if(lista == NULL || lista->primero == NULL)
        return NULL;
    
    return lista->primero->dato;
}

void* lista_ver_ultimo(const lista_t* lista)
{
    if(lista == NULL || lista->ultimo == NULL)
        return NULL;
    
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t* lista)
{
    if(lista == NULL)
        return 0;
    
    return lista->cantidad;
}

void lista_destruir(lista_t* lista, void(*destruir_dato)(void*))
{
    if(lista == NULL)
        return;
    
    nodo_t* actual = lista->primero;

    while(actual != NULL)
    {
        nodo_t* siguiente = actual->siguiente;
        if(destruir_dato != NULL)
            destruir_dato(actual->dato);
        free(actual);
        actual = siguiente;
    }
    free(lista);
    return;
}

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra)
{
    if(lista == NULL || visitar == NULL)
        return;
    
    nodo_t* actual = lista->primero;
    while(actual != NULL && visitar(actual->dato, extra))
        actual = actual->siguiente;
    return;
}

  ///////////////////////////
 //   Iterador Externo    //
///////////////////////////
struct lista_iter
{
    lista_t* lista;
    nodo_t* actual;
    nodo_t* anterior;
};

lista_iter_t* lista_iter_crear(lista_t* lista)
{
    if(lista == NULL)
        return NULL;
    lista_iter_t* lista_iter = malloc(sizeof(lista_iter_t));
    if(lista_iter == NULL)
        return NULL;
    
    
    lista_iter->lista = lista;
    lista_iter->actual = lista->primero;
    lista_iter->anterior = NULL;
    return lista_iter;
}

bool lista_iter_avanzar(lista_iter_t* iter)
{
    if(iter == NULL || iter->actual == NULL)
        return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->siguiente;
    return true;
}

void* lista_iter_ver_actual(const lista_iter_t* iter)
{
    if(iter == NULL || iter->actual == NULL)
        return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t* iter)
{
    if(iter == NULL)
        return false;
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t* iter)
{
    if(iter == NULL)
        return;
    
    free(iter);
    return;
}

bool lista_iter_insertar(lista_iter_t* iter, void* dato)
{
    if(iter == NULL || iter->lista == NULL)
        return false;
    
    //
    //CASO BORDE: Insertar al inicio
    //
    if(iter->anterior== NULL)
    {
        if(!lista_insertar_primero(iter->lista, dato))
            return false;

        iter->actual = iter->lista->primero;
        return true;
    }
    //
    //CASO BORDE: Insertar al final
    //
    if(iter->actual == NULL)
    {
        if(!lista_insertar_ultimo(iter->lista, dato))
            return false;

        iter->actual = iter->lista->ultimo;
        return true;
    }
    
    nodo_t* nuevo_nodo = nodo_crear(dato, iter->actual);
    if(nuevo_nodo == NULL)
        return false;
    iter->anterior->siguiente = nuevo_nodo;
    iter->lista->cantidad++;
    iter->actual = nuevo_nodo;
    return true;
}

void* lista_iter_borrar(lista_iter_t* iter)
{
    if(iter == NULL || iter->lista == NULL || iter->actual == NULL)
        return NULL;
    //
    //CASO BORDE: Borrar el primero elemento
    //
    if(iter->anterior == NULL)
    {
        void* dato = lista_borrar_primero(iter->lista);
        iter->actual = iter->lista->primero;
        iter->anterior = NULL;
        return dato;       
    }
    //
    //
    //
    void* dato = iter->actual->dato;
    iter->anterior->siguiente = iter->actual->siguiente;
    free(iter->actual);
    iter->actual = iter->anterior->siguiente;
    iter->lista->cantidad--;
    //
    //CASO BORDE: Borrar el ultimo elemento
    //
    if(iter->actual == NULL)
        iter->lista->ultimo = iter->anterior;
    return dato;
}