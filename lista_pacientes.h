#ifndef LISTA_PACIENTES_H
#define LISTA_PACIENTES_H

#include <stdbool.h>

typedef struct lista_pacientes lista_pacientes_t;


lista_pacientes_t* lista_pacientes_crear();


bool lista_pacientes_guardar_paciente(paciente_t* paciente);


paciente_t* lista_pacientes_obtener_paciente();


void lista_pacientes_destruir();


#endif  // LISTA_PACIENTES_H