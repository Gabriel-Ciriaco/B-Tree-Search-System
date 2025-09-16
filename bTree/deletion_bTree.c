#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "bTree.h"
#include "../file_manager/file_manager.h"


void remocao(BT* arvore, char* caminho)
{
    if (!arvore || !arvore->raiz)
    {
        printf("\n[REMOÇÃO]: A árvore dada está vazia.\n");
        return;
    }

    removerArvoreB(arvore->raiz, caminho);

    // Se a raiz ficou sem chaves
    if (arvore->raiz->n == 0)
    {
        BTNo *novaRaiz = NULL;

        if (!arvore->raiz->ehFolha)
        {
            novaRaiz = lerNo(arvore->raiz->filho[0]);
        }

        remove(arvore->raiz->nomeArquivo);

        freeNo(arvore->raiz);

        arvore->raiz = novaRaiz;
    }
}

void removerArvoreB(BTNo * raiz, char *k)
{
    if (!raiz)
    {
        printf("\n[REMOVER-ARVORE-B]: A árvore dada está vazia.\n");

        return;
    }

    int i = 0;

    while (i < raiz->n && strcmp(raiz->chaves[i], k) < 0)
    {
        i++;
    }

    // Se o valor está neste nó
    if (i < raiz->n && strcmp(raiz->chaves[i], k) == 0)
    {
        if (raiz->ehFolha)
        {
            removerFolhaArvoreB(raiz, k);
        }
        else
        {
            removerNaoFolhaArvoreB(raiz, k);
        }
    }
    else
    {
        // A chave não está na árvore.
        if (raiz->ehFolha)
        {
            printf("\n[REMOVER-ARVORE-B]: O caminho especificado não está na árvore.\n");

            return;
        }

        BTNo *filho = lerNo(raiz->filho[i]);

        /*
            Se o filho onde a chave deve estar tem
            apenas t - 1 chave, então devemos "rotaciona-lo"
        */
        if (filho->n < raiz->t)
        {
            fillNos(raiz, i);
        }

        /*
            Se o filho que recebeu o merge foi o último, então
            com certeza ele deu "merge" com o filho anterior.
            Logo, a recursão deve ocorrer no filho anterior
            -- até porque ele é o último filho agora.
            Caso contrário, faremos no filho i que agora tem
            t chaves.
        */
        if (i > raiz->n)
        {
            removerArvoreB(lerNo(raiz->filho[i - 1]), k);
        }
        else
        {
            removerArvoreB(lerNo(raiz->filho[i]), k);
        }
    }

    escreverNo(raiz);
}

bool removerFolhaArvoreB(BTNo *pai, char *k)
{
    if (!pai) return false;

    int i = 0;

    // Procura pelo k.
    while (i < pai->n && strcmp(pai->chaves[i], k) < 0)
    {
        i++;
    }

    // Encontramos o valor.
    if (i < pai->n && strcmp(pai->chaves[i], k) == 0)
    {
        // Removemos o valor da folha, movendo os valores para trás.
        while (i + 1 < pai->n)
        {
            copiarChave(pai, i, pai->chaves[i + 1]);
            i++;
        }

        pai->n--;

        escreverNo(pai);
        return true;
    }

    return false;
}

bool removerNaoFolhaArvoreB(BTNo *pai, char *k)
{
    int i = 0;

    while (i < pai->n && strcmp(pai->chaves[i], k) < 0) i++;

    // Se o nó interno contém k.
    if (i < pai->n && strcmp(pai->chaves[i], k) == 0)
    {
        BTNo *esq = lerNo(pai->filho[i]);
        BTNo *dir = lerNo(pai->filho[i + 1]);

        // Se o filho da esquerda tem t chaves
        if (esq->n >= pai->t)
        {
            char *predecessor = getPredecessor(pai, i);

            removerArvoreB(esq, predecessor);

            copiarChave(pai, i, predecessor);
        }
        // Se o filho da direita tem t chaves
        else if (dir->n >= pai->t)
        {
            char *sucessor = getSucessor(pai, i);

            removerArvoreB(dir, sucessor);

            copiarChave(pai, i, sucessor);
        }
        // Se ambos têm t - 1 chaves.
        else
        {
            mergeNos(pai, i);
            removerArvoreB(pai, k);
        }
    }

    escreverNo(pai);
    return true;
}

char *getPredecessor(BTNo *pai, int i)
{
    // Pegamos o filho da esquerda.
    BTNo *cur = lerNo(pai->filho[i]);

    // Vamos até o nó mais à direita.
    while (!cur->ehFolha)
    {
        cur = lerNo(cur->filho[cur->n]);
    }

    // O último elemento é retornado.
    return cur->chaves[cur->n - 1];
}

char *getSucessor(BTNo *pai, int i)
{
    // Pegamos o filho da direita
    BTNo *cur = lerNo(pai->filho[i + 1]);

    // Vamos até o nó mais à esquerda
    while (!cur->ehFolha)
    {
        cur = lerNo(cur->filho[0]);
    }

    // O primeiro elemento é retornado.
    return cur->chaves[0];
}

void emprestarDireita(BTNo *pai, int i)
{
    BTNo *filho = lerNo(pai->filho[i]);
    BTNo *irmao = lerNo(pai->filho[i - 1]);

    // Movendo as chaves um passo à direita
    for (int j = filho->n - 1; j >= 0; j--)
    {
        copiarChave(filho, j + 1, filho->chaves[j]);
    }

    if (!filho->ehFolha)
    {
        for (int j = filho->n; j >= 0; j--)
        {
            copiarFilho(filho, j + 1, filho->filho[j]);
        }
    }

    // Colocando a primeira chave do filho igual a chave[i-1]
    copiarChave(filho, 0, pai->chaves[i - 1]);

    // Movemos o ponteiro também.
    if (!filho->ehFolha)
    {
        copiarFilho(filho, 0, irmao->filho[irmao->n]);
    }

    // Movemos a chave do irmão para o pai.
    copiarChave(pai, i - 1, irmao->chaves[irmao->n - 1]);

    filho->n++;
    irmao->n--;

    escreverNo(filho);
    escreverNo(irmao);
    escreverNo(pai);
}

void emprestarEsquerda(BTNo *pai, int i)
{
    BTNo *filho = lerNo(pai->filho[i]);
    BTNo *irmao = lerNo(pai->filho[i + 1]);

    // Colocamos a chave do pai no final do filho.
    copiarChave(filho, filho->n, pai->chaves[i]);

    // Salvamos o filho do irmão.
    if (!filho->ehFolha)
    {
        copiarFilho(filho, filho->n + 1, irmao->filho[0]);
    }

    // Colocamos a chave do irmão no pai.
    copiarChave(pai, i, irmao->chaves[0]);

    // Movemos as chaves do irmão para trás
    for (int j = 1; j < irmao->n; j++)
    {
        copiarChave(irmao, j - 1, irmao->chaves[j]);
    }

    if (!irmao->ehFolha)
    {
        // Arrumamos os ponteiros do irmão
        for (int j = 1; j <= irmao->n; j++)
        {
            copiarFilho(irmao, j - 1, irmao->filho[j]);
        }
    }

    filho->n++;
    irmao->n--;

    escreverNo(filho);
    escreverNo(irmao);
    escreverNo(pai);
}

void fillNos(BTNo * pai, int i)
{
    BTNo * filhoAnterior = NULL;
    BTNo * filhoPosterior = NULL;

    if (i > 0)
        filhoAnterior = lerNo(pai->filho[i - 1]);

    if (i <= pai->n)
        filhoPosterior = lerNo(pai->filho[i]);

    /*
        Se o filho anterior tem mais do que t - 1 chaves,
        pegamos uma chave dele.
    */
    if (i != 0 && (filhoAnterior && filhoAnterior->n >= pai->t))
        emprestarDireita(pai, i);

    /*
        Se o filho posterior tem mais do que t - 1 chaves,
        pegamos uma chave dele
    */
    else if (i != pai->n && (filhoPosterior && filhoPosterior->n >= pai->t))
        emprestarEsquerda(pai, i);

    /*
        Merge o filho com o irmão.
        Se o filho for o último filho, merge com o irmão anterior.
        Caso contrário, com merge com o posterior.
    */
    else
    {
        if (i != pai->n)
            mergeNos(pai, i);
        else
            mergeNos(pai, i - 1);
    }

}

void mergeNos(BTNo *pai, int i)
{
    // Filho certo hehe
    BTNo *filho = lerNo(pai->filho[i]);
    BTNo *irmao = lerNo(pai->filho[i + 1]);

    // Retiramos a chave do nó e inserimos em t-1 do filho.
    copiarChave(filho, pai->t - 1, pai->chaves[i]);

    // Copiamos as chaves do irmão para o filho no final
    for (int j = 0; j < irmao->n; j++)
    {
        copiarChave(filho, j + pai->t, irmao->chaves[j]);
    }

    if (!irmao->ehFolha)
    {
        // Copiando os ponteiros do irmão para o filho
        for (int j = 0; j <= irmao->n; j++)
        {
            copiarFilho(filho, j + pai->t, irmao->filho[j]);
        }
    }

    /*
        Movemos todas as chaves após i para o nó atual
        para preencher o vazio criado ao mover
        pai->chaves[i] para filho->chaves[t - 1]
    */
    for (int j = i + 1; j < pai->n; j++)
    {
        copiarChave(pai, j - 1, pai->chaves[j]);
    }

    /*
        Também devemos mover os ponteiros desse mesmo vazio.
    */
    for (int j = i + 2; j <= pai->n; j++)
    {
        copiarFilho(pai, j - 1, pai->filho[j]);
    }

    // Atualizamos o valor da quantidade de chaves no filho e do nó atual.
    filho->n += irmao->n + 1; // O +1 é daquela chave que foi do pai pro filho.

    pai->n--;

    escreverNo(filho);
    escreverNo(pai);

    // Exclusão do irmão
    remove(irmao->nomeArquivo);
    freeNo(irmao);
}
