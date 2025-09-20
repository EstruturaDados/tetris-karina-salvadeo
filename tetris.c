// ============================================================================
//         Desafio Tetris
// ============================================================================
// Karina Salvadeo Pereira - Engenharia de Software (Estrutura de Dados)       
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição do tamanho fixo da fila circular
#define TAMANHO_FILA 5

// Struct que representa uma peça do Tetris
typedef struct {
    int id;          // Identificador único da peça
    char nome[2];    // Nome da peça (I, O, T, L etc.)
} Peca;

// Variáveis globais da fila circular
Peca fila[TAMANHO_FILA]; // Array que armazena as peças
int frente = 0;          // Índice do primeiro elemento da fila
int tras = 0;            // Índice da posição de inserção
int quantidade = 0;      // Quantidade atual de peças na fila
int contadorID = 1;      // Contador para gerar IDs únicos

// Função que gera automaticamente uma nova peça
Peca gerarPeca() {
    Peca nova;
    nova.id = contadorID++; // ID incremental
    // Lista de tipos possíveis de peça
    char tipos[4][2] = {"I", "O", "T", "L"};
    // Sorteia uma peça
    int sorteio = rand() % 4;
    strcpy(nova.nome, tipos[sorteio]);
    return nova;
}

// Insere uma nova peça no final da fila (enqueue)
void enqueue(Peca p) {
    if (quantidade == TAMANHO_FILA) {
        printf("⚠️ Fila cheia! Não é possível inserir.\n");
        return;
    }
    fila[tras] = p;                // Coloca a peça na posição 'tras'
    tras = (tras + 1) % TAMANHO_FILA; // Avança circularmente
    quantidade++;                  // Incrementa a quantidade
}

// Remove a peça da frente da fila (dequeue)
Peca dequeue() {
    Peca removida = {"", -1}; // Peça "vazia" para retorno
    if (quantidade == 0) {
        printf("⚠️ Fila vazia! Nada a remover.\n");
        return removida;
    }
    removida = fila[frente];       // Pega a peça da frente
    frente = (frente + 1) % TAMANHO_FILA; // Avança circularmente
    quantidade--;                  // Decrementa a quantidade
    return removida;
}

// Exibe o estado atual da fila
void mostrarFila() {
    printf("\n=== Estado atual da Fila ===\n");
    if (quantidade == 0) {
        printf("Fila vazia!\n");
        return;
    }
    int i, pos;
    for (i = 0; i < quantidade; i++) {
        pos = (frente + i) % TAMANHO_FILA; // Calcula posição circular
        printf("Pos %d -> ID:%d | Peca:%s\n", i, fila[pos].id, fila[pos].nome);
    }
    printf("===========================\n\n");
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(gerarPeca());
    }

    int opcao;
    do {
        printf("🎮 Menu - Fila de Pecas Futuras\n");
        printf("1. Visualizar fila\n");
        printf("2. Jogar (remover) peca\n");
        printf("3. Inserir nova peca\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                mostrarFila();
                break;
            case 2: {
                Peca jogada = dequeue();
                if (jogada.id != -1) {
                    printf("✅ Jogou a peca ID:%d (%s)\n", jogada.id, jogada.nome);
                }
                mostrarFila();
                break;
            }
            case 3: {
                Peca nova = gerarPeca();
                enqueue(nova);
                printf("✅ Inseriu nova peca ID:%d (%s)\n", nova.id, nova.nome);
                mostrarFila();
                break;
            }
            case 0:
                printf("👋 Encerrando programa...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

