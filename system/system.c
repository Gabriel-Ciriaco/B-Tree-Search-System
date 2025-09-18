#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

#include "system.h"
#include "../bTree/bTree.h"
#include "../file_manager/file_manager.h"

extern int NO_ID; // ID do N� definido file_manager.c


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
    printf("5 - Visualizar os arquivos da �rvore\n");
    printf("6 - Destruir �rvore\n");
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
    printf("5 - Visualizar os arquivos da �rvore\n");
    printf("6 - Destruir �rvore\n");
    printf("0 - Sair\n");
}

void executarSistema()
{
    setlocale(LC_ALL, "Portuguese");

    mensagemBoasVindas();

    Sistema novoSistema; //Cria o sistema.

    novoSistema.arvore = NULL; //Inicializa a �rvore como NULL.

    int opcao = 0;
    int t1 = 0;
    int respostaDestruir = 0;
    char caminho[100];

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

                novoSistema.arvore = CriarBT(ARVORE_PATH, t1);

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

                fgets(caminho, 100, stdin);
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

                printf("Digite o nome do arquivo que voc� deseja procurar: ");

                setbuf(stdin, NULL);
                fgets(caminho, 100, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                BTNo *encontrado = BTbusca(novoSistema.arvore, caminho);

                if (encontrado)
                {
                    int posicao = posElemento(encontrado, caminho);

                    printf("\n** O arquivo \"%s\" foi encontrado: %s. **\n", caminho, encontrado->caminhosArquivos[posicao]);
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

            case 5:
                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a �rvore para depois visualizar. **\n");
                    break;
                }

                printf("\n** ARQUIVOS DO SISTEMA EM ORDEM ALFAB�TICA **\n\n");
                imprimeArvore(novoSistema.arvore);

                break;

            case 6:

                if (arvoreJaExiste || novoSistema.arvore)
                {
                    printf("\n** Deseja destruir a �rvore atual? Digite 1 para sim e 2 para n�o. **\n");

                    setbuf(stdin, NULL);
                    scanf("%d", &respostaDestruir);

                    if (respostaDestruir == 1)
                    {
                        freeBTree(novoSistema.arvore);
                        remove(CFG_PATH);

                        novoSistema.arvore = NULL;
                        arvoreJaExiste = false;

                        NO_ID = 0; // Reseta o contador de n�s.

                        destruirArvoreSistema();

                        printf("\n** �rvore destru�da com sucesso! **\n");
                    }

                    break;
                }

                printf("\n** � necess�rio criar uma �rvore para acessar esta fun��o. **\n");

                break;

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

        setbuf(stdin, NULL); // Limpa o buffer, caso algo tenha sobrado.
        getchar(); // Espera o ENTER.
        system("cls");

    }while(opcao != 0);
}


bool lerConfiguracoes(Sistema * sistema)
{
    FILE *arquivo = fopen(CFG_PATH, "rb");

    if (arquivo == NULL)
    {
        // se nn tem config ent avisa
        printf("\n\n [CONFIG]: Arquivo de configura��o NAO encontrado.\n\n");
        return false;
    }


    // Recupera o NO_ID
    int tempNO_ID = 0;

    fread(&tempNO_ID, sizeof(int), 1, arquivo);

    setNO_ID(tempNO_ID);

    // Recupera o grau m�nimo.
    int t = 0;
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
    char *caminhoNoRaiz = (char*) malloc((tamanhoNome + 1) * sizeof(char));

    if (!caminhoNoRaiz)
    {
        printf("\n[CONFIG]: N�o foi poss�vel alocar espa�o para o caminho do n� raiz.\n");

        return false;
    }

    // le o nome
    fread(caminhoNoRaiz, sizeof(char), tamanhoNome, arquivo);
    caminhoNoRaiz[tamanhoNome] = '\0'; // fecha a string certinho

    fclose(arquivo);

    // monta a raiz direto do arquivo salvo
    sistema->arvore = CriarBT(ARVORE_PATH, t);
    (sistema->arvore)->raiz = lerNo(caminhoNoRaiz);

    free(caminhoNoRaiz);

    return true;
}

void escreverConfiguracoes(Sistema sistema)
{
    BT* arvore = sistema.arvore;

    FILE *arquivo = fopen(CFG_PATH, "wb");

    if (arquivo == NULL)
    {
        // se nn abriu, avisa e para
        printf("\n\n [CONFIG]: ERRO ao criar arquivo de configura��o.\n\n");
        return;
    }

    if (arvore == NULL || arvore->raiz== NULL || arvore->raiz->caminhoNo== NULL)
    {
        // nada p salvar
        printf("\n[CONFIG]: Nenhuma raiz para salvar.\n");
        remove("cfg.dat");

        fclose(arquivo);
        return;
    }


    // Salva o NO_ID
    fwrite(&NO_ID, sizeof(int), 1, arquivo);

    // Salva o grau m�nimo da �rvore.
    fwrite(&sistema.arvore->t, sizeof(int), 1, arquivo);

    // pega o tamanho do nome do arquivo
    int tamanhoNome = strlen(arvore->raiz->caminhoNo);

    // coloca esse tamanho no cfg.dat
    fwrite(&tamanhoNome, sizeof(int), 1, arquivo);

    // agora coloca o nome real (sem o \0
    fwrite(arvore->raiz->caminhoNo, sizeof(char), tamanhoNome, arquivo);

    fclose(arquivo);
    // config deu certo
}

void destruirArvoreSistema()
{
    struct dirent *entry;

    char caminho[512];

    DIR *dir = opendir(ARVORE_PATH);

    if (dir == NULL)
    {
        printf("\n**N�o foi poss�vel abrir a pasta dos arquivos da �rvore **\n");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        // Ignora . e ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Monta o caminho completo do arquivo
        snprintf(caminho, sizeof(caminho), "%s/%s", ARVORE_PATH, entry->d_name);

        // Tenta apagar
        if (remove(caminho) == 0)
            printf("\nRemovido: %s\n", caminho);
        else
            printf("\n** Erro ao remover o n� %s **\n", caminho);
    }

    closedir(dir);

}
