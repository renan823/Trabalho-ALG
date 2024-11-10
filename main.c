#include "item.h"
#include "set.h"
#include <stdio.h>

int main(void) {
    int ed;
    scanf("%d", &ed);

    SET *set = set_criar(ed);
    set_imprimir(set);

    set_apagar(&set);

    return(0);
}