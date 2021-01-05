#include <stdlib.h>
#include <string.h>
#include "lista_pacientes.h"
#include "cola.h"
#include "hash.h"
#include "heap.h"

struct lista_pacientes{
    cola_t* emergencias;
    heap_t* normales;
    size_t cantidad;
};

lista_pacientes_t* lista_pacientes_crear(){
    lista_pacientes_t* lista_pacientes = malloc(sizeof(lista_pacientes_t));
    if(!lista_pacientes){
        return NULL;
    }
    lista_pacientes->emergencias = cola_crear();
    if(!lista_pacientes->emergencias){
        free(lista_pacientes);
        return NULL;
    }
    lista_pacientes->normales = heap_crear(strcmp);
    if(!lista_pacientes->normales){
        cola_destruir(lista_pacientes->emergencias, NULL);
        free(lista_pacientes);
        return NULL;
    }
    lista_pacientes->cantidad = 0;
    return lista_pacientes;
}

bool lista_pacientes_guardar_paciente(lista_pacientes_t* lista_pacientes, paciente_t* paciente, URGENCIA_T urgencia_paciente){
    if(urgencia_paciente == URGENTE){
        return cola_encolar(lista_pacientes->emergencias, paciente);
    }
    return heap_encolar(lista_pacientes->normales, paciente);
}

paciente_t* lista_pacientes_obtener_paciente(lista_pacientes_t* lista_pacientes){
    if(!cola_esta_vacia(lista_pacientes->emergencias)){
        return cola_desencolar(lista_pacientes->emergencias);
    }
    return heap_desencolar(lista_pacientes->normales);
}

void lista_pacientes_destruir(lista_pacientes_t* lista_pacientes){
    cola_destruir(lista_pacientes->emergencias, NULL);
    heap_destruir(lista_pacientes->normales, NULL);
    free(lista_pacientes);
}