#include "jvm.h"
#include <stdio.h>
#include <string.h>

#define ISTORE 54
#define ILOAD  21
#define BIPUSH 16
#define DUP    89
#define IAND   126
#define IXOR   130
#define IOR    128
#define IREM   112
#define INEG   116
#define IDIV   108
#define IADD   96
#define IMUL   104
#define ISUB   100

static const int OK = 0;

void instruccion_istore(jvm_t *la_jvm, int8_t opeando);
void instruccion_iload(jvm_t *la_jvm, int8_t operando);
void instruccion_bipush(jvm_t *la_jvm, int8_t operando);
void instruccion_dup(jvm_t *la_jvm);
void instruccion_iand(jvm_t *la_jvm);
void instruccion_ixor(jvm_t *la_jvm);
void instruccion_ior(jvm_t *la_jvm);
void instruccion_irem(jvm_t *la_jvm);
void instruccion_ineg(jvm_t *la_jvm);
void instruccion_idiv(jvm_t *la_jvm);
void instruccion_iadd(jvm_t *la_jvm);
void instruccion_imul(jvm_t *la_jvm);
void instruccion_isub(jvm_t *la_jvm);



int jvm_crear(jvm_t *la_jvm, int* memoria, size_t tamanio_memoria) {
    pila_crear(&(la_jvm->pila), sizeof(uint32_t));
    la_jvm->memoria = memoria;
    la_jvm->tamanio_memoria = tamanio_memoria;

    for (int i = 0; i < la_jvm->tamanio_memoria; i++) {
        la_jvm->memoria[i] = 0;
    }

    return OK;
}


int jvm_procesar(jvm_t *la_jvm, programa_t *programa) {
    uint8_t instruccion;

    fprintf(stdout, "Bytecode trace\n");

    while (!programa_obtener_instruccion(programa, &instruccion)) {
        switch (instruccion) {
            case ISTORE :
                programa_obtener_instruccion(programa, &instruccion);
                instruccion_istore(la_jvm, instruccion);
                break;
            case ILOAD :
                programa_obtener_instruccion(programa, &instruccion);
                instruccion_iload(la_jvm, instruccion);
                break;
            case BIPUSH :
                programa_obtener_instruccion(programa, &instruccion);
                instruccion_bipush(la_jvm, instruccion);
                break;
            case DUP :
            instruccion_dup(la_jvm);
                break;
            case IAND :
                instruccion_iand(la_jvm);
                break;
            case IXOR :
                instruccion_ixor(la_jvm);
                break;
            case IOR :
                instruccion_ior(la_jvm);
                break;
            case IREM :
                instruccion_irem(la_jvm);
                break;
            case INEG :
                instruccion_ineg(la_jvm);
                break;
            case IDIV :
                instruccion_idiv(la_jvm);
                break;
            case IADD :
                instruccion_iadd(la_jvm);
                break;
            case IMUL :
                instruccion_imul(la_jvm);
                break;
            case ISUB :
                instruccion_isub(la_jvm);
                break;
            default :
                break;
        }
    }

    return OK;
}

int jvm_finalizar(jvm_t *la_jvm) {
    pila_destruir(&(la_jvm->pila));
    return OK;
}


void instruccion_istore(jvm_t *la_jvm, int8_t opeando) {
    fprintf(stdout, "istore\n");
    int32_t elemento;

    pila_sacar(&(la_jvm->pila), &elemento);
    (la_jvm->memoria)[opeando] = elemento;
}

void instruccion_iload(jvm_t *la_jvm, int8_t operando) {
    fprintf(stdout, "iload\n");
    int32_t elemento;

    elemento = (la_jvm->memoria)[operando];
    pila_agregar(&(la_jvm->pila), &elemento);
}

void instruccion_bipush(jvm_t *la_jvm, int8_t operando) {
    int32_t dato = operando;
    fprintf(stdout, "bipush\n");
    pila_agregar(&(la_jvm->pila), &dato);
}

void instruccion_dup(jvm_t *la_jvm) {
    fprintf(stdout, "dup\n");
    int32_t elemento;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_agregar(&(la_jvm->pila), &elemento);
    pila_agregar(&(la_jvm->pila), &elemento);
}

void instruccion_iand(jvm_t *la_jvm) {
    fprintf(stdout, "iand\n");
    int32_t elemento, elemento2, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_sacar(&(la_jvm->pila), &elemento2);
    operacion_binaria = elemento & elemento2;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_ixor(jvm_t *la_jvm) {
    fprintf(stdout, "ixor\n");
    int32_t elemento, elemento2, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_sacar(&(la_jvm->pila), &elemento2);
    operacion_binaria = elemento ^ elemento2;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_ior(jvm_t *la_jvm) {
    fprintf(stdout, "ior\n");
    int32_t elemento, elemento2, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_sacar(&(la_jvm->pila), &elemento2);
    operacion_binaria = elemento | elemento2;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_irem(jvm_t *la_jvm) {
    fprintf(stdout, "irem\n");
    int32_t elemento, elemento2, resto;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    resto = elemento % elemento2;
    pila_agregar(&(la_jvm->pila), &resto);
}

void instruccion_ineg(jvm_t *la_jvm) {
    fprintf(stdout, "ineg\n");
    int32_t elemento, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    operacion_binaria = ~elemento;
    operacion_binaria++;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_idiv(jvm_t *la_jvm) {
    fprintf(stdout, "idiv\n");
    int32_t elemento, elemento2, division;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    division = elemento / elemento2;
    pila_agregar(&(la_jvm->pila), &division);
}

void instruccion_iadd(jvm_t *la_jvm) {
    fprintf(stdout, "iadd\n");

    int32_t elemento, elemento2, suma;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    suma = elemento + elemento2;
    pila_agregar(&(la_jvm->pila), &suma);
}

void instruccion_imul(jvm_t *la_jvm) {
    fprintf(stdout, "imul\n");
    int32_t elemento, elemento2, multiplicacion;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    multiplicacion = elemento * elemento2;
    pila_agregar(&(la_jvm->pila), &multiplicacion);
}

void instruccion_isub(jvm_t *la_jvm) {
    fprintf(stdout, "isub\n");
    int32_t elemento, elemento2, resta;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    resta = elemento - elemento2;
    pila_agregar(&(la_jvm->pila), &resta);
}

