#ifndef ZYXCBA_HELP
#define ZYXCBA_HELP
#include <stdlib.h>
#include "mensajes.h"
typedef struct paciente{
    char* nombre;
    size_t anio_inscripcion;
}paciente_t;

void* constructor_paciente(char** linea, void* extra);
void destruir_paciente(void* p);


typedef struct doctor{
    char* nombre;
    char* especialidad;
    size_t pacientes_atendidos;
}doctor_t;

void* constructor_doctor(char** linea, void* extra);
void destruir_doctor(void* d);


typedef enum URGENCIA{
    URGENTE,
    REGULAR
}URGENCIA_T;

#endif