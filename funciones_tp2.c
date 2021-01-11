#include "funciones_tp2.h"
#include "mensajes.h"
#include "lista_pacientes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool pedir_turno(paciente_t* paciente, char* especialidad, URGENCIA_T urgencia, hash_t* lista_pacientes_totales){
    lista_pacientes_t* lista_pacientes = hash_obtener(lista_pacientes_totales, especialidad);
    if(!lista_pacientes_guardar_paciente(lista_pacientes, paciente, urgencia)){
        //Falta agregar mensaje de error por si no se pudo guardar el paciente a la lista
        return false;
    }
    printf(PACIENTE_ENCOLADO, paciente->nombre);
    printf(CANT_PACIENTES_ENCOLADOS, lista_pacientes_cantidad(lista_pacientes), especialidad);
    return true;
}

void atender_siguiente_paciente(doctor_t* doctor, hash_t* lista_pacientes_totales){
    lista_pacientes_t* lista_pacientes = hash_obtener(lista_pacientes_totales, doctor->especialidad);
    if(!lista_pacientes){
        printf(SIN_PACIENTES);
        return;
    }
    paciente_t* paciente = lista_pacientes_obtener_paciente(lista_pacientes);
    if(!paciente){
        printf(SIN_PACIENTES);
        return;
    }
    doctor->pacientes_atendidos++;
    printf(PACIENTE_ATENDIDO, paciente->nombre);
    printf(CANT_PACIENTES_ENCOLADOS, lista_pacientes_cantidad(lista_pacientes), doctor->especialidad);
}

void informe_doctores(char* letra_inicio, char* letra_final, abb_t* doctores){
    abb_iter_t* iterador = strcmp(letra_inicio, "") == 0 ? abb_iter_in_crear(doctores) : abb_iter_in_crear_personalizado(doctores, letra_inicio);
    size_t contador = 0;
    while(!abb_iter_in_al_final(iterador) && strcmp(abb_iter_in_ver_actual(iterador), letra_final) < 0){
        contador++; 
        abb_iter_in_avanzar(iterador);
    }   
    printf(DOCTORES_SISTEMA, contador);
    abb_iter_in_destruir(iterador);
    if(contador == 0){
        return;
    }
    contador = 0;
    iterador = abb_iter_in_crear_personalizado(doctores, letra_inicio);
    while(!abb_iter_in_al_final(iterador) && strcmp(abb_iter_in_ver_actual(iterador), letra_final) < 0){
        doctor_t* doctor_actual = abb_obtener(doctores, abb_iter_in_ver_actual(iterador));
        contador++;
        printf(INFORME_DOCTOR, contador, doctor_actual->nombre, doctor_actual->especialidad, doctor_actual->pacientes_atendidos);
        abb_iter_in_avanzar(iterador);
    }
    abb_iter_in_destruir(iterador);
}