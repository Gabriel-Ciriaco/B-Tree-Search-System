#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

#include "system.h"
#include "../bTree/bTree.h"
#include "../file_manager/file_manager.h"


void mensagemBoasVindas()
{
    printf("\n***Bem-vindo ao nosso Sistema de Busca em �rvore B!***\n");
    printf("\nEste trabalho foi feito por Caroline Nunes, Gabriel Ciriaco e Mariana Feitosa.\n\n");
    printf("Nele voc� poder� observar:\n");
    printf("- A inser��o de arquivos em uma �rvore B.\n");
    printf("- A busca pelos arquivos.\n");
    printf("- Al�m da possibilidade de remo��o deles.\n");
}

int rodarMenu()
{
    int opcao;

    printf("\n\n*** MENU PRINCIPAL ***\n\n");
    printf("1 - Criar �rvore\n");
    printf("2 - Inserir arquivo\n");
    printf("3 - Buscar arquivo\n");
    printf("4 - Remover arquivo\n");
    printf("0 - Sair\n");
    printf("---------------------------\n");
    printf("Digite o n�mero da op��o escolhida: ");
    scanf("%d", &opcao);

    return opcao;
}

void exibirMenu()
{
    printf("=== MENU �RVORE B ===\n\n");

    printf("1 - Criar �rvore\n");
    printf("2 - Inserir arquivo\n");
    printf("3 - Buscar arquivo\n");
    printf("4 - Remover arquivo\n");
    printf("0 - Sair\n");
}

void executarSistema()
{
    setlocale(LC_ALL, "Portuguese");

    mensagemBoasVindas();

    Sistema novoSistema; //Cria o sistema.

    novoSistema.arvore = NULL; //Inicializa a �rvore como NULL.

    int opcao;
    int t1;
    char caminho[30];

    bool arvoreJaExiste = lerConfiguracoes(&novoSistema);

    do{

        opcao = rodarMenu();

        switch(opcao)
        {
            case 1:

                if (arvoreJaExiste)
                {
                    printf("\n** J� existe uma �rvore criada. Caso queira refaz�-la, a destrua primeiro. **\n");
                    break;
                }

                printf("Digite o grau m�nimo t da �rvore: ");
                scanf("%d", &t1);
                getchar(); //Limpa o buffer do teclado.

                novoSistema.arvore = CriarBT(t1);

                if (novoSistema.arvore)
                {
                    printf("\n** �rvore criada com sucesso. **\n");
                }
                else
                {
                    printf("\n** Houve algum problema ao criar a �rvore. ** \n");
                }

                break;

            case 2:

                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a �rvore para depois inserir. **\n");
                    break;
                }

                printf("Digite o caminho do arquivo deseja inserir: ");

                setbuf(stdin, NULL);

                fgets(caminho, 30, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                insercao(novoSistema.arvore, caminho);

                printf("\n** Inser��o conclu�da com sucesso. **\n");

                break;

            case 3:

                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a �rvore para depois buscar. **\n");
                    break;
                }

                printf("Digite o caminho do arquivo que voc� deseja procurar: ");

                setbuf(stdin, NULL);
                fgets(caminho, 30, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                BTNo *encontrado = BTbusca(novoSistema.arvore, caminho);

                if (encontrado)
                {
                    printf("\n** O arquivo \"%s\" foi encontrado (no->nomeArquivo = %s). **\n", caminho, encontrado->nomeArquivo);
                }
                else
                {
                    printf("\n** O arquivo \"%s\" n�o foi encontrado. ** \n", caminho);
                }


                break;


            case 4:
            {
                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a �rvore para depois remover. **\n");
                    break;
                }

                printf("Digite o caminho do arquivo que voc� deseja remover: ");

                setbuf(stdin, NULL);
                fgets(caminho, 30, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                remocao(novoSistema.arvore, caminho);

                printf("\n** Remo��o conclu�da com sucesso. **\n");

                break;
            }

            case 0:


                // Salvamos a raiz nas configura��es do sistema.
                escreverConfiguracoes(novoSistema);

                if(freeBTree(novoSistema.arvore))
                {
                     printf("\n** Limpando a �rvore. **\n");
                }

                printf("\n** Saindo. ** \n");

                break;

            default:

                printf("\n** Op��o inv�lida. ** \n");
                break;

        }

        printf("\nPressione ENTER para continuar...");

        fflush(stdin); // Limpa o buffer, caso algo tenha sobrado.
        getchar(); // Espera o ENTER.

    }while(opcao != 0);
}


bool lerConfiguracoes(Sistema * sistema)
{
    FILE *arquivo = fopen("cfg.dat", "rb");

    if (arquivo == NULL)
    {
        // se nn tem config ent avisa
        printf("\n\n [CONFIG]: Arquivo de configura��o NAO encontrado.\n\n");
        return false;
    }

    int t = 0;

    // Recupera o grau m�nimo.
    fread(&t, sizeof(int), 1, arquivo);

    int tamanhoNome = 0;
    // recupera o tamanho salvo
    fread(&tamanhoNome, sizeof(int), 1, arquivo);


    if (tamanhoNome <= 0)
    {
        // se nn tem nome fecha e retorna
        fclose(arquivo);
        return false;
    }


    // aloca espa�o +1 pro '\0'
    char *nomeArquivoRaiz = (char*) malloc((tamanhoNome + 1) * sizeof(char));

    // le o nome
    fread(nomeArquivoRaiz, sizeof(char), tamanhoNome, arquivo);
    nomeArquivoRaiz[tamanhoNome] = '\0'; // fecha a string certinho

    fclose(arquivo);

    // monta a raiz direto do arquivo salvo
    sistema->arvore = CriarBT(t);
    (sistema->arvore)->raiz = lerNo(nomeArquivoRaiz);

    setNO_ID(nomeArquivoRaiz);

    free(nomeArquivoRaiz);

    return true;
}

void escreverConfiguracoes(Sistema sistema)
{
    BT* arvore = sistema.arvore;

    FILE *arquivo = fopen("cfg.dat", "wb");

    if (arquivo == NULL)
    {
        // se nn abriu, avisa e para
        printf("\n\n [CONFIG]: ERRO ao criar arquivo de configura��o.\n\n");
        return;
    }

    if (arvore == NULL || arvore->raiz== NULL || arvore->raiz->nomeArquivo== NULL)
    {
        // nada p salvar
        printf("\n[CONFIG]: Nenhuma raiz para salvar.\n");
        remove("cfg.dat");

        fclose(arquivo);
        return;
    }


    // Salva o grau m�nimo da �rvore.
    fwrite(&sistema.arvore->t, sizeof(int), 1, arquivo);

    // pega o tamanho do nome do arquivo
    int tamanhoNome = strlen(arvore->raiz->nomeArquivo);

    // coloca esse tamanho no cfg.dat
    fwrite(&tamanhoNome, sizeof(int), 1, arquivo);

    // agora coloca o nome real (sem o \0
    fwrite(arvore->raiz->nomeArquivo, sizeof(char), tamanhoNome, arquivo);

    fclose(arquivo);
    // config deu certo
}
