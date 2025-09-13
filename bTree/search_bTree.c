#include <string.h>

#include "bTree.h"
#include "../file_manager/file_manager.h"


/*
    Achamos em qual posição está o caracter ou onde
    ele deveria estar.

    Usamos o strcmp para comparar as strings por ordem alfabética.
*/
int posElemento(BTNo *no, char *procurado)
{
    int esquerda = 0;
    int direita = no->n - 1;

    while(esquerda <= direita)
    {
        int meio = (esquerda + direita) / 2;

        // O resultado é como um sinalizador, ele fala se a string procurada vem antes, depois ou é igual a chave do meio.
        int resultado = strcmp(procurado, no->chaves[meio]);

        if(resultado < 0)
        {
            direita = meio - 1; // procurado vem antes move busca pro lado esquerdo
        }

        else if(resultado > 0)
        {
            esquerda = meio + 1; // procurado vem depois, move busca pro lado direito
        }

        else
        {
            return meio; // achou!!!
        }

    }

    return esquerda; // Posição onde era para estar.
}


BTNo *BTbusca(BT *arvore, char *procurado)
{

    if(arvore == NULL || arvore->raiz == NULL)
    {
       return NULL;
    }

    BTNo * atual = arvore->raiz;
    BTNo * proximo = NULL;

    while(atual != NULL)
    {
        int posicao = posElemento(atual, procurado);

        /*
            Se a posição que retornou é menor que o máximo de chaves (ou seja está dentro do nó)
            e a chave da posição e o procurado são iguais, então ele encontrou!
        */
        if(posicao < atual->n && strcmp(atual->chaves[posicao], procurado) == 0)
        {
            return atual;
        }

        // Se não foi encontrado no nó mas é folha, então não tem mais onde procurar, logo o procurado não está na árvore.
        if(atual->ehFolha == true)
        {
            if(atual != arvore->raiz)
            {
                freeNo(atual);
            }

            return NULL;
        }
         else // Não é folha
        {

            char * nomeproximo = atual->filho[posicao];

            proximo = lerNo(nomeproximo);

            if(proximo == NULL)
            {
                if(atual != arvore->raiz)
                {
                    freeNo(atual);

                }
                return NULL;
            }

            if(atual != arvore->raiz)
            {
               freeNo(atual);

            }

            atual = proximo; // Próximo já está carregado com o nome e os atributos do filho!!!
        }
    }
        return NULL;
}
