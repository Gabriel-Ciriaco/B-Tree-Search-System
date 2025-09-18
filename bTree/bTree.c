#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bTree.h"
#include "../file_manager/file_manager.h"


BTNo *CriarNo(const char* diretorio, int t1, bool folha1)
{
    BTNo *novoNo = (BTNo*) malloc(sizeof(BTNo));

    if (!diretorio)
    {
        return NULL;
    }

    if(novoNo == NULL)
    {
        return NULL;
    }

    // Cria espaço para 2t - 1 nomes de arquivos.
    novoNo->chaves = (char**) malloc((2*t1-1) * sizeof(char*));

    if(novoNo->chaves == NULL)
    {
        free(novoNo);
        return NULL;
    }

    novoNo->caminhosArquivos = (char**) malloc((2*t1-1) * sizeof(char*));

    if (novoNo->caminhosArquivos == NULL)
    {
        free(novoNo->chaves);
        free(novoNo->caminhosArquivos);
        return NULL;
    }

    for(int i = 0; i < 2*t1-1; i++)
    {
        novoNo->chaves[i] = NULL;
        novoNo->caminhosArquivos[i] = NULL;
    }

    /*
        Alocação dos filhos. No caso, temos 1 filho a mais
        do que o número de chaves, por isso não é 2t - 1.
    */
    novoNo->filho = (char**) malloc((2*t1) * sizeof(char*));

    if(novoNo->filho == NULL)
    {
        free(novoNo->chaves);
        free(novoNo->caminhosArquivos);
        free(novoNo);

        return NULL;
    }

    for(int i=0; i<2*t1; i++)
    {
        novoNo->filho[i] = NULL;
    }

    novoNo->n = 0;
    novoNo->t = t1;
    novoNo->ehFolha = folha1;

    novoNo->caminhoNo = NULL;

    criarNomeNo(diretorio, novoNo);

    return novoNo;
}

BT *CriarBT(const char* diretorio, int t1)
{
    BT *arvore = (BT*) malloc(sizeof(BT));

    if(arvore == NULL)
    {
        return NULL;
    }

    arvore->diretorio = (char*) malloc(sizeof(char) * sizeof(strlen(diretorio) + 1));

    if (arvore->diretorio)
    {
        strcpy(arvore->diretorio, diretorio);
        arvore->diretorio[strlen(diretorio)] = '\0';
    }
    else
    {
        arvore->diretorio = NULL;
    }

    arvore->raiz = NULL;

    arvore->t = t1;

    return arvore;
}


void copiarChave(BTNo * no, int indice, char * nomeArquivo)
{
    if (!no)
    {
        printf("\n[COPIAR-CHAVE]: O nó especificado não foi encontrado.\n");

        return;
    }

    // Realocamos corretamente o tamanho do nomeArquivo, caso necessário.
    if(no->chaves[indice])
        free(no->chaves[indice]);

    if (nomeArquivo)
    {
        // Alocamos um espaço para o nomeArquivo.
        no->chaves[indice] = (char*) malloc(sizeof(char) * (strlen(nomeArquivo) + 1));

        if (!no->chaves[indice])
        {
            printf("\n[COPIAR-CHAVE]: Não foi possível alocar espaço para o nó desejado\n");
            return;
        }

        strcpy(no->chaves[indice], nomeArquivo);
    }
    else
    {
        no->chaves[indice] = NULL;
    }
}

void copiarFilho(BTNo * no, int indice, char * filho)
{
    if (!no)
    {
        printf("\n[COPIAR-FILHO]: O nó especificado não foi encontrado.\n");

        return;
    }

    // Realocamos corretamente o tamanho do filho, caso necessário.
    if(no->filho[indice])
        free(no->filho[indice]);

    if (filho)
    {
        // Alocamos um espaço para o filho.
        no->filho[indice] = (char*) malloc(sizeof(char) * (strlen(filho) + 1));

        if (!no->filho[indice])
        {
            printf("\n[COPIAR-FILHO]: Não foi possível alocar espaço para o nó desejado\n");
            return;
        }

        strcpy(no->filho[indice], filho);
    }
    else
    {
        no->filho[indice] = NULL;
    }
}

void copiarCaminho(BTNo * no, int indice, char * caminho)
{
    if (!no)
    {
        printf("\n[COPIAR-CAMINHO]: O nó especificado não foi encontrado.\n");

        return;
    }

    // Realocamos corretamente o tamanho do caminho, caso necessário.
    if(no->caminhosArquivos[indice])
        free(no->caminhosArquivos[indice]);

    if (caminho)
    {
        // Alocamos um espaço para o nomeArquivo.
        no->caminhosArquivos[indice] = (char*) malloc(sizeof(char) * (strlen(caminho) + 1));

        if (!no->caminhosArquivos[indice])
        {
            printf("\n[COPIAR-CAMINHO]: Não foi possível alocar espaço para o nó desejado\n");
            return;
        }

        strcpy(no->caminhosArquivos[indice], caminho);
    }
    else
    {
        no->caminhosArquivos[indice] = NULL;
    }
}

void imprimeArvore(BT *arvore)
{
    if (arvore == NULL || arvore->raiz == NULL) return;

    imprimeEmOrdem(arvore->raiz);
}

void imprimeEmOrdem(BTNo *no)
{
    if (!no) return;

    int i;
    for (i = 0; i < no->n; i++)
    {
        // Primeiro imprime a subárvore à esquerda da chave i
        if (!no->ehFolha && no->filho[i] != NULL)
        {
            BTNo* filhoNo = lerNo(no->filho[i]);

            imprimeEmOrdem(filhoNo);

            freeNo(filhoNo);
        }

        // Agora imprime a chave atual
        if (no->chaves[i])
            printf("Arquivo: %s\n", no->chaves[i]);
    }

    // Depois da última chave, imprime a subárvore à direita
    if (!no->ehFolha && no->filho[i])
    {
        BTNo* filhoNo = lerNo(no->filho[i]);

        imprimeEmOrdem(filhoNo);

        freeNo(filhoNo);
    }
}

bool freeNo(BTNo * no)
{
    if (!no) return false;

    if (no->n > 0)
    {
        for (int i = 0; i < no->n; i++)
        {
            if(no->chaves[i])
                free(no->chaves[i]);

            if(no->caminhosArquivos[i])
                free(no->caminhosArquivos[i]);
        }

        if(no->chaves)
            free(no->chaves);

        if(no->caminhosArquivos)
            free(no->caminhosArquivos);
    }

    if (!no->ehFolha)
    {
        for (int i = 0; i <= no->n; i++)
        {
            if(no->filho[i])
                free(no->filho[i]);
        }

        if(no->filho)
            free(no->filho);
    }

    if(no->caminhoNo)
        free(no->caminhoNo);

    free(no);

    return true;
}

bool freeBTree(BT * arvore)
{
    if (!arvore) return false;

    if(arvore->raiz)
        freeNo(arvore->raiz);

    if(arvore->diretorio)
        free(arvore->diretorio);

    return true;
}
