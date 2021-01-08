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

int paciente_cmp(const void *paciente1, const void *paciente2){
    paciente_t* p1 = (paciente_t*) paciente1;
    paciente_t* p2 = (paciente_t*) paciente2;
    if (p1->anio_inscripcion < p2->anio_inscripcion){
        return -1;
    }
    if (p1->anio_inscripcion > p2->anio_inscripcion){
        return 1;
    }
    return 0;
}

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
    lista_pacientes->normales = heap_crear(paciente_cmp);
    if(!lista_pacientes->normales){
        cola_destruir(lista_pacientes->emergencias, NULL);
        free(lista_pacientes);
        return NULL;
    }
    lista_pacientes->cantidad = 0;
    return lista_pacientes;
}

bool lista_pacientes_guardar_paciente(lista_pacientes_t* lista_pacientes, paciente_t* paciente, URGENCIA_T urgencia_paciente){
    if(urgencia_paciente == URGENTE && !cola_encolar(lista_pacientes->emergencias, paciente)){
        return false;
    }
    if(urgencia_paciente == REGULAR && !heap_encolar(lista_pacientes->normales, paciente)){
        return false;
    }
    lista_pacientes->cantidad++;
    return true;
}

paciente_t* lista_pacientes_obtener_paciente(lista_pacientes_t* lista_pacientes){
    if(lista_pacientes->cantidad == 0){
        return NULL;
    }
    lista_pacientes->cantidad--;
    if(!cola_esta_vacia(lista_pacientes->emergencias)){
        return cola_desencolar(lista_pacientes->emergencias);
    }
    return heap_desencolar(lista_pacientes->normales);
}

size_t lista_pacientes_cantidad(lista_pacientes_t* lista_pacientes){
    return lista_pacientes->cantidad;
}

void lista_pacientes_destruir(lista_pacientes_t* lista_pacientes){
    cola_destruir(lista_pacientes->emergencias, free);
    heap_destruir(lista_pacientes->normales, free); // Agregue los free para que no se pierda memoria al dejar pacientes sin liberar
    free(lista_pacientes);
}