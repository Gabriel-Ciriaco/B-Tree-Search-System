#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bTree.h"
#include "../file_manager/file_manager.h"


void splitChild(BTNo *pai, int indice, const char* diretorio)
{
    BTNo *crianca = lerNo(pai->filho[indice]);
    BTNo *irmao = CriarNo(diretorio, crianca->t, crianca->ehFolha);

    irmao->n = crianca->t - 1;

    for(int i = 0; i < crianca->t-1; i++)
    {
        copiarChave(irmao, i, crianca->chaves[crianca->t + i]);
        copiarCaminho(irmao, i, crianca->caminhosArquivos[crianca->t + i]);
    }

    if(!crianca->ehFolha)
    {
        for(int i=0; i < crianca->t; i++)
        {
            copiarFilho(irmao, i, crianca->filho[crianca->t + i]);
        }
    }

    crianca->n = crianca->t - 1;

    for(int i = pai->n; i >= indice + 1; i--)
    {
        copiarChave(pai, i + 1, pai->chaves[i]);
        copiarCaminho(pai, i + 1, pai->caminhosArquivos[i]);
    }

    if(!pai->ehFolha)
    {
        for(int i = pai->n; i > indice; i--)
        {
            copiarFilho(pai, i + 1, pai->filho[i]);
        }
    }

    copiarChave(pai, indice, crianca->chaves[crianca->t - 1]);
    copiarCaminho(pai, indice, crianca->caminhosArquivos[crianca->t - 1]);

    copiarFilho(pai, indice + 1, irmao->caminhoNo);

    pai->n++;

    escreverNo(crianca);
    escreverNo(irmao);
    escreverNo(pai);

    freeNo(crianca);
    freeNo(irmao);

}

void insercaoNaoCheio(BTNo *no, const char* diretorio, char *caminho)
{
    char* nomeArquivo = NULL;
    extrairNomeArquivo(&nomeArquivo, caminho);

    int i = no->n - 1;

    if(no->ehFolha)
    {
        while((i >=0) && (strcmp(no->chaves[i], nomeArquivo) > 0))
        {
            // Previnimos um strcpy numa chave vazia.
            copiarChave(no, i + 1, no->chaves[i]);
            copiarCaminho(no, i + 1, no->caminhosArquivos[i]);

            i--;
        }

        // Colocamos o caminho do arquivo no mesmo indice do nome.
        copiarChave(no, i + 1, nomeArquivo);
        copiarCaminho(no, i + 1, caminho);

        no->n++;

        escreverNo(no);
    }
    else
    {
        while((i >= 0) && (strcmp(no->chaves[i], nomeArquivo) > 0))
        {
            i--;
        }

        i++;

        BTNo *filho = lerNo(no->filho[i]);

        if(filho->n == 2 * filho->t - 1)
        {
            splitChild(no, i, diretorio);

            if(strcmp(no->chaves[i], nomeArquivo) < 0)
            {
                i++;
            }

            freeNo(filho);
            filho = lerNo(no->filho[i]);
        }
        insercaoNaoCheio(filho, diretorio, caminho);
        freeNo(filho);
    }

    if(nomeArquivo)
        free(nomeArquivo);
}

void insercao(BT *arvore, char *caminho)
{
    char* nomeArquivo = NULL;
    extrairNomeArquivo(&nomeArquivo, caminho);

    if(arvore->raiz == NULL)
    {
        arvore->raiz = CriarNo(arvore->diretorio, arvore->t, true);

        copiarChave(arvore->raiz, 0, nomeArquivo);
        copiarCaminho(arvore->raiz, 0, caminho);

        arvore->raiz->n = 1;

        escreverNo(arvore->raiz);
    }
    else
    {
        if (BTbusca(arvore, nomeArquivo))
        {
            printf("\n [INSERÇÃO]: O arquivo \"%s\" já existe na árvore.\n",
                   caminho);
            return;
        }

        if(arvore->raiz->n == (2 * (arvore->t)) - 1)
        {
            BTNo *novaRaiz = CriarNo(arvore->diretorio, arvore->t, false);

            copiarFilho(novaRaiz, 0, arvore->raiz->caminhoNo);

            splitChild(novaRaiz, 0, arvore->diretorio);

            insercaoNaoCheio(novaRaiz, arvore->diretorio, caminho);

            arvore->raiz = novaRaiz;

        }
        else
        {
            insercaoNaoCheio(arvore->raiz, arvore->diretorio, caminho);

            escreverNo(arvore->raiz);
        }
    }

    if(nomeArquivo)
        free(nomeArquivo);
}
