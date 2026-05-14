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
struct Territorio* alocarMapa(int quantidade);
void liberarMemoria(struct Territorio *mapa);

// Funções de interface com o usuário:
void exibirMapa(const struct Territorio *mapa, int quantidade);
void situacaoAtualMapa(const struct Territorio *mapa, int quantidade);
void exibirMenuPrincipal();

// Funções de lógica principal do jogo:
void inicializarTerritorios(struct Territorio *mapa, int quantidade);
void faseDeAtaque(struct Territorio *mapa, int total);

// Função que simula a batalha entre dois territórios
void atacar(struct Territorio *atacante, struct Territorio *defensor);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main()
{
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    struct Territorio *mapa = NULL;

    srand(time(NULL));

    mapa = alocarMapa(5);

    inicializarTerritorios(mapa, 5);

    exibirMapa(mapa, 5);

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
        exibirMenuPrincipal();
        if (scanf("%d", &opcao) != 1)
        {
            limparBufferEntrada();
            opcao = -1;
        }
        limparBufferEntrada(); // Limpa o rastro do Enter do scanf da opção

        switch (opcao)
        {
        case 1:
            faseDeAtaque(mapa, 5);
            situacaoAtualMapa(mapa, 5); // Mostra o mapa atualizado após o ataque
            break;
        case 0:
            printf("\nSaindo do War...\n");
            break;
        default:
            printf("\nOpção inválida!\n");
        }
    } while (opcao != 0);

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
struct Territorio* alocarMapa(int quantidade) {
    struct Territorio *mapa = (struct Territorio *)calloc(quantidade, sizeof(struct Territorio));
    
    if (mapa == NULL) {
        printf("\n[ERRO CRÍTICO] Falha ao alocar memória para o mapa!\n");
        exit(1); // Encerra o programa se não houver memória
    }
    
    return mapa;
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(struct Territorio *mapa, int quantidade)
{
    printf("==================================================================\n\n");
    printf("--- Vamos cadastrar os %d territorios iniciais do nosso mundo. ---\n\n", quantidade);

    for (int i = 0; i < quantidade; i++)
    {
        printf("--- Cadastrando território %d ---\n", i + 1);

        printf("Nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // Limpa o \n

        printf("Cor do exército (ex: Azul ou Verde): ");
        fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0'; // Limpa o \n

        printf("Quantidade de Tropas: ");
        scanf("%d", &mapa[i].tropas);

        limparBufferEntrada(); // Limpa o buffer para a próxima iteração do fgets
        printf("\n");
    }
    printf("\nCadastro de Territorios Concluído com Sucesso \n\n");
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(struct Territorio *mapa)
{
    if (mapa != NULL)
    {
        free(mapa);
        printf("\nMemoria do mapa liberada.\n");
    }
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal()
{
    printf("\n======================================\n");
    printf("           WAR - MENU DE AÇÕES\n");
    printf("======================================\n");
    printf("1. Iniciar Fase de Ataque\n");
    printf("0. Sair e Finalizar Jogo\n");
    printf("--------------------------------------\n");
    printf("Escolha sua ação: ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const struct Territorio *mapa, int quantidade)
{
    printf("==================================================================\n");
    printf("                 MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("==================================================================\n");

    for (int i = 0; i < quantidade; i++)
    {
        printf("Território %d:\n", i + 1);
        printf("  - Nome: %s\n", mapa[i].nome);
        printf("  - Dominado por: Exercito %s\n", mapa[i].corExercito);
        printf("  - Tropas: %d\n\n", mapa[i].tropas);
    }
}

void situacaoAtualMapa(const struct Territorio *mapa, int quantidade)
{
    printf("==================================================================\n");
    printf("                 SITUAÇÃO ATUAL DO MAPA\n");
    printf("==================================================================\n");
    for (int i = 0; i < quantidade; i++)
    {
        printf("%d. %s (Exercito: %s - Tropas: %d)\n", i + 1, mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(struct Territorio *mapa, int total)
{
    int idxAtq, idxDef;

    printf("\n--- FASE DE ATAQUE ---\n");

    printf("Escolha o território Atacante (1-%d): ", total);
    scanf("%d", &idxAtq);
    if (idxAtq == 0)
        return; // Permite ao jogador voltar ao menu principal

    printf("Escolha o território Defensor (1-%d): ", total);
    scanf("%d", &idxDef);
    if (idxDef == 0)
        return;

    // Ajuste para índices do array (0 a n-1)
    idxAtq--;
    idxDef--;

    // Validações que agora ficam organizadas aqui dentro
    if (idxAtq < 0 || idxAtq >= total || idxDef < 0 || idxDef >= total)
    {
        printf("\n[ERRO] Território inexistente! Use números entre 1 e %d.\n", total);
    }
    else if (strcmp(mapa[idxAtq].corExercito, mapa[idxDef].corExercito) == 0)
    {
        printf("\n[ERRO] %s e %s são do mesmo exército!\n", mapa[idxAtq].nome, mapa[idxDef].nome);
    }
    else if (mapa[idxAtq].tropas < 2)
    {
        printf("\n[AVISO] %s precisa de pelo menos 2 tropas para atacar.\n", mapa[idxAtq].nome);
    }
    else
    {
        // Chama a subfunção que cuida dos dados
        atacar(&mapa[idxAtq], &mapa[idxDef]);
    }
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
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
                printf("%s foi conquistado pelo Exercito %s!\n", defensor->nome, atacante->corExercito);
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

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}