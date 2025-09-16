#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NO_NAME_SIZE 8 // Tamanho do nome do nó sem contar o ID dele.

#include "../bTree/bTree.h"


int NO_ID = 0; // ID do Nó usado para criar um nome específico dele.


BTNo * lerNo(const char * nomeArquivo)
{
    if (!nomeArquivo) return NULL;

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
        noLido->nomeArquivo = (char*) malloc(sizeof(char) * (nomeArquivoLen + 1));

        fread(noLido->nomeArquivo, sizeof(char) * nomeArquivoLen, 1, noBin);

        noLido->nomeArquivo[nomeArquivoLen] = '\0'; // Finalizar string.
    }


    // Verifica se não é um nó vazio.
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

            // Alocamos espaço para a chave.
            noLido->chaves[i] = (char*) malloc(sizeof(char) * (chaveLen + 1));

            fread(noLido->chaves[i], sizeof(char) * chaveLen, 1, noBin);

            noLido->chaves[i][chaveLen] = '\0'; // Finalizar string.
        }

        // Salvamos os filhos preenchidos.
        for (int i = 0; i <= noLido->n; i++)
        {
            int filhoLen = 0;

            fread(&filhoLen, sizeof(int), 1, noBin);

            // Alocamos espaço para o filho.
            noLido->filho[i] = (char*) malloc(sizeof(char) * (filhoLen + 1));

            fread(noLido->filho[i], sizeof(char) * filhoLen, 1, noBin);

            noLido->filho[i][filhoLen] = '\0'; // Finalizar string.
        }

        // Seta o resto das chaves como NULL.
        for (int i = noLido->n; i < 2 * noLido->t - 1; i++)
            noLido->chaves[i] = NULL;

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
    int nomeArquivoLen = strlen(noEscrever->nomeArquivo);

    fwrite(&nomeArquivoLen, sizeof(int), 1, noBin);
    fwrite(noEscrever->nomeArquivo, sizeof(char) * nomeArquivoLen, 1, noBin);

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
            int chaveLen = strlen(noEscrever->chaves[i]);

            fwrite(&chaveLen, sizeof(int), 1, noBin);
            fwrite(noEscrever->chaves[i], sizeof(char) * chaveLen, 1, noBin);
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




// adicionei essas duas

void escreverConfiguracoes(BT *arvore)
{
    if (arvore == NULL || arvore->raiz== NULL || arvore->raiz->nomeArquivo== NULL)
    {
        // nada p salvar
        printf("\n[CONFIG]: Nenhuma raiz ou nomeArquivo para salvar.\n");
        return;
    }

    FILE *arquivo = fopen("cfg.dat", "wb");

    if (arquivo == NULL)
    {
        // se nn abriu, avisa e para
        printf("\n\n [CONFIG]: ERRO ao criar arquivo de config\n\n");
        return;
    }

    // pega o tamanho do nome do arquivo
    int tamanhoNome = strlen(arvore->raiz->nomeArquivo);

    // coloca esse tamanho no cfg.dat
    fwrite(&tamanhoNome, sizeof(int), 1, arquivo);

    // agora coloca o nome real (sem o \0
    fwrite(arvore->raiz->nomeArquivo, sizeof(char), tamanhoNome, arquivo);

    fclose(arquivo);
    // config deu certo

}


void lerConfiguracoes(BT *arvore)
{
    if (arvore== NULL)
    {
        return; // sem arvore = sem resto de codigo
    }


    FILE *arquivo = fopen("cfg.dat", "rb");
    if (arquivo == NULL)
    {
        // se nn tem config ent avbisa
        printf("\n\n [CONFIG]: arquivo de configuração NAO encontrado.\n\n");
        return;
    }

    int tamanhoNome = 0;
    // recupera o tamanho salvo
    fread(&tamanhoNome, sizeof(int), 1, arquivo);


    if (tamanhoNome <= 0)
    {
        // se nn tem nome fecha e retorna
        fclose(arquivo);
        return;
    }


    // aloca espaço +1 pro '\0'
    char *nomeArquivoRaiz = (char*) malloc((tamanhoNome + 1) * sizeof(char));

    // le o nome
    fread(nomeArquivoRaiz, sizeof(char), tamanhoNome, arquivo);
    nomeArquivoRaiz[tamanhoNome] = '\0'; // fecha a string certinho


    fclose(arquivo);

    // monta a raiz direto do arquivo salvo
    arvore->raiz = lerNo(nomeArquivoRaiz);

    free(nomeArquivoRaiz);

}

