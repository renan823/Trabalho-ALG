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

    ARB *arb_unir(ARB *a1, ARB *a2);
    ARB *arb_intersectar(ARB *a1, ARB *a2);

    void arb_imprimir(ARB *arvore);
#endif 