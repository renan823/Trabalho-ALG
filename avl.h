#ifndef AVL_H
    #define AVL_H

    #include "item.h"

    #include <stdbool.h>

    typedef struct avl AVL;

    AVL *avl_criar(void);
    void avl_apagar(AVL **arvore);

    bool avl_inserir(AVL *arvore, ITEM *item);
    ITEM *avl_remover(AVL *arvore, int chave);
    ITEM *avl_buscar(AVL *arvore, int chave);

    void avl_imprimir(AVL *arvore);
#endif 