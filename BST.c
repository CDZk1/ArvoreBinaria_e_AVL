#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *esquerda;
    struct No *direita;
} No;

/*
 * Valores repetidos são ignorados.
 * A árvore mantém apenas uma ocorrência de cada valor.
 */

No *criarNo(int valor) {
    No *novo = (No *)malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }

    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

No *inserir(No *raiz, int valor) {
    if (raiz == NULL) {
        return criarNo(valor);
    }

    if (valor < raiz->valor) {
        raiz->esquerda = inserir(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserir(raiz->direita, valor);
    } else {
        printf("Valor repetido. Insercao ignorada.\n");
    }

    return raiz;
}

No *buscar(No *raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }

    if (valor < raiz->valor) {
        return buscar(raiz->esquerda, valor);
    }

    return buscar(raiz->direita, valor);
}

No *encontrarMenor(No *raiz) {
    No *atual = raiz;

    while (atual != NULL && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }

    return atual;
}

No *remover(No *raiz, int valor) {
    if (raiz == NULL) {
        return NULL;
    }

    if (valor < raiz->valor) {
        raiz->esquerda = remover(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = remover(raiz->direita, valor);
    } else {
        /* Caso 1: no folha */
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }

        /* Caso 2: no com apenas filho direito */
        if (raiz->esquerda == NULL) {
            No *temp = raiz->direita;
            free(raiz);
            return temp;
        }

        /* Caso 2: no com apenas filho esquerdo */
        if (raiz->direita == NULL) {
            No *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        /*
         * Caso 3: no com dois filhos.
         * Substitui pelo sucessor in-order:
         * menor elemento da subarvore direita.
         */
        No *temp = encontrarMenor(raiz->direita);
        raiz->valor = temp->valor;
        raiz->direita = remover(raiz->direita, temp->valor);
    }

    return raiz;
}

void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        emOrdem(raiz->direita);
    }
}

void posOrdem(No *raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esquerda);
        posOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

void menuPercursos(No *raiz) {
    int opcao;

    do {
        printf("\n");
        printf("1\n");
        printf("Pre-ordem\n");
        printf("2\n");
        printf("Em ordem\n");
        printf("3\n");
        printf("Pos-ordem\n");
        printf("0\n");
        printf("Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Pre-ordem: ");
                preOrdem(raiz);
                printf("\n");
                break;

            case 2:
                printf("Em ordem: ");
                emOrdem(raiz);
                printf("\n");
                break;

            case 3:
                printf("Pos-ordem: ");
                posOrdem(raiz);
                printf("\n");
                break;

            case 0:
                break;

            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

int main(void) {
    No *raiz = NULL;
    int opcao;
    int valor;

    do {
        printf("\n");
        printf("1\n");
        printf("Inserir valor\n");
        printf("2\n");
        printf("Buscar valor\n");
        printf("3\n");
        printf("Remover valor\n");
        printf("4\n");
        printf("Percorrer arvore\n");
        printf("0\n");
        printf("Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor: ");
                scanf("%d", &valor);

                raiz = inserir(raiz, valor);
                break;

            case 2:
                printf("Digite o valor: ");
                scanf("%d", &valor);

                if (buscar(raiz, valor) != NULL) {
                    printf("Valor encontrado na arvore.\n");
                } else {
                    printf("Valor nao encontrado na arvore.\n");
                }
                break;

            case 3:
                printf("Digite o valor: ");
                scanf("%d", &valor);

                if (buscar(raiz, valor) != NULL) {
                    raiz = remover(raiz, valor);
                    printf("Valor removido com sucesso.\n");
                } else {
                    printf("Valor nao encontrado na arvore.\n");
                }
                break;

            case 4:
                menuPercursos(raiz);
                break;

            case 0:
                liberarArvore(raiz);
                raiz = NULL;
                printf("Memoria liberada. Programa encerrado.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}