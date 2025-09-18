#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>

#include "system.h"
#include "../bTree/bTree.h"
#include "../file_manager/file_manager.h"

extern int NO_ID; // ID do Nó definido file_manager.c


void mensagemBoasVindas()
{
    printf("\n***Bem-vindo ao nosso Sistema de Busca em Árvore B!***\n");
    printf("\nEste trabalho foi feito por Caroline Nunes, Gabriel Ciriaco e Mariana Feitosa.\n\n");
    printf("Nele você poderá observar:\n");
    printf("- A inserção de arquivos em uma árvore B.\n");
    printf("- A busca pelos arquivos.\n");
    printf("- Além da possibilidade de remoção deles.\n");
}

int rodarMenu()
{
    int opcao;

    printf("\n\n*** MENU PRINCIPAL ***\n\n");
    printf("1 - Criar árvore\n");
    printf("2 - Inserir arquivo\n");
    printf("3 - Buscar arquivo\n");
    printf("4 - Remover arquivo\n");
    printf("5 - Visualizar os arquivos da árvore\n");
    printf("6 - Destruir árvore\n");
    printf("0 - Sair\n");
    printf("---------------------------\n");
    printf("Digite o número da opção escolhida: ");
    scanf("%d", &opcao);

    return opcao;
}

void exibirMenu()
{
    printf("=== MENU ÁRVORE B ===\n\n");

    printf("1 - Criar árvore\n");
    printf("2 - Inserir arquivo\n");
    printf("3 - Buscar arquivo\n");
    printf("4 - Remover arquivo\n");
    printf("5 - Visualizar os arquivos da árvore\n");
    printf("6 - Destruir árvore\n");
    printf("0 - Sair\n");
}

void executarSistema()
{
    setlocale(LC_ALL, "Portuguese");

    mensagemBoasVindas();

    Sistema novoSistema; //Cria o sistema.

    novoSistema.arvore = NULL; //Inicializa a árvore como NULL.

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
                    printf("\n** Já existe uma árvore criada. Caso queira refazê-la, a destrua primeiro. **\n");
                    break;
                }

                printf("Digite o grau mínimo t da árvore: ");
                scanf("%d", &t1);
                getchar(); //Limpa o buffer do teclado.

                novoSistema.arvore = CriarBT(ARVORE_PATH, t1);

                if (novoSistema.arvore)
                {
                    printf("\n** Árvore criada com sucesso. **\n");
                }
                else
                {
                    printf("\n** Houve algum problema ao criar a árvore. ** \n");
                }

                break;

            case 2:

                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a árvore para depois inserir. **\n");
                    break;
                }

                printf("Digite o caminho do arquivo deseja inserir: ");

                setbuf(stdin, NULL);

                fgets(caminho, 100, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                insercao(novoSistema.arvore, caminho);

                printf("\n** Inserção concluída com sucesso. **\n");

                break;

            case 3:

                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a árvore para depois buscar. **\n");
                    break;
                }

                printf("Digite o nome do arquivo que você deseja procurar: ");

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
                    printf("\n** O arquivo \"%s\" não foi encontrado. ** \n", caminho);
                }


                break;


            case 4:
            {
                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a árvore para depois remover. **\n");
                    break;
                }

                printf("Digite o caminho do arquivo que você deseja remover: ");

                setbuf(stdin, NULL);
                fgets(caminho, 30, stdin);
                caminho[strcspn(caminho, "\n")] = '\0';

                remocao(novoSistema.arvore, caminho);

                printf("\n** Remoção concluída com sucesso. **\n");

                break;
            }

            case 5:
                if(novoSistema.arvore == NULL)
                {
                    printf("\n** Crie primeiro a árvore para depois visualizar. **\n");
                    break;
                }

                printf("\n** ARQUIVOS DO SISTEMA EM ORDEM ALFABÉTICA **\n\n");
                imprimeArvore(novoSistema.arvore);

                break;

            case 6:

                if (arvoreJaExiste || novoSistema.arvore)
                {
                    printf("\n** Deseja destruir a árvore atual? Digite 1 para sim e 2 para não. **\n");

                    setbuf(stdin, NULL);
                    scanf("%d", &respostaDestruir);

                    if (respostaDestruir == 1)
                    {
                        freeBTree(novoSistema.arvore);
                        remove(CFG_PATH);

                        novoSistema.arvore = NULL;
                        arvoreJaExiste = false;

                        NO_ID = 0; // Reseta o contador de nós.

                        destruirArvoreSistema();

                        printf("\n** Árvore destruída com sucesso! **\n");
                    }

                    break;
                }

                printf("\n** É necessário criar uma árvore para acessar esta função. **\n");

                break;

            case 0:


                // Salvamos a raiz nas configurações do sistema.
                escreverConfiguracoes(novoSistema);

                if(freeBTree(novoSistema.arvore))
                {
                     printf("\n** Limpando a árvore. **\n");
                }

                printf("\n** Saindo. ** \n");

                break;

            default:

                printf("\n** Opção inválida. ** \n");
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
        printf("\n\n [CONFIG]: Arquivo de configuração NAO encontrado.\n\n");
        return false;
    }


    // Recupera o NO_ID
    int tempNO_ID = 0;

    fread(&tempNO_ID, sizeof(int), 1, arquivo);

    setNO_ID(tempNO_ID);

    // Recupera o grau mínimo.
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


    // aloca espaço +1 pro '\0'
    char *caminhoNoRaiz = (char*) malloc((tamanhoNome + 1) * sizeof(char));

    if (!caminhoNoRaiz)
    {
        printf("\n[CONFIG]: Não foi possível alocar espaço para o caminho do nó raiz.\n");

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
        printf("\n\n [CONFIG]: ERRO ao criar arquivo de configuração.\n\n");
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

    // Salva o grau mínimo da árvore.
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
        printf("\n**Não foi possível abrir a pasta dos arquivos da árvore **\n");
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
            printf("\n** Erro ao remover o nó %s **\n", caminho);
    }

    closedir(dir);

}
