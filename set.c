#include "set.h"
#include "item.h"
#include "avl.h"
#include "arb.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ED_AVL 0 //quando usar avl/rubro-negra

struct _set {
    int tipo;
    void *ed;
};

SET *set_criar(int tipo) {
    SET *set = (SET*) malloc(sizeof(SET));

    if (set != NULL) {
        set->tipo = tipo;

        if (tipo == ED_AVL) {
            set->ed = avl_criar(); 
        } else {
            set->ed = arb_criar();
        }
    }
    return(set);
}

void set_apagar(SET **set) {
    if (*set == NULL) {
        return;
    }

    if ((*set)->tipo == ED_AVL) {
        avl_apagar((AVL**) &(*set)->ed);
    } else {
        arb_apagar((ARB**) &(*set)->ed);
    }

    free(*set);
    *set = NULL;
} 

bool set_pertence(SET *set, int chave) {
    if (set == NULL) {
        return(false);
    }

    if (set->tipo = ED_AVL) {
        return(avl_buscar((AVL*) set->ed, chave) != NULL);
    } else {
        return(arb_buscar((ARB*) set->ed, chave) != NULL);
    }

}

bool set_inserir(SET *set, ITEM *item) {
    if (set == NULL) {
        return(false);
    }

    if (set->tipo == ED_AVL) {
        return(avl_inserir((AVL*) set->ed, item));
    } else {
        return(arb_inserir((ARB*) set->ed, item));
    }
}

ITEM *set_remover(SET *set, int chave) {
    if (set == NULL) {
        return(false);
    }

    if (set->tipo == ED_AVL) {
        return(avl_remover((AVL*) set->ed, chave));
    } else {
        return(arb_remover((ARB*) set->ed, chave));
    }
}

SET *set_uniao(SET *a, SET *b) {
    if (a == NULL || b == NULL) {
        return(NULL);
    }
}


SET *set_interseccao(SET *a, SET *b) {
    if (a == NULL || b == NULL) {
        return(NULL);
    }
}

void set_imprimir(SET *set) {
    if (set == NULL) {
        return;
    }

    if (set->tipo == ED_AVL) {
        avl_imprimir((AVL*) set->ed);
    } else {
        arb_imprimir((ARB*) set->ed);
    }
}