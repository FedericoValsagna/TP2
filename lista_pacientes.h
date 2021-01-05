#ifndef LISTA_PACIENTES_H
#define LISTA_PACIENTES_H
#include <stdbool.h>
#include "zyxcba_helper.h"


typedef struct lista_pacientes lista_pacientes_t;


lista_pacientes_t* lista_pacientes_crear();


bool lista_pacientes_guardar_paciente(lista_pacientes_t* lista_pacientes, paciente_t* paciente, URGENCIA_T urgencia_paciente);


paciente_t* lista_pacientes_obtener_paciente(lista_pacientes_t* lista_pacientes);


void lista_pacientes_destruir();


#endif  // LISTA_PACIENTES_H