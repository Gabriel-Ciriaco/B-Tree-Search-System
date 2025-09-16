#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bTree.h"
#include "../file_manager/file_manager.h"


void splitChild(BTNo *pai, int indice)
{
    BTNo *crianca = lerNo(pai->filho[indice]);
    BTNo *irmao = CriarNo(crianca->t, crianca->ehFolha);

    irmao->n = crianca->t - 1;

    for(int i = 0; i < crianca->t-1; i++)
    {
        copiarChave(irmao, i, crianca->chaves[crianca->t + i]);
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
    }

    if(!pai->ehFolha)
    {
        for(int i = pai->n; i > indice; i--)
        {
            copiarFilho(pai, i + 1, pai->filho[i]);
        }
    }

    copiarChave(pai, indice, crianca->chaves[crianca->t - 1]);

    copiarFilho(pai, indice + 1, irmao->nomeArquivo);

    pai->n++;

    escreverNo(crianca);
    escreverNo(irmao);
    escreverNo(pai);

    freeNo(crianca);
    freeNo(irmao);

}

void insercaoNaoCheio(BTNo *no, char *caminho)
{
    int i = no->n - 1;

    if(no->ehFolha)
    {
        while((i >=0) && (strcmp(no->chaves[i], caminho) > 0))
        {
            // Previnimos um strcpy numa chave vazia.
            copiarChave(no, i + 1, no->chaves[i]);
            i--;
        }

        copiarChave(no, i + 1, caminho);
        no->n++;
    }
    else
    {
        while((i >= 0) && (strcmp(no->chaves[i], caminho) > 0))
        {
            i--;
        }

        i++;

        BTNo *filho = lerNo(no->filho[i]);

        if(filho->n == 2 * filho->t - 1)
        {
            splitChild(no, i);

            if(strcmp(no->chaves[i], caminho) < 0)
            {
                i++;
            }

            freeNo(filho);
            filho = lerNo(no->filho[i]);
        }
        insercaoNaoCheio(filho, caminho);
        freeNo(filho);
    }

}

void insercao(BT *arvore, char *caminho)
{
    if(arvore->raiz == NULL)
    {
        arvore->raiz = CriarNo(arvore->t, true);

        copiarChave(arvore->raiz, 0, caminho);

        arvore->raiz->n = 1;

        escreverNo(arvore->raiz);
    }
    else
    {
        if(arvore->raiz->n == (2 * (arvore->t)) - 1)
        {
            BTNo *novaRaiz = CriarNo(arvore->t, false);

            copiarFilho(novaRaiz, 0, arvore->raiz->nomeArquivo);

            splitChild(novaRaiz, 0);

            insercaoNaoCheio(novaRaiz, caminho);

            arvore->raiz = novaRaiz;

        }
        else
        {
            insercaoNaoCheio(arvore->raiz, caminho);

            escreverNo(arvore->raiz);
        }
    }
}
