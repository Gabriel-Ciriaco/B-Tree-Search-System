#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bTree.h"


BTNo *CriarNo(int t1, bool folha1)
{
    BTNo *novoNo = (BTNo*) malloc(sizeof(BTNo));

    if(novoNo == NULL)
    {
        return NULL;
    }

    // Cria espaço para 2t - 1 caminhos de arquivos.
    novoNo->chaves = (char**) malloc((2*t1-1) * sizeof(char*));

    if(novoNo->chaves == NULL)
    {
        free(novoNo);
        return NULL;
    }

    for(int i = 0; i < 2*t1-1; i++)
    {
        novoNo->chaves[i] = NULL;
    }

    /*
        Alocação dos filhos. No caso, temos 1 filho a mais
        do que o número de chaves, por isso não é 2t - 1.
    */
    novoNo->filho = (char**) malloc((2*t1) * sizeof(char*));

    if(novoNo->filho == NULL)
    {
        free(novoNo->chaves);
        free(novoNo);

        return NULL;
    }

    for(int i=0; i<2*t1; i++)
    {
        novoNo->filho[i] = NULL;
    }


    novoNo->nomeArquivo=NULL;

    novoNo->n = 0;
    novoNo->t = t1;
    novoNo->ehFolha = folha1;


    return novoNo;
}

BT *CriarBT(int t1)
{
    BT *arvore = (BT*) malloc(sizeof(BT));

    if(arvore == NULL)
    {
        return NULL;
    }

    arvore->diretorio=NULL;

    arvore->raiz = NULL;

    arvore->t = t1;

    return arvore;
}


void copiarChave(BTNo * no, int indice, char * caminho)
{
    if (!no)
    {
        printf("\n[COPIAR-CHAVE]: O nó especificado não foi encontrado.\n");

        return;
    }

    // Realocamos corretamente o tamanho do caminho, caso necessário.
    if(no->chaves[indice])
        free(no->chaves[indice]);

    if (caminho)
    {
        // Alocamos um espaço para o caminho.
        no->chaves[indice] = (char*) malloc(sizeof(char) * strlen(caminho));

        strcpy(no->chaves[indice], caminho);
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
        no->filho[indice] = (char*) malloc(sizeof(char) * strlen(filho));

        strcpy(no->filho[indice], filho);
    }
    else
    {
        no->filho[indice] = NULL;
    }
}

bool freeNo(BTNo * no)
{
    if (!no) return false;

    if (no->n > 0)
    {
        for (int i = 0; i < no->n; i++)
        {
            free(no->chaves[i]);
        }

        free(no->chaves);
    }

    if (!no->ehFolha)
    {
        for (int i = 0; i <= no->n; i++)
        {
            free(no->filho[i]);
        }

        free(no->filho);
    }

    if (no->nomeArquivo)
        free(no->nomeArquivo);

    free(no);

    return true;
}
