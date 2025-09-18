#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NO_NAME_SIZE 9 // Tamanho do nome do n� sem contar o ID dele.

#include "../bTree/bTree.h"


int NO_ID = 0; // ID do N� usado para criar um nome espec�fico dele.


// Extrai o n�mero do padr�o "no_%d.bin" e seta o NO_ID.
void setNO_ID(int id)
{
    if (id < 0) return;

    NO_ID = id;
}

void extrairNomeArquivo(char** nomeArquivo, char* caminho)
{
    if (!caminho) return;

    // Encontrar a �ltima ocorr�ncia de '/' ou '\\' no caminho
    const char* nomeArquivoExtraido = strrchr(caminho, '/');

    // Caso n�o encontre '/', verifica '\\' (para sistemas Windows)
    if (!nomeArquivoExtraido)
        nomeArquivoExtraido = strrchr(caminho, '\\');

    // Avan�a um caractere para come�ar depois da barra
    if (nomeArquivoExtraido)
        nomeArquivoExtraido++;

    // Se n�o encontrar barra, ent�o todo o caminho � o nome do arquivo
    else
        nomeArquivoExtraido = caminho;

    if (!*nomeArquivo)
        *nomeArquivo = (char*) malloc(sizeof(char) * (strlen(nomeArquivoExtraido ) + 1));

        if (!*nomeArquivo)
        {
            printf("\n[EXTRAIR-NOME-ARQUIVO]: N�o foi poss�vel alocar espa�o para extrair o nome.\n");
        }

    strcpy(*nomeArquivo, nomeArquivoExtraido);
}

BTNo * lerNo(const char * caminhoNo)
{
    if (!caminhoNo) return NULL;

    FILE * noBin = fopen(caminhoNo, "rb");

    if (!noBin)
    {
        printf("\n[LER-NO]: O arquivo %s n�o foi encontrado.\n",
               caminhoNo);

        return NULL;
    }

    BTNo * noLido = (BTNo *) malloc(sizeof(BTNo));

    if (!noLido)
    {
        printf("\n[LER-NO]: N�o foi poss�vel alocar o arquivo %s.\n",
               caminhoNo);

        return NULL;
    }

    // Setamos como NULL inicialmente.
    noLido->caminhoNo = NULL;

    // Recuperando os campos mais simples.
    fread(&(noLido->ehFolha), sizeof(bool), 1, noBin);

    fread(&(noLido->n), sizeof(int), 1, noBin);

    fread(&(noLido->t), sizeof(int), 1, noBin);

    // Criamos os espa�os para as chaves e os filhos.
    noLido->chaves = (char **) malloc(sizeof(char*) * 2 * noLido->t - 1);
    noLido->caminhosArquivos = (char**) malloc(sizeof(char) * 2 * noLido->t - 1);
    noLido->filho = (char **) malloc(sizeof(char*) * 2 * noLido->t);

    if (!noLido->chaves || !noLido->caminhosArquivos || !noLido->filho)
    {
        freeNo(noLido);

        printf("\n[LER-NO]: N�o foi poss�vel alocar espa�o para o arquivo %s.\n",
               caminhoNo);

        return NULL;
    }

    // Recuperando o caminho do n�.
    int caminhoNoLen = 0;
    fread(&caminhoNoLen, sizeof(int), 1, noBin);

    if (caminhoNoLen > 0)
    {
        noLido->caminhoNo = (char*) malloc(sizeof(char) * (caminhoNoLen + 1));

        if(!noLido->caminhoNo)
        {
            printf("\n[LER-NO]: N�o foi poss�vel alocar espa�o para o arquivo %s.\n",
                   caminhoNo);

            freeNo(noLido);

            return NULL;
        }

        fread(noLido->caminhoNo, sizeof(char) * caminhoNoLen, 1, noBin);

        noLido->caminhoNo[caminhoNoLen] = '\0';
    }

    // Verifica se n�o � um n� vazio.
    if (noLido->n <= 0)
    {
        // Recuperando as chaves.
        for (int i = 0; i < 2 * noLido->t - 1; i++)
            noLido->chaves[i] = NULL;

        // Recuperando os filhos.
        for (int i = 0; i < 2 * noLido->t; i++)
            noLido->filho[i] = NULL;
    }
    else
    {
        // Salvamos as chaves preenchidas.
        for (int i = 0; i < noLido->n; i++)
        {
            int chaveLen = 0;

            fread(&chaveLen, sizeof(int), 1, noBin);

            // Alocamos espa�o para a chave.
            noLido->chaves[i] = (char*) malloc(sizeof(char) * (chaveLen + 1));

            if(!noLido->chaves[i])
            {
                printf("\n[LER-NO]: N�o foi poss�vel alocar espa�o para uma chave do arquivo %s.\n",
                       caminhoNo);

                freeNo(noLido);

                return NULL;
            }

            fread(noLido->chaves[i], sizeof(char) * chaveLen, 1, noBin);

            noLido->chaves[i][chaveLen] = '\0'; // Finalizar string.

            // Salvamos o caminho da chave.
            int caminhoChaveLen = 0;

            fread(&caminhoChaveLen, sizeof(int), 1, noBin);

            noLido->caminhosArquivos[i] = (char*) malloc(sizeof(char) * (caminhoChaveLen + 1));

            if(!noLido->caminhosArquivos[i])
            {
                printf("\n[LER-NO]: N�o foi poss�vel alocar espa�o para o caminho duma chave do arquivo %s.\n",
                       caminhoNo);

                freeNo(noLido);

                return NULL;
            }

            fread(noLido->caminhosArquivos[i], sizeof(char) * caminhoChaveLen, 1, noBin);

            noLido->caminhosArquivos[i][caminhoChaveLen] = '\0'; // Finalizar string.

        }

        // Salvamos os filhos preenchidos.
        for (int i = 0; i <= noLido->n; i++)
        {
            int filhoLen = 0;

            fread(&filhoLen, sizeof(int), 1, noBin);

            // Alocamos espa�o para o filho.
            noLido->filho[i] = (char*) malloc(sizeof(char) * (filhoLen + 1));

            if(!noLido->filho[i])
            {
                printf("\n[LER-NO]: N�o foi poss�vel alocar espa�o para um filho do arquivo %s.\n",
                       caminhoNo);

                freeNo(noLido);

                return NULL;
            }

            fread(noLido->filho[i], sizeof(char) * filhoLen, 1, noBin);

            noLido->filho[i][filhoLen] = '\0'; // Finalizar string.
        }

        // Seta o resto das chaves como NULL.
        for (int i = noLido->n; i < 2 * noLido->t - 1; i++)
        {
            noLido->chaves[i] = NULL;
            noLido->caminhosArquivos[i] = NULL;
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
        printf("\n[ESCREVE-NO]: Esse n� n�o foi encontrado.\n");

        return;
    }

    if (!noEscrever->caminhoNo)
    {
        printf("\n[ESCREVE-NO]: Esse n� n�o cont�m um caminho especificado para escrita.\n");

        return;
    }

    FILE * noBin = fopen(noEscrever->caminhoNo, "wb");

    if (!noBin)
    {
        printf("\n[ESCREVE-NO]: N�o foi poss�vel criar o n� desejado.\n");
        printf("\n%s\n", noEscrever->caminhoNo);

        return;
    }

    // Salvando os campos mais simples.
    fwrite(&(noEscrever->ehFolha), sizeof(bool), 1, noBin);

    fwrite(&(noEscrever->n), sizeof(int), 1, noBin);

    fwrite(&(noEscrever->t), sizeof(int), 1, noBin);


    // Salvando o caminho do arquivo com o seu tamanho.
    int caminhoNoLen = strlen(noEscrever->caminhoNo);

    fwrite(&caminhoNoLen, sizeof(int), 1, noBin);
    fwrite(noEscrever->caminhoNo, sizeof(char) * caminhoNoLen, 1, noBin);

    /*
        As chaves e os filhos, por serem arrays de strings,
        seguir�o o mesmo padr�o:

        Desconsiderando os espa�os e sendo,

        tamanhoStr1: int
        str1: string

        temos:

        tamanhoStr1str1 tamanhoStr2str2 ... tamanhoStrNstrN
    */
    if (noEscrever->n > 0)
    {
        for (int i = 0; i < noEscrever->n; i++)
        {
            int chaveLen = strlen(noEscrever->chaves[i]);

            fwrite(&chaveLen, sizeof(int), 1, noBin);
            fwrite(noEscrever->chaves[i], sizeof(char) * chaveLen, 1, noBin);

            int caminhoChaveLen = strlen(noEscrever->caminhosArquivos[i]);

            fwrite(&caminhoChaveLen, sizeof(int), 1, noBin);
            fwrite(noEscrever->caminhosArquivos[i], sizeof(char) * caminhoChaveLen, 1, noBin);
        }
    }

    if (!noEscrever->ehFolha)
    {
        for (int i = 0; i <= noEscrever->n; i++)
        {
            int filhoLen = strlen(noEscrever->filho[i]);

            fwrite(&filhoLen, sizeof(int), 1, noBin);
            fwrite(noEscrever->filho[i], sizeof(char) * filhoLen, 1, noBin);
        }
    }

    fclose(noBin);
}

/*
    Conta a quantidade de digitos
    usando uma equa��o matem�tica.
*/
int contarDigitos(int numero)
{
    if (numero == 0) return 1;

    return (int) log10(abs(numero)) + 1;
}

void criarNomeNo(const char* diretorio, BTNo * no)
{
    if (!no || !diretorio) return;

    NO_ID++;

    int caminhoSize = strlen(diretorio) + contarDigitos(NO_ID) + NO_NAME_SIZE;

    no->caminhoNo = (char *) malloc(sizeof(char) * caminhoSize);

    if (!no->caminhoNo)
    {
        no->caminhoNo = NULL;
        return;
    }

    snprintf(no->caminhoNo, caminhoSize, "%s/no_%d.bin", diretorio, NO_ID);
}
