#ifndef FUNCIONES_TP2_H
#define FUNCIONES_TP2_H

#include <stdbool.h>
#include "zyxcba_helper.h"
#include "hash.h"
#include "abb.h"


// Se recibe un nombre de paciente y el nombre de una especialidad,
// y el sistema le añade a la lista de espera de la especialidad correspondiente.
bool pedir_turno(paciente_t* paciente, char* especialidad, URGENCIA_T urgencia, hash_t* lista_pacientes_totales);

// Se recibe el nombre de le doctore que quedó libre,
// y este atiende al siguiente paciente urgente (por orden de llegada).
// Si no hubiera ningún paciente urgente,
// atiende al siguiente paciente con mayor antigüedad como paciente en la clínica."
void atender_siguiente_paciente(doctor_t* doctor, hash_t* lista_pacientes_totales);


// El sistema imprime la lista de doctores en orden alfabético,
// junto con su especialidad y el número de pacientes que atendieron desde que arrancó el sistema.
// Opcionalmente, se puede especificar el rango (alfabético) de doctores sobre los que se desean informes."
void informe_doctores(char letra_inicio, char letra_final, abb_t* doctores);


#endif  // FUNCIONES_TP2_H
