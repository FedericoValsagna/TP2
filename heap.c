#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "heap.h"
#define SIZE_MINIMO 8
#define FACTOR_REDIMENSION 2

#define TOPE 0
#define TOPE_HIJO_IZQ 1
#define TOPE_HIJO_DER 2

typedef int (*cmp_func_t) (const void *a, const void *b);

void downheap(heap_t* heap, size_t padre, size_t hijo_izq, size_t hijo_der);
void upheap(heap_t* heap, size_t padre, size_t hijo);

typedef struct heap{
    vector_t* arreglo;
    cmp_func_t comparar;
}heap_t;

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap){
        return NULL;
    }
    heap->arreglo = vector_crear_vacio();
    if(!heap->arreglo){
        free(heap);
        return NULL;
    }
    heap->comparar = cmp;
    return heap;
}

void heapify(heap_t* heap){
    int mitad = (int)vector_cantidad(heap->arreglo) / 2;
    for(int i = mitad; i > -1; i--){
        downheap(heap, i, 2 * i + 1, 2 * i + 2);
    }
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    if(!arreglo){
        return NULL;
    }
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap){
        return NULL;
    }
    heap->arreglo = vector_crear(arreglo, n);
    if(!heap->arreglo){
        free(heap);
        return NULL;
    }
    heap->comparar = cmp;
    heapify(heap);
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    vector_destruir(heap->arreglo, destruir_elemento);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return vector_cantidad(heap->arreglo);
}

bool heap_esta_vacio(const heap_t *heap){
    return vector_cantidad(heap->arreglo) == 0;
}

/* Se guarda al final del array se hace upheap */
bool heap_encolar(heap_t *heap, void *elem){
    if(!vector_guardar(heap->arreglo, elem)){
        return false;
    }
    size_t pos_hijo = vector_cantidad(heap->arreglo) - 1;
    upheap(heap, (pos_hijo - 1) / 2, pos_hijo);
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap))
        return NULL;
    return vector_obtener(heap->arreglo, 0);
}

/* Lo primero que hace es swappear con el ultimo elemento ya que el vector elimina el ultimo, se hace downheap de la nueva raiz despues*/
void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)){
        return NULL;
    }
    vector_swap(heap->arreglo ,0 , vector_cantidad(heap->arreglo) - 1);
    void* dato = vector_eliminar(heap->arreglo);
    downheap(heap, TOPE, TOPE_HIJO_IZQ, TOPE_HIJO_DER);
    return dato;
}

void upheap(heap_t* heap, size_t padre, size_t hijo){
    if(hijo == 0){
        return;
    }
    if(heap->comparar(vector_obtener(heap->arreglo, padre), vector_obtener(heap->arreglo, hijo)) >= 0){
        return;
    }
    vector_swap(heap->arreglo, padre, hijo);
    size_t nuevo_padre = (padre - 1) / 2;
    upheap(heap, nuevo_padre, padre);
}

void downheap(heap_t* heap, size_t padre, size_t hijo_izq, size_t hijo_der){
    if(hijo_izq >= vector_cantidad(heap->arreglo)){
        return;
    }
    //Si el hijo derecho no existe en el arreglo toma el valor del hijo izquierdo (Probablemente ahorre casos borde esto)
    hijo_der = hijo_der >= vector_cantidad(heap->arreglo) ? hijo_izq : hijo_der; 
    size_t maximo = heap->comparar(vector_obtener(heap->arreglo, hijo_izq), vector_obtener(heap->arreglo, hijo_der)) >= 0 ? hijo_izq : hijo_der;
    if(heap->comparar(vector_obtener(heap->arreglo, padre), vector_obtener(heap->arreglo, maximo)) > 0){
        return;
    }
    vector_swap(heap->arreglo, padre, maximo);
    size_t nuevo_hijo_izq = maximo * 2 + 1;
    size_t nuevo_hijo_der = maximo * 2 + 2;
    downheap(heap, maximo, nuevo_hijo_izq, nuevo_hijo_der);
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heap_t* heap = heap_crear_arr(elementos, cant, cmp);
    int ultimo_relativo = (int)cant - 1;
    while(ultimo_relativo > -1){
        vector_swap(heap->arreglo, 0, ultimo_relativo);
        elementos[ultimo_relativo] = vector_eliminar(heap->arreglo);
        ultimo_relativo--;
        downheap(heap, TOPE, TOPE_HIJO_IZQ, TOPE_HIJO_DER);
    }
    heap_destruir(heap, NULL);
}