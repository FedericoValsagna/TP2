#ifndef LISTA_PACIENTES_H
#define LISTA_PACIENTES_H
#include <stdbool.h>
#include "zyxcba_helper.h"


typedef struct lista_pacientes lista_pacientes_t;

// Crea un paciente
lista_pacientes_t* lista_pacientes_crear();

// Recibe un paciente con su urgencia y lo guarda dentro de la lista_pacientes, devuelve True
// si lo guardo con exito, sino devuelve False.
bool lista_pacientes_guardar_paciente(lista_pacientes_t* lista_pacientes, paciente_t* paciente, URGENCIA_T urgencia_paciente);

// Devuelve el proximo paciente en la lista de pacientes. Si no hay proximo devuelve NULL.
paciente_t* lista_pacientes_obtener_paciente(lista_pacientes_t* lista_pacientes);

// Devuelve la cantidad de pacientes en la lista de pacientes
size_t lista_pacientes_cantidad(lista_pacientes_t* lista_pacientes);

// Destruye la lista_pacientes
void lista_pacientes_destruir(lista_pacientes_t* lista_pacientes);

#endif  // LISTA_PACIENTES_H
