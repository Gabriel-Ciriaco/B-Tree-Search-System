#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdbool.h>


typedef struct bTNo
{
    bool ehFolha;

    int n; // N�mero de chaves atuais no n�.
    int t; // Grau m�nimo do n�.

    char* caminhoNo; // Caminho do n�.

    char **chaves; // As chaves s�o os nomes dos arquivos.
    char **filho; // Cada filho � um arquivo contendo um n�.

    char** caminhosArquivos; // Os caminhos completos dos nomes dos arquivos.

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
