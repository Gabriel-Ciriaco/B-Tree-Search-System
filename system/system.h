#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>

#include "../bTree/bTree.h"

#define ARVORE_PATH "arquivos_arvore"
#define CFG_PATH "cfg.dat"


typedef struct
{
    BT *arvore;
}Sistema;

void mensagemBoasVindas();

int rodarMenu();

bool lerConfiguracoes(Sistema* sistema);

void escreverConfiguracoes(Sistema sistema);

void executarSistema();

void destruirArvoreSistema();

#endif // SYSTEM_H
