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
    int id;          
    char nome[2];    
} Peca;

// ===================== VARIÁVEIS GLOBAIS ===================== //

// Fila circular
Peca fila[TAMANHO_FILA]; 
int frente = 0;          
int tras = 0;            
int quantidade = 0;      

// Pilha linear
Peca pilha[TAMANHO_PILHA];
int topo = -1; 

// Controle de IDs
int contadorID = 1;      

// Histórico para desfazer última jogada
Peca ultimaJogada = {"", -1}; // Guarda a última peça jogada

// ===================== FUNÇÕES AUXILIARES ===================== //

// Gera automaticamente uma nova peça
Peca gerarPeca() {
    Peca nova;
    nova.id = contadorID++;
    char tipos[4][2] = {"I", "O", "T", "L"};
    int sorteio = rand() % 4;
    strcpy(nova.nome, tipos[sorteio]);
    return nova;
}

// ========== OPERAÇÕES DA FILA ========== //
void enqueue(Peca p) {
    if (quantidade == TAMANHO_FILA) {
        printf("⚠️ Fila cheia! Não é possível inserir.\n");
        return;
    }
    fila[tras] = p;
    tras = (tras + 1) % TAMANHO_FILA;
    quantidade++;
}

Peca dequeue() {
    Peca removida = {"", -1};
    if (quantidade == 0) {
        printf("⚠️ Fila vazia!\n");
        return removida;
    }
    removida = fila[frente];
    frente = (frente + 1) % TAMANHO_FILA;
    quantidade--;
    return removida;
}

void mostrarFila() {
    printf("\n=== Fila de Pecas Futuras ===\n");
    if (quantidade == 0) {
        printf("Fila vazia!\n");
    } else {
        for (int i = 0; i < quantidade; i++) {
            int pos = (frente + i) % TAMANHO_FILA;
            printf("Pos %d -> ID:%d | Peca:%s\n", i, fila[pos].id, fila[pos].nome);
        }
    }
    printf("=============================\n");
}

// ========== OPERAÇÕES DA PILHA ========== //
void push(Peca p) {
    if (topo == TAMANHO_PILHA - 1) {
        printf("⚠️ Pilha cheia! Não é possível reservar.\n");
        return;
    }
    pilha[++topo] = p;
}

Peca pop() {
    Peca removida = {"", -1};
    if (topo == -1) {
        printf("⚠️ Pilha vazia!\n");
        return removida;
    }
    removida = pilha[topo--];
    return removida;
}

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

// ========== OPERAÇÕES DO NÍVEL MESTRE ========== //

// Troca a peça do topo da pilha com a da frente da fila
void trocar() {
    if (topo == -1 || quantidade == 0) {
        printf("⚠️ Não é possível trocar (pilha ou fila vazia).\n");
        return;
    }
    Peca temp = pilha[topo];
    pilha[topo] = fila[frente];
    fila[frente] = temp;
    printf("✅ Troca realizada entre fila e pilha.\n");
}

// Desfaz a última jogada (reinsere peça na frente da fila)
void desfazer() {
    if (ultimaJogada.id == -1) {
        printf("⚠️ Nenhuma jogada para desfazer.\n");
        return;
    }
    // Desloca os elementos da fila para trás
    if (quantidade == TAMANHO_FILA) {
        printf("⚠️ Não é possível desfazer, fila cheia.\n");
        return;
    }
    frente = (frente - 1 + TAMANHO_FILA) % TAMANHO_FILA;
    fila[frente] = ultimaJogada;
    quantidade++;
    printf("✅ Desfez a última jogada! Peça ID:%d (%s) retornou.\n", ultimaJogada.id, ultimaJogada.nome);
    ultimaJogada.id = -1; // Limpa histórico
}

// Inverte todo o conteúdo da fila com a pilha
void inverter() {
    if (quantidade == 0 && topo == -1) {
        printf("⚠️ Nada a inverter, ambas vazias.\n");
        return;
    }

    // Copiar fila atual para pilha temporária
    Peca pilhaTemp[TAMANHO_FILA];
    int topoTemp = -1;
    for (int i = 0; i < quantidade; i++) {
        int pos = (frente + i) % TAMANHO_FILA;
        pilhaTemp[++topoTemp] = fila[pos];
    }

    // Copiar pilha atual para fila
    int qtdAntigaFila = quantidade;
    frente = 0;
    tras = 0;
    quantidade = 0;
    for (int i = topo; i >= 0; i--) {
        enqueue(pilha[i]);
    }

    // Copiar fila antiga (temp) para pilha
    topo = -1;
    for (int i = topoTemp; i >= 0; i--) {
        push(pilhaTemp[i]);
    }

    printf("✅ Fila e pilha foram invertidas!\n");
}

// ===================== PROGRAMA PRINCIPAL ===================== //
int main() {
    srand(time(NULL));

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(gerarPeca());
    }

    int opcao;
    do {
        printf("\n🏆 Menu - Nivel Mestre\n");
        printf("1. Jogar peca\n");
        printf("2. Reservar peca\n");
        printf("3. Usar peca reservada\n");
        printf("4. Trocar fila <-> pilha\n");
        printf("5. Desfazer ultima jogada\n");
        printf("6. Inverter fila com pilha\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue();
                if (jogada.id != -1) {
                    ultimaJogada = jogada; // Guarda para possível desfazer
                    printf("✅ Jogou a peca ID:%d (%s)\n", jogada.id, jogada.nome);
                    enqueue(gerarPeca()); // Reposição automática
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 2: {
                Peca reservada = dequeue();
                if (reservada.id != -1) {
                    push(reservada);
                    printf("✅ Reservou a peca ID:%d (%s)\n", reservada.id, reservada.nome);
                    enqueue(gerarPeca());
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 3: {
                Peca usada = pop();
                if (usada.id != -1) {
                    ultimaJogada = usada; // Também conta como jogada
                    printf("✅ Usou a peca reservada ID:%d (%s)\n", usada.id, usada.nome);
                }
                mostrarFila();
                mostrarPilha();
                break;
            }
            case 4:
                trocar();
                mostrarFila();
                mostrarPilha();
                break;
            case 5:
                desfazer();
                mostrarFila();
                mostrarPilha();
                break;
            case 6:
                inverter();
                mostrarFila();
                mostrarPilha();
                break;
            case 0:
                printf("👋 Encerrando programa...\n");
                break;
            default:
                printf("⚠️ Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}
