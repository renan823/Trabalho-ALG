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

    SET *set2 = set_criar(ed);
    set_inserir(set2, item_criar(7, NULL));
    set_inserir(set2, item_criar(5, NULL));
    set_inserir(set2, item_criar(6, NULL));
    set_inserir(set2, item_criar(8, NULL));

    SET *set3 = set_uniao(set, set2);

    set_imprimir(set3);

    set_apagar(&set);

    return(0);
}