#include "set.h"
#include <stdio.h>

int main(void) {
    int ed; // 0 para avl, 1 para rubro-negro
    scanf("%d", &ed);

    SET *set = set_criar(ed);

    set_inserir(set, 1);
    set_inserir(set, 3);
    set_inserir(set, 4);
    set_inserir(set, 2);

    SET *set2 = set_criar(ed);
    set_inserir(set2, 1);
    set_inserir(set2, 5);
    set_inserir(set2, 2);
    set_inserir(set2, 8);

    SET *set3 = set_interseccao(set, set2);

    set_remover(set3, 1);

    set_imprimir(set3);

    set_apagar(&set);

    return(0);
}