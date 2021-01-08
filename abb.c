#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include "abb.h"
#include <string.h>
#include "pila.h"

typedef struct nodo nodo_t;

struct nodo{
    nodo_t* izq;
    nodo_t* der;
    char* clave;
    void* dato;
};
struct abb{
    nodo_t* raiz;
    abb_comparar_clave_t comparar;
    abb_destruir_dato_t destruir;
    size_t cantidad;
};
struct abb_iter{
    const abb_t* abb;
    pila_t* stack;
    nodo_t* act;
};


nodo_t* nodo_crear(const char *clave, void *dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL){
        return false;
    }
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->clave = strdup(clave);
    nodo->dato = dato;
    return nodo;
}

void* nodo_destruir(nodo_t* act){
    void* dato = act->dato;
    free(act->clave);
    free(act);
    return dato;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb){ //Caso borde: Falla el malloc
        return NULL;
    }
    abb->raiz = NULL;
    abb->comparar = cmp;
    abb->destruir = destruir_dato;
    abb->cantidad = 0;
    return abb;
}

bool _abb_guardar(abb_t *arbol, nodo_t *act, const char *clave, void *dato){
    if(arbol->comparar(act->clave, clave) == 0){
        if(arbol->destruir != NULL){
            arbol->destruir(act->dato);
        }
        act->dato = dato;
        return true;
    }
    if(arbol->comparar(act->clave, clave) > 0){
        if(!act->izq){
            nodo_t* nodo = nodo_crear(clave, dato);
            if(!nodo){
                return false;
            }
            act->izq = nodo;
            arbol->cantidad++;
            return true;
        }
        return _abb_guardar(arbol, act->izq, clave, dato);
    }
    if(!act->der){
        nodo_t* nodo = nodo_crear(clave, dato);
        if(!nodo){
            return false;
        }
        act->der = nodo;
        arbol->cantidad++;
        return true;
    }
    return _abb_guardar(arbol, act->der, clave, dato);
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol->raiz){
        nodo_t* nodo = nodo_crear(clave, dato);
        if(!nodo){
            return false;
        }
        arbol->raiz = nodo;
        arbol->cantidad++;
        return true;
    }
    return _abb_guardar(arbol, arbol->raiz, clave, dato);
}

nodo_t* reemplazante(nodo_t* actual, int(*comparar)(const char*, const char*),const char* clave){
    if(!actual || !actual->izq){
        return NULL;
    }
    actual = actual->izq;
    while(actual->der){
        actual = actual->der;
    }
    return actual;
}

void* _abb_borrar(abb_t* arbol, nodo_t* padre, nodo_t* hijo, const char* clave, bool hijo_es_izquierdo){
    if(!arbol || !hijo){
        return NULL;
    }
    if(arbol->comparar(hijo->clave, clave) > 0){
        return _abb_borrar(arbol, hijo, hijo->izq, clave, true);
    }
    if(arbol->comparar(hijo->clave, clave) < 0){
        return _abb_borrar(arbol, hijo, hijo->der, clave, false);
    }
    //Caso 2 hijos
    if(hijo->izq && hijo->der){
        nodo_t* reemplazo = reemplazante(hijo, arbol->comparar, clave);
        char* reemplazo_clave = strdup(reemplazo->clave);
        void* reemplazo_dato = reemplazo->dato;
        void* dato = hijo->dato;
        _abb_borrar(arbol, NULL, arbol->raiz, reemplazo->clave, false);
        free(hijo->clave);
        hijo->clave = reemplazo_clave;
        hijo->dato = reemplazo_dato;
        return dato;
    }
    //
    //CASO BORDE: Eliminar la raiz
    //
    if(!padre){
        //Caso sin hijos
        if(!hijo->izq && !hijo->der){
            arbol->raiz = NULL;
        }
        //Caso 1 hijo (Derecho)
        if(!hijo->izq && hijo->der){
            arbol->raiz = hijo->der;
        }
        //Caso 1 hijo (Izquierdo)
        if(!hijo->der && hijo->izq){
            arbol->raiz = hijo->izq;
        }
        arbol->cantidad--;
        return nodo_destruir(hijo);
    }
    //
    //
    //
    nodo_t** puntero_a_hijo = hijo_es_izquierdo ? &padre->izq : &padre->der;
    //Caso sin hijos
    if(!hijo->izq && !hijo->der){
        *puntero_a_hijo = NULL;
    }
    //Caso 1 hijo (Derecho)
    if(!hijo->izq && hijo->der){
        *puntero_a_hijo = hijo->der;
    }
    //Caso 1 hijo (Izquierdo)
    if(!hijo->der && hijo->izq){
        *puntero_a_hijo = hijo->izq;
    }
    arbol->cantidad--;
    return nodo_destruir(hijo);
}
void* abb_borrar(abb_t *arbol, const char *clave){
    if(!arbol){
        return NULL;
    }
    return _abb_borrar(arbol, NULL, arbol->raiz, clave, false);
}

void* _abb_obtener(nodo_t* padre, int(*comparar)(const char*, const char*), const char* clave){
    if(!padre){
        return NULL;
    }
    if(comparar(padre->clave, clave) < 0){
        return _abb_obtener(padre->der, comparar, clave);
    }
    if(comparar(padre->clave, clave) > 0){
        return _abb_obtener(padre->izq, comparar, clave);
    }
    return padre->dato;
}
void *abb_obtener(const abb_t *arbol, const char *clave){
    if(!arbol){
        return NULL;
    }
    return _abb_obtener(arbol->raiz, arbol->comparar, clave);
}

bool _abb_pertenece(nodo_t* padre, int(*comparar)(const char*, const char*), const char* clave){
    if(!padre){
        return NULL;
    }
    if(comparar(padre->clave, clave) < 0){
        return _abb_pertenece(padre->der, comparar, clave);
    }
    if(comparar(padre->clave, clave) > 0){
        return _abb_pertenece(padre->izq, comparar, clave);
    }
    return true;
}
bool abb_pertenece(const abb_t *arbol, const char *clave){
    if(!arbol){
        return NULL;
    }
    return _abb_pertenece(arbol->raiz, arbol->comparar, clave);
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cantidad;
}

void _abb_destruir(nodo_t *act, abb_destruir_dato_t destruir){
    if(!act){
        return;
    }
    _abb_destruir(act->izq, destruir);
    _abb_destruir(act->der, destruir);
    void* dato = nodo_destruir(act);
    if(destruir != NULL){
        destruir(dato);
    }
}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->destruir);
    free(arbol);
}

bool _abb_in_order(nodo_t*actual, bool visitar(const char*, void*, void*), void* extra){
    if(!actual){
        return true;
    }
    bool estado =_abb_in_order(actual->izq, visitar, extra);
    if(estado){
        if(!visitar(actual->clave,actual->dato, extra)){
            return false;
        }
        return _abb_in_order(actual->der, visitar, extra);
    }
    return false;
}
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    if(!arbol){
        return;
    }
    _abb_in_order(arbol->raiz, visitar, extra);
}

nodo_t* obtener_inicio(abb_t* abb, nodo_t* actual, char* inicio){
    //
    //Creo que esto debería funcionar, pero no tengo ni la menor idea
    //
    if(!actual){
        return NULL;
    }
    int comparacion = abb->comparar(actual->clave, inicio);
    if(comparacion < 0){
        nodo_t* resultado = obtener_inicio(abb,actual->der,inicio);
        return resultado == NULL ?  actual :  resultado;
    }
    if(comparacion > 0){
        return obtener_inicio(abb, actual->izq, inicio);
    }
    return actual;

}
abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter){
        return NULL;
    }
    pila_t* pila = pila_crear();
    if(!pila){
        free(iter);
        return NULL;
    }
    iter->stack = pila;
    iter->abb = arbol;
    iter->act = arbol->raiz;
    if(!iter->act){
        return iter;
    }
    while(iter->act->izq){
        pila_apilar(pila, iter->act);
        iter->act = iter->act->izq;
    }
    return iter;
}

abb_iter_t* abb_iter_in_crear_desde(const abb_t* arbol, char* inicio){
    //Hay que crear esta primitiva, que devuelva un abb
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter){
        return NULL;
    }
    pila_t* pila = pila_crear();

    if(!pila){
        free(iter);
        return NULL;
    }
    iter->stack = pila;
    iter->abb = arbol;
    iter->act = obtener_inicio(arbol, arbol->raiz, inicio);
    if(!iter->act){
        return iter;
    }
    while(iter->act->izq){
        pila_apilar(pila, iter->act);
        iter->act = iter->act->izq;
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)){
        return false;
    }
    if(!iter->act->der){
        nodo_t* act = pila_desapilar(iter->stack);
        iter->act = act;
        return true;
    }
    iter->act = iter->act->der;
    while(iter->act->izq){
        pila_apilar(iter->stack, iter->act);
        iter->act = iter->act->izq;
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(!iter->act){
        return NULL;
    }
    return iter->act->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->stack) && iter->act == NULL;
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->stack);
    free(iter);
}
