#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include "../bTree/bTree.h"

typedef struct
{

    BT *arvore;

}Sistema;

void mensagemBoasVindas();

int rodarMenu();

void executarSistema();

#endif // SYSTEM_H_INCLUDED
