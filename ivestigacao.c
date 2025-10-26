#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_TABELA 10

// Estrutura para cada nó da lista encadeada da tabela hash
typedef struct No {
    char pista[50];
    char suspeito[50];
    struct No *prox;
} No;

// Vetor de ponteiros (tabela hash)
No *tabelaHash[TAM_TABELA];

// Função hash simples — soma dos caracteres da pista mod TAM_TABELA
int funcaoHash(char *pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        soma += pista[i];
    }
    return soma % TAM_TABELA;
}

// Função para inserir uma relação pista → suspeito
void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);

    // Cria novo nó
    No *novo = (No *)malloc(sizeof(No));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = NULL;

    // Encadeamento simples em caso de colisão
    if (tabelaHash[indice] == NULL) {
        tabelaHash[indice] = novo;
    } else {
        No *atual = tabelaHash[indice];
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
}

// Função para exibir toda a tabela hash
void exibirTabela() {
    printf("\n=== Pistas e Suspeitos ===\n");
    for (int i = 0; i < TAM_TABELA; i++) {
        No *atual = tabelaHash[i];
        if (atual != NULL) {
            printf("\n[%d] -> ", i);
            while (atual != NULL) {
                printf("Pista: '%s' => Suspeito: '%s' | ", atual->pista, atual->suspeito);
                atual = atual->prox;
            }
            printf("\n");
        }
    }
}

// Função para contar quantas vezes cada suspeito aparece
void suspeitoMaisFrequente() {
    typedef struct {
        char nome[50];
        int contagem;
    } Contador;

    Contador contadores[50];
    int total = 0;

    // Percorrer tabela e contar ocorrências
    for (int i = 0; i < TAM_TABELA; i++) {
        No *atual = tabelaHash[i];
        while (atual != NULL) {
            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(contadores[j].nome, atual->suspeito) == 0) {
                    contadores[j].contagem++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(contadores[total].nome, atual->suspeito);
                contadores[total].contagem = 1;
                total++;
            }
            atual = atual->prox;
        }
    }

    // Determinar o mais suspeito
    int max = 0;
    char suspeitoTop[50] = "";
    for (int i = 0; i < total; i++) {
        if (contadores[i].contagem > max) {
            max = contadores[i].contagem;
            strcpy(suspeitoTop, contadores[i].nome);
        }
    }

    if (max > 0) {
        printf("\n🏁 O suspeito mais relatado é: **%s** com %d pistas associadas!\n", suspeitoTop, max);
    } else {
        printf("\n⚠️ Nenhum suspeito registrado ainda!\n");
    }
}

// Limpar memória alocada
void liberarMemoria() {
    for (int i = 0; i < TAM_TABELA; i++) {
        No *atual = tabelaHash[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

int main() {
    int opcao;
    char pista[50], suspeito[50];

    // Inicializa a tabela hash
    for (int i = 0; i < TAM_TABELA; i++) {
        tabelaHash[i] = NULL;
    }

    do {
        printf("\n=== MENU INVESTIGAÇÃO ===\n");
        printf("1 - Inserir relação Pista → Suspeito\n");
        printf("2 - Exibir tabela de pistas e suspeitos\n");
        printf("3 - Mostrar suspeito mais relatado\n");
        printf("0 - Encerrar investigação\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("\nDigite a pista: ");
                fgets(pista, sizeof(pista), stdin);
                pista[strcspn(pista, "\n")] = '\0';

                printf("Digite o suspeito associado: ");
                fgets(suspeito, sizeof(suspeito), stdin);
                suspeito[strcspn(suspeito, "\n")] = '\0';

                inserirNaHash(pista, suspeito);
                printf("✅ Relação registrada com sucesso!\n");
                break;

            case 2:
                exibirTabela();
                break;

            case 3:
                suspeitoMaisFrequente();
                break;

            case 0:
                printf("\nEncerrando investigação... 🕵️‍♀️\n");
                liberarMemoria();
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
