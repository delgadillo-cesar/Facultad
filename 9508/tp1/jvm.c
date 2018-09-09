#include "jvm.h"
#include <stdio.h>
#include <string.h>

#define OK 0
#define ERROR 1

#define ISTORE 54
#define ILOAD  31
#define BIPUSH 16
#define DUP    89
#define IAND   126
#define IXOR   130
#define IOR    128
#define IREM   112
#define INEG   116
#define IDIV   109
#define IADD   96
#define IMUL   104
#define ISUB   100

void instruccion_istore(jvm_t *la_jvm, char opeando);
void instruccion_iload(jvm_t *la_jvm, char operando);
void instruccion_bipush(jvm_t *la_jvm, char operando);
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
    pila_crear(&(la_jvm->pila), sizeof(char));
//    la_jvm->memoria = malloc(tamanio_memoria * sizeof(int));
    la_jvm->memoria = memoria;
    la_jvm->tamanio_memoria = tamanio_memoria;

    for (int i = 0; i < la_jvm->tamanio_memoria; i++) {
        la_jvm->memoria[i] = 0;
    }

    return OK;
}


int jvm_procesar(jvm_t *la_jvm, programa_t *programa) {
    char instruccion;

    fprintf(stdout, "Bytecode trace\n");

    while (!programa_obtener_instruccion(programa, &instruccion)) {
        switch((int)instruccion){
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

//    variables_dump(la_jvm);

    return OK;
}

int jvm_finalizar(jvm_t *la_jvm) {
    pila_destruir(&(la_jvm->pila));
//    free(la_jvm->memoria);
    return OK;
}


void instruccion_istore(jvm_t *la_jvm, char opeando) {
    fprintf(stdout, "istore\n");
    char elemento;

    pila_sacar(&(la_jvm->pila), &elemento);
    (la_jvm->memoria)[(int)opeando] = (int) elemento;
}

void instruccion_iload(jvm_t *la_jvm, char operando) {
    fprintf(stdout, "iload\n");
    int elemento;

    elemento = (int) ((la_jvm->memoria)[(int)operando]);
    pila_sacar(&(la_jvm->pila), &elemento);
}

void instruccion_bipush(jvm_t *la_jvm, char operando) {
    fprintf(stdout, "bipush\n");
    pila_agregar(&(la_jvm->pila), &operando);
}

void instruccion_dup(jvm_t *la_jvm) {
    fprintf(stdout, "dup\n");
    char elemento;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_agregar(&(la_jvm->pila), &elemento);
    pila_agregar(&(la_jvm->pila), &elemento);
}

void instruccion_iand(jvm_t *la_jvm) {
    fprintf(stdout, "iand\n");
    char elemento, elemento2, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_sacar(&(la_jvm->pila), &elemento2);
    operacion_binaria = elemento & elemento2;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_ixor(jvm_t *la_jvm) {
    fprintf(stdout, "ixor\n");
    char elemento, elemento2, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_sacar(&(la_jvm->pila), &elemento2);
    operacion_binaria = elemento ^ elemento2;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_ior(jvm_t *la_jvm) {
    fprintf(stdout, "ior\n");
    char elemento, elemento2, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    pila_sacar(&(la_jvm->pila), &elemento2);
    operacion_binaria = elemento | elemento2;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_irem(jvm_t *la_jvm) {
    fprintf(stdout, "irem\n");
    char elemento, elemento2;
    int resto;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    resto = (int)elemento % (int)elemento2;
    elemento = (char) resto;
    pila_agregar(&(la_jvm->pila), &elemento);
}

void instruccion_ineg(jvm_t *la_jvm) {
    fprintf(stdout, "ineg\n");
    char elemento, operacion_binaria;

    pila_sacar(&(la_jvm->pila), &elemento);
    operacion_binaria = ~elemento;

    pila_agregar(&(la_jvm->pila), &operacion_binaria);
}

void instruccion_idiv(jvm_t *la_jvm) {
    fprintf(stdout, "idiv\n");
    char elemento, elemento2;
    int division;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    division = (int)elemento / (int)elemento2;
    elemento = (char) division;
    pila_agregar(&(la_jvm->pila), &elemento);
}

void instruccion_iadd(jvm_t *la_jvm) {
    fprintf(stdout, "iadd\n");

    char elemento, elemento2;
    int suma;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    suma = (int)elemento + (int)elemento2;
    elemento = (char) suma;
    pila_agregar(&(la_jvm->pila), &elemento);
}

void instruccion_imul(jvm_t *la_jvm) {
    fprintf(stdout, "imul\n");
    char elemento, elemento2;
    int multiplicacion;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    multiplicacion = (int)elemento * (int)elemento2;
    elemento = (char) multiplicacion;
    pila_agregar(&(la_jvm->pila), &elemento);
}

void instruccion_isub(jvm_t *la_jvm) {
    fprintf(stdout, "isub\n");
    char elemento, elemento2;
    int resta;

    pila_sacar(&(la_jvm->pila), &elemento2);
    pila_sacar(&(la_jvm->pila), &elemento);
    resta = (int)elemento - (int)elemento2;
    elemento = (char) resta;
    pila_agregar(&(la_jvm->pila), &elemento);
}

