#include <string.h>

#include "bTree.h"
#include "../file_manager/file_manager.h"


/*
    Achamos em qual posi��o est� o caracter ou onde
    ele deveria estar.

    Usamos o strcmp para comparar as strings por ordem alfab�tica.
*/
int posElemento(BTNo *no, char *procurado)
{
    int esquerda = 0;
    int direita = no->n - 1;

    while(esquerda <= direita)
    {
        int meio = (esquerda + direita) / 2;

        // O resultado � como um sinalizador, ele fala se a string procurada vem antes, depois ou � igual a chave do meio.
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

    return esquerda; // Posi��o onde era para estar.
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
            Se a posi��o que retornou � menor que o m�ximo de chaves (ou seja est� dentro do n�)
            e a chave da posi��o e o procurado s�o iguais, ent�o ele encontrou!
        */
        if(posicao < atual->n && strcmp(atual->chaves[posicao], procurado) == 0)
        {
            return atual;
        }

        // Se n�o foi encontrado no n� mas � folha, ent�o n�o tem mais onde procurar, logo o procurado n�o est� na �rvore.
        if(atual->ehFolha == true)
        {
            if(atual != arvore->raiz)
            {
                freeNo(atual);
            }

            return NULL;
        }
         else // N�o � folha
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

            atual = proximo; // Pr�ximo j� est� carregado com o nome e os atributos do filho!!!
        }
    }
        return NULL;
}
