#ifndef BTREE_H
#define BTREE_H

#include<stdlib.h>
#include<stdbool.h>


typedef struct bTNo
{
    bool ehFolha;

    int n; // Número de chaves atuais no nó.
    int t; // Grau mínimo do nó.

    char *nomeArquivo; // Nome do próprio arquivo.

    char **chaves; // As chaves são os caminhos dos arquivos.
    char **filho; // Cada filho é um arquivo contendo um nó.

}BTNo;

typedef struct
{
    int t; // Grau mínimo da árvore.

    BTNo *raiz; // Diretório raiz.

    char *diretorio; // Caminho da pasta, por exemplo, "C://".

}BT;


typedef struct
{
    BTNo *no;   // Nó onde a chave está.
    int pos;    // Posição da chave dentro do Nó.

} ResultadoBusca;


BTNo *CriarNo(int t1, bool folha1);

BT *CriarBT(int t1);

int posElemento(BTNo *no, char *procurado);

BTNo *BTbusca(BT *arvore, char *procurado);

void copiarChave(BTNo * no, int indice, char * caminho);

void copiarFilho(BTNo * no, int indice, char * filho);

void splitChild(BTNo *pai, int indice);

void insercaoNaoCheio(BTNo *no, char *caminho);

void insercao(BT *arvore, char *caminho);

bool freeNo(BTNo* no);

bool freeBTree(BT * arvore);

#endif // BTREE_H
