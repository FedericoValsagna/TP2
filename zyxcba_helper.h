#ifndef ZYXCBA_HELP
#define ZYXCBA_HELP
#include <stdlib.h>
typedef struct paciente{
    char* nombre;
    size_t anio_inscripcion;
}paciente_t;

typedef struct doctor{
    char* nombre;
    char* especialidad;
    size_t pacientes_atendidos;
}doctor_t;

typedef enum URGENCIA{
    URGENTE,
    REGULAR
}URGENCIA_T;

#endif