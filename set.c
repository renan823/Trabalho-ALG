#include "set.h"
#include "item.h"
#include "ab.h"
#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ED_LISTA 0 //quando usar lista/arvore

struct _set {
    int tipo;
    void *ed;
};

SET *set_criar(int tipo) {
    SET *set = (SET*) malloc(sizeof(SET));

    if (set != NULL) {
        set->tipo = tipo;

        if (tipo == ED_LISTA) {
            set->ed = lista_criar(false); //lista não ordenada
        } else {
            set->ed = ab_criar();
        }
    }
    return(set);
}

void set_apagar(SET **set) {
    if (*set == NULL) {
        return;
    }

    if ((*set)->tipo == ED_LISTA) {
        lista_apagar((LISTA**) &(*set)->ed);
    } else {
        ab_apagar((AB**) &(*set)->ed);
    }

    free(*set);
    *set = NULL;
} 

bool set_pertence(SET *set, ITEM *item);
bool set_inserir(SET *set, ITEM *item);
bool set_remover(SET *set, int chave);

SET *set_uniao(SET *a, SET *b);
SET *set_interseccao(SET *a, SET *b);

void set_imprimir(SET *set) {
    if (set == NULL) {
        return;
    }

    if (set->tipo == ED_LISTA) {
        lista_imprimir((LISTA*) set->ed);
    } else {
        ab_imprimir((AB*) set->ed);
    }
}