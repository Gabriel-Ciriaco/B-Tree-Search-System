#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>

#include "../bTree/bTree.h"


typedef struct
{
    BT *arvore;
}Sistema;

void mensagemBoasVindas();

int rodarMenu();

bool lerConfiguracoes(Sistema* sistema);

void escreverConfiguracoes(Sistema sistema);

void executarSistema();

#endif // SYSTEM_H
