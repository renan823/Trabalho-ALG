#ifndef SET_H
    #define SET_H

    #include "item.h"
    #include "avl.h"
    #include "arb.h"

    #include <stdbool.h>

    typedef struct _set SET;

    SET *set_criar(int tipo);
    void set_apagar(SET **set);

    bool set_pertence(SET *set, int chave);
    bool set_inserir(SET *set, ITEM *item);
    bool set_remover(SET *set, int chave);

    SET *set_uniao(SET *a, SET *b);
    SET *set_interseccao(SET *a, SET *b);

    void set_imprimir(SET *set);
#endif