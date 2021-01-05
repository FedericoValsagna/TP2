#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define CAPACIDAD_INICIAL 11
#define ALPHA_MAX 0.7
#define FACTOR_REDIMENSION 2

uint32_t jenkins_one_at_a_time_hash(const char *key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += (uint32_t)key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

enum estados{
    VACIO,
    OCUPADO,
    BORRADO,
};

typedef struct celda{
    char* clave;
    void* dato;
    enum estados estado;
}celda_t;

struct hash{
    celda_t* tabla;
    uint32_t capacidad;
    size_t cantidad_actual;
    size_t cantidad_total;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
    const hash_t* hash;
    size_t actual;
};


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash){ //Caso borde: Falla el malloc del hash
        return NULL;
    }
    celda_t* tabla = malloc(sizeof(celda_t) * CAPACIDAD_INICIAL);
    if(!tabla){//Caso borde: Falla el malloc de la tabla
        free(hash);
        return NULL;
    }

    hash->tabla = tabla;
    hash->capacidad = CAPACIDAD_INICIAL;
    hash->cantidad_actual = 0;
    hash->cantidad_total = 0;
    hash->destruir_dato = destruir_dato;
    //Inicializar los valores de la tabla
    for(int i = 0; (uint32_t)i < hash->capacidad; i++){
        tabla[i].clave = NULL;
        tabla[i].dato = NULL;
        tabla[i].estado = VACIO;
    }
    return hash;
}

bool redimensionar_tabla(hash_t* hash, uint32_t nueva_capacidad){
    //Crear nueva tabla
    celda_t* nueva_tabla = malloc(sizeof(celda_t) * nueva_capacidad);
    if(!nueva_tabla){
        return false;
    }
    //Inicializar los valores de la tabla
    for(int i = 0; (uint32_t)i < nueva_capacidad; i++){
        nueva_tabla[i].clave = NULL;
        nueva_tabla[i].dato = NULL;
        nueva_tabla[i].estado = VACIO;
    }
    //Reempazar las tablas en el hash
    celda_t* tabla_antigua = hash->tabla;
    uint32_t vieja_capacidad = hash->capacidad;
    hash->tabla = nueva_tabla;
    hash->capacidad = nueva_capacidad;
    hash->cantidad_actual = 0;
    hash->cantidad_total = 0;
    //Pasar los valores de la tabla vieja a la nueva
    for(int i = 0; (uint32_t)i < vieja_capacidad; i++){
        if(tabla_antigua[i].estado != OCUPADO){
            continue;
        }
        hash_guardar(hash, tabla_antigua[i].clave, tabla_antigua[i].dato);
        free(tabla_antigua[i].clave);
    }
    free(tabla_antigua);
    return true;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    //Obtener el indice con la función de Hash
    uint32_t index = jenkins_one_at_a_time_hash(clave, strlen(clave)) % hash->capacidad;
    if(hash_pertenece(hash, clave)){
        while(strcmp(hash->tabla[index].clave, clave) != 0){
            index = index != hash->capacidad ? index + 1 : 0;
        }
        if(hash->destruir_dato != NULL){
            hash->destruir_dato(hash->tabla[index].dato);
        }
        hash->tabla[index].dato = dato;

    }else{
        //Recorrer hasta el primer lugar vacio o borrado que encuentre
        while(hash->tabla[index].estado == OCUPADO){
            index = index != hash->capacidad - 1 ? index + 1: 0;
        }
        //Aumentar cantidades del hash, (Si la celda era borrada no se debe aumentar la capacidad total)
        if(hash->tabla[index].estado == VACIO){
            hash->cantidad_total++;
        }
        hash->cantidad_actual++;
        //Insertar el elemento
        hash->tabla[index].estado = OCUPADO;
        hash->tabla[index].clave = strdup(clave);
        hash->tabla[index].dato = dato;
        //Verificar Redimension
        double alpha_actual = (double) hash->cantidad_total / (double) hash->capacidad;
        if(alpha_actual < ALPHA_MAX){
            return true;
        }
        //Redimensionar
        return redimensionar_tabla(hash, hash->capacidad * FACTOR_REDIMENSION);
    }
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    uint32_t numero_hash = jenkins_one_at_a_time_hash(clave, strlen(clave)) % hash->capacidad;
    celda_t* actual = &(hash->tabla[numero_hash]);
    while(actual->estado != VACIO){
        if(actual->estado == OCUPADO && strcmp(actual->clave, clave) == 0){
            actual->estado = BORRADO;
            hash->cantidad_actual--;
            free(actual->clave);
            actual->clave = NULL;
            return actual->dato;
        }
        if(numero_hash == hash->capacidad - 1){
            numero_hash = 0;
        }else{
            numero_hash++;
        }
        actual = &(hash->tabla[numero_hash]);
    }
    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    uint32_t numero_hash = jenkins_one_at_a_time_hash(clave, strlen(clave)) % hash->capacidad;
    celda_t actual = hash->tabla[numero_hash];
    while(actual.estado != VACIO){
        if(actual.estado == OCUPADO && strcmp(actual.clave, clave) == 0){
            return actual.dato;
        }
        if(numero_hash == hash->capacidad - 1){
            numero_hash = 0;
        }else{
            numero_hash++;
        }
        actual = hash->tabla[numero_hash];
    }
    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    uint32_t numero_hash = jenkins_one_at_a_time_hash(clave, strlen(clave)) % hash->capacidad;
    celda_t actual = hash->tabla[numero_hash];
    while(actual.estado != VACIO){
        if(actual.estado == OCUPADO && strcmp(actual.clave, clave) == 0){
            return true;
        }
        if(numero_hash == hash->capacidad - 1){
            numero_hash = 0;
        }else{
            numero_hash++;
        }
        actual = hash->tabla[numero_hash];
    }
    return false;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad_actual;
}

void hash_destruir(hash_t *hash){
    for(int i = 0; (uint32_t)i < hash->capacidad; i++){
        if(hash->tabla[i].estado == OCUPADO){
            free(hash->tabla[i].clave);
            if(hash->destruir_dato){
                hash->destruir_dato(hash->tabla[i].dato);
            }
        }
    }
    free(hash->tabla);
    free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if(iter == NULL){
        return NULL;
    }
    iter->hash = hash;
    iter->actual = 0;
    while(iter->actual < iter->hash->capacidad && iter->hash->tabla[iter->actual].estado != OCUPADO){
        iter->actual++;
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if(hash_iter_al_final(iter)){
        return false;
    }
    iter->actual++;
    while(iter->actual < iter->hash->capacidad && iter->hash->tabla[iter->actual].estado != OCUPADO){
        iter->actual++;
    }
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(hash_iter_al_final(iter)){
        return NULL;
    }
    return iter->hash->tabla[iter->actual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return iter->actual == iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t* iter){
    free(iter);
}
