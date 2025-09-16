#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../bTree/bTree.h"


void setNO_ID(char* nomeArquivo);

BTNo *lerNo(const char *nomeArquivo);

void escreverNo(BTNo *noEscrever);

void criarNomeNo(BTNo *no);

#endif // FILE_MANAGER_H
