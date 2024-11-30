#include "item.h"
#include "set.h"
#include <stdio.h>

int main(void) {
    int ed; // 0 para avl, 1 para rubro-negro
    scanf("%d", &ed);

    SET *set = set_criar(ed);

    set_inserir(set, item_criar(1, NULL));
    set_inserir(set, item_criar(3, NULL));
    set_inserir(set, item_criar(4, NULL));
    set_inserir(set, item_criar(2, NULL));

    set_imprimir(set);

    set_apagar(&set);

    return(0);
}