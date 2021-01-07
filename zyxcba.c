#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "zyxcba_helper.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void procesar_comando(const char* comando, const char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

	} else {

	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada() {
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
		procesar_comando(campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

void* constructor_paciente(char** linea, void* extra){
	paciente_t* paciente = malloc(sizeof(paciente_t));
	paciente->nombre = strdup(linea[0]);
	paciente->anio_inscripcion = (size_t)atoi(linea[1]);
	return paciente;
}

// Estas 2 funciones tal vez quedarian mejor como primitivas de doctor_crear o paciente_crear

void* constructor_doctor(char** linea, void* extra){
	doctor_t* doctor = malloc(sizeof(doctor_t));
	doctor->nombre = strdup(linea[0]);
	doctor->especialidad = strdup(linea[1]);
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

void destruir_estructuras(hash_t* hash, abb_t* abb){
	hash_destruir(hash);
	abb_destruir(abb);
}

int main(int argc, char** argv) {
	if(argc < 3){
		return 1;
	}
	// argv[1] CSV Doctores
	// argv[2] CSV Pacientes

	abb_t* doctores = abb_crear(strcmp, free);
	if(!doctores){
		return 1;
	}
	hash_t* pacientes = hash_crear(free);
	if(!pacientes){
		abb_destruir(doctores);
		return 1;
	}
	int extra = 0; // Esto todavia no se para que usarlo
	lista_t* doctores_lista = csv_crear_estructura(argv[1], constructor_doctor, &extra);
	if(!doctores_lista){
		destruir_estructuras(pacientes, doctores);
		return 1;
	}
	lista_t* pacientes_lista = csv_crear_estructura(argv[2], constructor_paciente, &extra);
	if(!pacientes_lista){
		destruir_estructuras(pacientes, doctores);
		return 1;
	}

	if(!llenar_doctores(doctores_lista, doctores)){
		destruir_estructuras(pacientes, doctores);
		return 1;
	}

	if(!llenar_pacientes(pacientes_lista, pacientes)){
		destruir_estructuras(pacientes, doctores);
		return 1;
	}

	procesar_entrada();
	hash_destruir(pacientes);
	abb_destruir(doctores);
	return 0;
}
