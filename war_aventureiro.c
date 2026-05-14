// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // PARA strcspn()
#include <time.h>   // Para Time para srand()

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio
{
    char nome[30];
    char corExercito[10];
    int tropas;
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
void liberarMemoria(struct Territorio *mapa)
{
    if (mapa != NULL)
    {
        free(mapa);
        printf("\nMemoria do mapa liberada.\n");
    }
}

// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função que simula a batalha entre dois territórios
void atacar(struct Territorio *atacante, struct Territorio *defensor)
{
    // Semente já inicializada no main
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou o dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou o dado e tirou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor)
    {
        printf("Vitoria do Atacante! O defensor perdeu 1 tropa.\n");
        if (defensor->tropas > 0)
        {
            defensor->tropas--;
            if (defensor->tropas == 0)
            {
                printf("%s foi conquistado por Exercito %s!\n", defensor->nome, atacante->corExercito);
                // Atualiza a cor para a cor do conquistador
                strcpy(defensor->corExercito, atacante->corExercito);
                // Transfere metade das tropas (mínimo 1)
                int tropasTransferidas = atacante->tropas / 2;
                defensor->tropas = tropasTransferidas;
                atacante->tropas -= tropasTransferidas;
            }
        }
    }
    else
    {
        if (atacante->tropas > 1)
        {
            printf("O Defensor resistiu! O atacante perdeu 1 tropa.\n");
            atacante->tropas--;
        }
        else
        {
            printf("O Defensor resistiu! O atacante não tem tropas sobressalentes para perder.\n");
        }
    }
}

// Função utilitária:
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main()
{
    // 1. Configuração Inicial (Setup):
    struct Territorio *mapa;
    int totalTropas = 0;

    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    mapa = (struct Territorio *)calloc(5, sizeof(struct Territorio));
    if (mapa == NULL)
    {
        printf("Erro: Falha ao alocar memória./n");
        return 1;
    }

    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    printf("==================================================================\n\n");
    printf("--- Vamos cadastrar os 5 territorios iniciais do nosso mundo. ---\n\n");
    for (totalTropas = 0; totalTropas < 5; totalTropas++)
    {
        printf("--- Cadastrando território %d ---\n", totalTropas + 1);

        printf("Nome do território: ");
        fgets(mapa[totalTropas].nome, sizeof(mapa[totalTropas].nome), stdin);

        printf("Cor do exército (ex: Azul ou Verde): ");
        fgets(mapa[totalTropas].corExercito, sizeof(mapa[totalTropas].corExercito), stdin);

        printf("Quantidade de Tropas: ");
        scanf("%d", &mapa[totalTropas].tropas);

        mapa[totalTropas].nome[strcspn(mapa[totalTropas].nome, "\n")] = '\0';
        mapa[totalTropas].corExercito[strcspn(mapa[totalTropas].corExercito, "\n")] = '\0';

        limparBufferEntrada();

        printf("\n");
    }

    printf("\nCadastro Inicial Concluído com Sucesso \n\n");
    printf("\nPressione Enter para continuar...\n");
    getchar(); // Aguarda o usuário pressionar Enter

    printf("==================================================================\n");
    printf("                 MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("==================================================================\n");

    for (totalTropas = 0; totalTropas < 5; totalTropas++)
    {

        printf("Território %d:\n", totalTropas + 1);
        printf("  - Nome: %s\n", mapa[totalTropas].nome);
        printf("  - Dominado por: Exercito %s\n", mapa[totalTropas].corExercito);
        printf("  - Tropas: %d\n\n", mapa[totalTropas].tropas);
    }
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):

    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    int opcao; // Declarada fora para ser usada na condição do while

    do
    {
        printf("\n======================================\n");
        printf("           MENU DE ATAQUE\n");
        printf("======================================\n");
        printf("1. Atacar\n");
        printf("0. Sair e Finalizar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            int idxAtq, idxDef;
            printf("\nIndice do atacante (1-5): ");
            scanf("%d", &idxAtq);
            printf("Indice do defensor (1-5): ");
            scanf("%d", &idxDef);

            // Ajuste para índice de array (0-4)
            idxAtq--;
            idxDef--;

            // VALIDAÇÕES TÉCNICAS
            if (idxAtq < 0 || idxAtq >= 5 || idxDef < 0 || idxDef >= 5)
            {
                printf("\n[ERRO] Indices invalidos! Escolha entre 1 e 5.\n");
            }
            else if (strcmp(mapa[idxAtq].corExercito, mapa[idxDef].corExercito) == 0)
            {
                printf("\n[ERRO] Ataque invalido! %s ja pertence ao Exercito %s.\n",
                       mapa[idxDef].nome, mapa[idxAtq].corExercito);
            }
            else if (mapa[idxAtq].tropas < 2)
            {
                printf("\n[ERRO] %s tem apenas %d tropa(s) e nao pode atacar.\n",
                       mapa[idxAtq].nome, mapa[idxAtq].tropas);
            }
            else
            {
                // Executa a lógica de ataque via ponteiros
                atacar(&mapa[idxAtq], &mapa[idxDef]);
            }

            // EXIBIÇÃO PÓS-ATAQUE (Requisito: Mostrar dados atualizados)
            printf("\n--- SITUACAO ATUAL DO MAPA ---\n");
            for (int i = 0; i < 5; i++)
            {
                printf("%d. %s [%s] - Tropas: %d\n",
                       i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
            }

            printf("\nPressione Enter para continuar...");
            limparBufferEntrada(); // Limpa o buffer do scanf anterior
            getchar();             // Pausa para leitura
        }
        else if (opcao != 0)
        {
            printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0); // O loop continua enquanto o jogador não digitar 0

    printf("\nEncerrando batalha e salvando relatorios...\n");

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
