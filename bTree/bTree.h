#ifndef BTREE_H
#define BTREE_H

#include<stdlib.h>
#include<stdbool.h>


typedef struct bTNo
{
    bool ehFolha;

    int n; // N�mero de chaves atuais no n�.
    int t; // Grau m�nimo do n�.

    char *nomeArquivo; // Nome do pr�prio arquivo.

    char **chaves; // As chaves s�o os caminhos dos arquivos.
    char **filho; // Cada filho � um arquivo contendo um n�.

}BTNo;

typedef struct
{
    int t; // Grau m�nimo da �rvore.

    BTNo *raiz; // Diret�rio raiz.

    char *diretorio; // Caminho da pasta, por exemplo, "C://".

}BT;


typedef struct
{
    BTNo *no;   // N� onde a chave est�.
    int pos;    // Posi��o da chave dentro do N�.

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
