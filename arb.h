#ifndef ARB_H
    #define ARB_H

    #include "item.h"

    #include <stdbool.h>

    typedef struct arb ARB;

    ARB *arb_criar(void);
    void arb_apagar(ARB **arvore);

    bool arb_inserir(ARB *arvore, ITEM *item);
    ITEM *arb_remover(ARB *arvore, int chave);
    ITEM *arb_buscar(ARB *arvore, int chave);

    void arb_imprimir(ARB *arvore);
#endif 