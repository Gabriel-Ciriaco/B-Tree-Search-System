#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NO_NAME_SIZE 8 // Tamanho do nome do nó sem contar o ID dele.

#include "../bTree/bTree.h"


int NO_ID = 0; // ID do Nó usado para criar um nome específico dele.


BTNo * lerNo(const char * nomeArquivo)
{
    FILE * noBin = fopen(nomeArquivo, "rb");

    if (!noBin)
    {
        printf("\n[LER-NO]: O arquivo %s não foi encontrado.\n",
               nomeArquivo);

        return NULL;
    }

    BTNo * noLido = (BTNo *) malloc(sizeof(BTNo));

    // Setamos como NULL inicialmente.
    noLido->nomeArquivo = NULL;

    // Recuperando os campos mais simples.
    fread(&(noLido->ehFolha), sizeof(bool), 1, noBin);

    fread(&(noLido->n), sizeof(int), 1, noBin);

    fread(&(noLido->t), sizeof(int), 1, noBin);

    // Criamos os espaços para as chaves e os filhos.
    noLido->chaves = (char **) malloc(sizeof(char*) * 2 * noLido->t - 1);
    noLido->filho = (char **) malloc(sizeof(char*) * 2 * noLido->t);

    // Recuperando o nome do arquivo.
    int nomeArquivoLen = 0;

    fread(&nomeArquivoLen, sizeof(int), 1, noBin);

    if (nomeArquivoLen > 0)
    {
        noLido->nomeArquivo = (char*) malloc(sizeof(char) * nomeArquivoLen);

        fread(noLido->nomeArquivo, sizeof(char) * nomeArquivoLen, 1, noBin);

        noLido->nomeArquivo[nomeArquivoLen] = '\0'; // Finalizar string.
    }

    // Vemos quantas chaves e quantos filhos o nó têm.
    int qtdChaves = 0;
    int qtdFilhos = 0;

    fread(&qtdChaves, sizeof(int), 1, noBin);
    fread(&qtdFilhos, sizeof(int), 1, noBin);


    // Recuperando as chaves.
    if (qtdChaves <= 0)
    {
        for (int i = 0; i < 2 * noLido->t - 1; i++)
            noLido->chaves[i] = NULL;
    }
    else
    {
        // Salvamos as chaves preenchidas.
        for (int i = 0; i < noLido->n; i++)
        {
            int chaveLen = 0;

            fread(&chaveLen, sizeof(int), 1, noBin);

            // Alocamos espaço para a chave.
            noLido->chaves[i] = (char*) malloc(sizeof(char) * chaveLen);

            fread(noLido->chaves[i], sizeof(char) * chaveLen, 1, noBin);

            noLido->chaves[i][chaveLen - 1] = '\0'; // Finalizar string.
        }

        // Seta o resto das chaves como NULL.
        for (int i = noLido->n; i < 2 * noLido->t - 1; i++)
            noLido->chaves[i] = NULL;
    }

    // Recuperando os filhos.
    if (qtdFilhos <= 0)
    {
        for (int i = 0; i < 2 * noLido->t; i++)
            noLido->filho[i] = NULL;
    }
    else
    {
        for (int i = 0; i <= noLido->n; i++)
        {
            int filhoLen = 0;

            fread(&filhoLen, sizeof(int), 1, noBin);

            // Alocamos espaço para o filho.
            noLido->filho[i] = (char*) malloc(sizeof(char) * filhoLen);

            fread(noLido->filho[i], sizeof(char) * filhoLen, 1, noBin);

            noLido->filho[i][filhoLen - 1] = '\0'; // Finalizar string.
        }

        // Seta o resto dos filhos como NULL.
        for (int i = noLido->n + 1; i < 2 * noLido->t; i++)
            noLido->filho[i] = NULL;
    }


    fclose(noBin);

    return noLido;
}

void escreverNo(BTNo * noEscrever)
{
    if (!noEscrever)
    {
        printf("\n[ESCREVE-NO]: Esse nó não foi encontrado.\n");

        return;
    }

    if (!noEscrever->nomeArquivo)
    {
        printf("\n[ESCREVE-NO]: Esse nó não contém um caminho especificado para escrita.\n");

        return;
    }

    FILE * noBin = fopen(noEscrever->nomeArquivo, "wb");

    if (!noBin)
    {
        printf("\n[ESCREVE-NO]: Não foi possível criar o nó desejado.\n");

        return;
    }

    // Salvando os campos mais simples.
    fwrite(&(noEscrever->ehFolha), sizeof(bool), 1, noBin);

    fwrite(&(noEscrever->n), sizeof(int), 1, noBin);

    fwrite(&(noEscrever->t), sizeof(int), 1, noBin);


    // Salvando o nome do arquivo com o seu tamanho.
    int nomeArquivoLen = strlen(noEscrever->nomeArquivo) + 1;

    fwrite(&nomeArquivoLen, sizeof(int), 1, noBin);
    fwrite(noEscrever->nomeArquivo, sizeof(char) * nomeArquivoLen, 1, noBin);


    /*
        Antes de salvarmos as chaves e os filhos,
        salvamos quantas chaves e quantos filhos temos.
    */
    int qtdChaves = 0;
    int qtdFilhos = 0;


    if (noEscrever->n > 0)
    {
        for (int i = 0; i < noEscrever->n; i++)
            qtdChaves++;
    }

    if (!noEscrever->ehFolha)
    {
        for (int i = 0; i <= noEscrever->n; i++)
            qtdFilhos++;
    }

    fwrite(&qtdChaves, sizeof(int), 1, noBin);

    fwrite(&qtdFilhos, sizeof(int), 1, noBin);


    /*
        As chaves e os filhos, por serem arrays de strings,
        seguirão o mesmo padrão:

        Desconsiderando os espaços e sendo,

        tamanhoStr1: int
        str1: string

        temos:

        tamanhoStr1str1 tamanhoStr2str2 ... tamanhoStrNstrN
    */
    if (noEscrever->n > 0)
    {
        for (int i = 0; i < noEscrever->n; i++)
        {
            int chaveLen = strlen(noEscrever->chaves[i]) + 1;

            fwrite(&chaveLen, sizeof(int), 1, noBin);
            fwrite(noEscrever->chaves[i], sizeof(char) * chaveLen, 1, noBin);
        }
    }

    if (!noEscrever->ehFolha)
    {
        for (int i = 0; i <= noEscrever->n; i++)
        {
            int filhoLen = strlen(noEscrever->filho[i]) + 1;

            fwrite(&filhoLen, sizeof(int), 1, noBin);
            fwrite(noEscrever->filho[i], sizeof(char) * filhoLen, 1, noBin);
        }
    }

    fclose(noBin);
}

/*
    Conta a quantidade de digitos
    usando uma equação matemática.
*/
int contarDigitos(int numero)
{
    if (numero == 0) return 1;

    return (int) log10(abs(numero)) + 1;
}

void criarNomeNo(BTNo * no)
{
    if (!no || no->nomeArquivo) return;

    NO_ID++;

    int nomeSize = contarDigitos(NO_ID) + NO_NAME_SIZE;

    no->nomeArquivo = (char *) malloc(sizeof(char) * nomeSize);

    sprintf(no->nomeArquivo, "no_%d.bin", NO_ID);
}
