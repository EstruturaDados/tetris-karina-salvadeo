// ============================================================================
//         Desafio Tetris
// ============================================================================
// Karina Salvadeo Pereira - Engenharia de Software (Estrutura de Dados)       
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ===================== CONSTANTES ===================== //
#define TAMANHO_FILA 5   // Capacidade fixa da fila circular
#define TAMANHO_PILHA 3  // Capacidade máxima da pilha de reserva

// ===================== ESTRUTURAS ===================== //

// Struct que representa uma peça do Tetris
typedef struct {
    int id;          // Identificador único da peça
    char nome[2];    // Nome da peça (I, O, T, L etc.)
} Peca;

// ===================== VARIÁVEIS GLOBAIS ===================== //

// Fila circular
Peca fila[TAMANHO_FILA]; 
int frente = 0;          
int tras = 0;            
int quantidade = 0;      

// Pilha linear
Peca pilha[TAMANHO_PILHA];
int topo = -1; // topo da pilha (-1 significa vazia)

// Controle de IDs
int contadorID = 1;      

// ===================== FUNÇÕES AUXILIARES ===================== //

// Função que gera automaticamente uma nova peça
Peca gerarPeca() {
    Peca nova;
    nova.id = contadorID++; // ID incremental único
    char tipos[4][2] = {"I", "O", "T", "L"}; // Tipos possíveis
    int sorteio = rand() % 4; // Sorteia um índice
    strcpy(nova.nome, tipos[sorteio]); // Copia o nome
    return nova;
}

// ========== OPERAÇÕES DA FILA ========== //

// Insere nova peça no final da fila (enqueue)
void enqueue(Peca p) {
    if (quantidade == TAMANHO_FILA) {
        printf("⚠️ Fila cheia! Não é possível inserir.\n");
        return;
    }
    fila[tras] = p;                 // Coloca a peça no índice 'tras'
    tras = (tras + 1) % TAMANHO_FILA; // Avança circularmente
    quantidade++;                   // Incrementa a quantidade
}

// Remove a peça da frente da fila (dequeue)
Peca dequeue() {
    Peca removida = {"", -1}; // Peça inválida padrão
    if (quantidade == 0) {
        printf("⚠️ Fila vazia! Nada a remover.\n");
        return removida;
    }
    removida = fila[frente];         // Pega a peça da frente
    frente = (frente + 1) % TAMANHO_FILA; // Avança circularmente
    quantidade--;                    // Decrementa
    return removida;
}

// Mostra o estado atual da fila
void mostrarFila() {
    printf("\n=== Fila de Pecas Futuras ===\n");
    if (quantidade == 0) {
        printf("Fila vazia!\n");
        return;
    }
    for (int i = 0; i < quantidade; i++) {
        int pos = (frente + i) % TAMANHO_FILA;
        printf("Pos %d -> ID:%d | Peca:%s\n", i, fila[pos].id, fila[pos].nome);
    }
    printf("=============================\n");
}

// ========== OPERAÇÕES DA PILHA ========== //

// Empilha (push) uma peça na pilha
void push(Peca p) {
    if (topo == TAMANHO_PILHA - 1) {
        printf("⚠️ Pilha cheia! Não é possível reservar.\n");
        return;
    }
    pilha[++topo] = p; // Avança o topo e insere
}

// Desempilha (pop) uma peça da pilha
Peca pop() {
    Peca removida = {"", -1}; // Peça inválida padrão
    if (topo == -1) {
        printf("⚠️ Pilha vazia! Nada a usar.\n");
        return removida;
    }
    removida = pilha[topo--]; // Retorna e reduz topo
    return removida;
}

// Mostra o estado da pilha
void mostrarPilha() {
    printf("\n=== Pilha de Reservas ===\n");
    if (topo == -1) {
        printf("Pilha vazia!\n");
    } else {
        for (int i = topo; i >= 0; i--) {
            printf("Topo[%d] -> ID:%d | Peca:%s\n", i, pilha[i].id, pilha[i].nome);
        }
    }
    printf("=========================\n\n");
}

// ===================== PROGRAMA PRINCIPAL ===================== //

int main() {
    srand(time(NULL)); // Inicializa sorteio aleatório

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(gerarPeca());
    }

    int opcao;
    do {
        printf("\n🎮 Menu - Nivel Aventureiro\n");
        printf("1. Jogar peca\n");
        printf("2. Reservar peca\n");
        printf("3. Usar peca reservada\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça (tira da fila e insere nova)
                Peca jogada = dequeue();
                if (jogada.id != -1) {
                    printf("✅ Jogou a peca ID:%d (%s)\n", jogada.id, jogada.nome);
                    enqueue(gerarPeca()); // Mantém a fila cheia
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 2: { // Reservar peça (move da fila para pilha)
                Peca reservada = dequeue();
                if (reservada.id != -1) {
                    push(reservada);
                    printf("✅ Reservou a peca ID:%d (%s)\n", reservada.id, reservada.nome);
                    enqueue(gerarPeca()); // Mantém a fila cheia
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 3: { // Usar peça da pilha (pop)
                Peca usada = pop();
                if (usada.id != -1) {
                    printf("✅ Usou a peca reservada ID:%d (%s)\n", usada.id, usada.nome);
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 0:
                printf("👋 Encerrando programa...\n");
                break;
            default:
                printf("⚠️ Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
