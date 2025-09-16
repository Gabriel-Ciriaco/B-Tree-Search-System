#include "system.h"

#include "bTree.h"
#include "insertion_bTree.h"
#include "search_bTree.h"

void mensagemBoasVindas()
{
    printf("\n***Bem-vindo ao nosso Sistema de Busca em �rvore B!***\n");
    printf("\nEste trabalho foi feito por Caroline Nunes, Gabriel Ciriaco e Mariana Feitosa.\n");
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

    do{

        opcao = rodarMenu();

        switch(opcao)
        {
            case 1:

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
                fgets(caminho, 30, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                insercao(novoSistema.arvore, caminho);

                printf("\n** Arquivo %s inserido com sucesso. **\n", caminho);

                break;

            case 3:

                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a �rvore para depois buscar. **\n");
                    break;
                }

                printf("Digite o caminho do arquivo que voc� deseja procurar: ");
                fgets(caminho, 30, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                BTNo *encontrado = BTbusca(novoSistema.arvore, caminho);

                if (encontrado)
                {
                    printf("\n** O arquivo %s foi encontrado (no->nomeArquivo = %s). **\n", caminho, encontrado->nomeArquivo);
                }
                else
                {
                    printf("\n** O arquivo %s n�o foi encontrado. ** \n", caminho);
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
                fgets(caminho, 30, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                //TO-DO: Colocar a fun��o de remo��o do Gabriel aqui.

                printf("\n** Arquivo %s removido com sucesso. **\n", caminho);

                break;
            }

            case 0:

                printf("\n** Saindo. ** \n");

                if(freeBTree(novoSistema.arvore))
                {
                     printf("\n** Limpando a �rvore. **\n");
                }

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
