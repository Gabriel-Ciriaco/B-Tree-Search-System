#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../bTree/bTree.h"


void setNO_ID(int id);

void extrairNomeArquivo(char** nomeArquivo, char* caminho);

BTNo *lerNo(const char *caminhoNo);

void escreverNo(BTNo *noEscrever);

void criarNomeNo(const char* diretorio, BTNo *no);

#endif // FILE_MANAGER_H
