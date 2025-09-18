#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdbool.h>


typedef struct bTNo
{
    bool ehFolha;

    int n; // Número de chaves atuais no nó.
    int t; // Grau mínimo do nó.

    char* caminhoNo; // Caminho do nó.

    char **chaves; // As chaves são os nomes dos arquivos.
    char **filho; // Cada filho é um arquivo contendo um nó.

    char** caminhosArquivos; // Os caminhos completos dos nomes dos arquivos.

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


BTNo *CriarNo(const char* diretorio, int t1, bool folha1);

BT *CriarBT(const char* diretorio, int t1);


int posElemento(BTNo *no, char *procurado);

BTNo *BTbusca(BT *arvore, char *procurado);


void copiarChave(BTNo * no, int indice, char * nomeArquivo);

void copiarCaminho(BTNo * no, int indice, char * caminho);

void copiarFilho(BTNo * no, int indice, char * filho);


void splitChild(BTNo *pai, int indice, const char* diretorio);

void insercaoNaoCheio(BTNo *no, const char* diretorio, char* caminho);

void insercao(BT *arvore, char *caminho);


void remocao(BT* arvore, char* nomeArquivo);

void removerArvoreB(BTNo* raiz, char *k);

bool removerFolhaArvoreB(BTNo *pai, char *k);

bool removerNaoFolhaArvoreB(BTNo *pai, char *k);

BTNo* getPredecessor(BTNo *pai, int i);

BTNo* getSucessor(BTNo *pai, int i);

void emprestarDireita(BTNo *pai, int i);

void emprestarEsquerda(BTNo *pai, int i);

void fillNos(BTNo * pai, int i);

void mergeNos(BTNo *pai, int i);


void imprimeArvore(BT *arvore);
void imprimeEmOrdem(BTNo *no);

bool freeNo(BTNo* no);

bool freeBTree(BT * arvore);

#endif // BTREE_H
