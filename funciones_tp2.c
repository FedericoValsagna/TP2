#include "funciones_tp2.h"
#include "mensajes.h"
#include "lista_pacientes.h"
#include <stdlib.h>
#include <stdio.h>

bool Pedir_Turno(paciente_t* paciente, char* especialidad, URGENCIA_T urgencia, hash_t* lista_pacientes_totales){
    lista_pacientes_t* lista_pacientes = hash_obtener(lista_pacientes_totales, especialidad);
    if(!lista_pacientes){
        printf(ENOENT_ESPECIALIDAD, especialidad);
        return false;
    }
    if(!lista_pacientes_guardar_paciente(lista_pacientes, paciente, urgencia)){
        //Falta agregar mensaje de error por si no se pudo guardar el paciente a la lista
    }
    fprintf(stdout, PACIENTE_ENCOLADO, paciente->nombre);
    fprintf(stdout, CANT_PACIENTES_ENCOLADOS, lista_pacientes_cantidad(lista_pacientes), especialidad);
    return true;
}

bool atender_siguiente_paciente(doctor_t* doctor, hash_t* lista_pacientes_totales){
    lista_pacientes_t* lista_pacientes = hash_obtener(lista_pacientes_totales, doctor->especialidad);
    if(!lista_pacientes){
        printf(ENOENT_ESPECIALIDAD, doctor->especialidad);
    }
}

bool informe_doctores(char letra_inicio, char letra_final){

}