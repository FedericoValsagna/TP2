#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "zyxcba_helper.h"
#include "lista_pacientes.h"
#include "lista.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

#define URGENCIA1 "URGENTE"
#define URGENCIA2 "REGULAR"

void procesar_comando(const char* comando, const char** parametros, hash_t* pacientes, abb_t* doctores, hash_t* turnos_pacientes) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if(strlen(parametros) < 3){
			printf(ENOENT_PARAMS, comando);
			return;
		}

		paciente_t* paciente = hash_obtener(pacientes, parametros[0]);
		if(!paciente){
			printf(ENOENT_PACIENTE, parametros[0]);
			return;
		}

		char* especialidad = parametros[1];
		if(!hash_pertenece(turnos_pacientes, especialidad)){
			printf(ENOENT_ESPECIALIDAD, especialidad);
			return;
		}

		URGENCIA_T urgencia;
		if(strcmp(parametros[2], URGENCIA1) == 0){
			urgencia = URGENTE;
		} else if(strcmp(parametros[2], URGENCIA2) == 0){
			urgencia = REGULAR;
		} else{
			printf(ENOENT_URGENCIA, parametros[2]);
			return;
		}

		pedir_turno(paciente, especialidad, urgencia, turnos_pacientes);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if(strlen(parametros) < 1){
			printf(ENOENT_PARAMS, comando);
			return;
		}
		
		doctor_t* doctor = abb_obtener(doctores, parametros[0]);
		if(!doctor){
			printf(ENOENT_DOCTOR, parametros[0]);
			return;
		}

		atender_siguiente_paciente(doctor, turnos_pacientes);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if(strlen(parametros) < 2){
			printf(ENOENT_PARAMS, comando);
			return;
		}
		informe_doctores(parametros[0], parametros[1], doctores);

	} else {
		printf(ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(hash_t* pacientes, abb_t* doctores, hash_t* turnos_pacientes) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, pacientes, doctores, turnos_pacientes);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

void* constructor_paciente(char** linea, void* extra){
	paciente_t* paciente = malloc(sizeof(paciente_t));
	if(!paciente){
		return NULL;
	}
	paciente->nombre = strdup(linea[0]);
	if(!isdigit_strutil(linea[1])){ // IMPLEMENTAR ESTO EN STRUTIL
		printf(ENOENT_ANIO, linea[1]);
		return NULL;
	}
	paciente->anio_inscripcion = (size_t)atoi(linea[1]);
	return paciente;
}

// Estas 2 funciones tal vez quedarian mejor como primitivas de doctor_crear o paciente_crear

void* constructor_doctor(char** linea, void* extra){
	doctor_t* doctor = malloc(sizeof(doctor_t));
	if(!doctor){
		return NULL;
	}
	char* especialidad = linea[1];
	if(!hash_pertenece(extra, especialidad)){
		lista_pacientes_t* lista_pacientes = lista_pacientes_crear();
		if(!lista_pacientes){
			free(doctor);
			return NULL;
		}
		hash_guardar(extra, especialidad, lista_pacientes);
	}

	doctor->nombre = strdup(linea[0]);
	doctor->especialidad = strdup(especialidad);
	doctor->pacientes_atendidos = 0;

	return doctor;
}

bool llenar_doctores(lista_t* doctores_lista, abb_t* doctores_abb){
	lista_iter_t* doctores_iter = lista_iter_crear(doctores_lista);
	if(!doctores_iter){
		return false;
	}
	while(!lista_iter_al_final(doctores_iter)){
		doctor_t* actual = lista_iter_ver_actual(doctores_iter);
		if (!abb_guardar(doctores_abb, actual->nombre, actual)){
			lista_iter_destruir(doctores_iter);
			return false;
		}
		lista_iter_avanzar(doctores_lista);
	}
	lista_iter_destruir(doctores_iter);
	return true;
}

bool llenar_pacientes(lista_t* pacientes_lista, hash_t* pacientes_hash){
	lista_iter_t* pacientes_iter = lista_iter_crear(pacientes_lista);
	if(!pacientes_iter){
		return false;
	}
	while(!lista_iter_al_final(pacientes_iter)){
		paciente_t* actual = lista_iter_ver_actual(pacientes_iter);
		if (!hash_guardar(pacientes_hash, actual->nombre, actual)){
			lista_iter_destruir(pacientes_iter);
			return false;
		}
		lista_iter_avanzar(pacientes_lista);
	}
	lista_iter_destruir(pacientes_iter);
	return true;
}

void destruir_estructuras(hash_t* pacientes, abb_t* doctores, hash_t* turnos_pacientes){
	hash_destruir(pacientes);
	abb_destruir(doctores);
	hash_destruir(turnos_pacientes);
}

int main(int argc, char** argv) {
	if(argc < 3){
		return 1;
	}
	// argv[1] CSV Doctores
	// argv[2] CSV Pacientes

	// FALTA ERROR DE NO PODER LEER ARCHIVOS CSV

	abb_t* doctores = abb_crear(strcmp, free);
	if(!doctores){
		return 1;
	}

	hash_t* pacientes = hash_crear(free);
	if(!pacientes){
		abb_destruir(doctores);
		return 1;
	}
	hash_t* turnos_pacientes = hash_crear(free);
	if(!turnos_pacientes){
		abb_destruir(doctores);
		hash_destruir(pacientes);
		return 1;
	}

	lista_t* doctores_lista = csv_crear_estructura(argv[1], constructor_doctor, turnos_pacientes);
	if(!doctores_lista){ // Este checkeo no se si sirve, revisar como funca csv cuando devolves NULL en el constructor.
		destruir_estructuras(pacientes, doctores, turnos_pacientes);
		return 1;
	}
	lista_t* pacientes_lista = csv_crear_estructura(argv[2], constructor_paciente, &extra);
	if(!pacientes_lista){
		destruir_estructuras(pacientes, doctores, turnos_pacientes);
		return 1;
	}

	if(!llenar_doctores(doctores_lista, doctores)){
		destruir_estructuras(pacientes, doctores, turnos_pacientes);
		return 1;
	}

	if(!llenar_pacientes(pacientes_lista, pacientes)){
		destruir_estructuras(pacientes, doctores, turnos_pacientes);
		return 1;
	}
	lista_destruir(doctores_lista, free); 
	// Creo que no alcanza con esto por que los nombres de cada doctor y paciente
	// estan hechos con strdup, tal vez se pierda memoria aca para tener en cuenta en el fiuter
	lista_destruir(pacientes_lista, free);

	procesar_entrada(pacientes, doctores, turnos_pacientes);
	destruir_estructuras(pacientes, doctores, turnos_pacientes);
	return 0;
}
