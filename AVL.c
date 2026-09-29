#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    int altura;
    struct No *esquerda;
    struct No *direita;
} No;

/*
 * Valores repetidos são ignorados.
 * A arvore mantem apenas uma ocorrencia de cada valor.
 */

int maior(int a, int b) {
    return (a > b) ? a : b;
}

int altura(No *no) {
    if (no == NULL) {
        return 0;
    }

    return no->altura;
}

int fatorBalanceamento(No *no) {
    if (no == NULL) {
        return 0;
    }

    return altura(no->esquerda) - altura(no->direita);
}

No *criarNo(int valor) {
    No *novo = (No *)malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(EXIT_FAILURE);
    }

    novo->valor = valor;
    novo->altura = 1;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

void atualizarAltura(No *no) {
    if (no != NULL) {
        no->altura = 1 + maior(altura(no->esquerda),
                                altura(no->direita));
    }
}

/*
 * Rotacao simples para a direita.
 *
 *        y                 x
 *       / \               / \
 *      x   T3     ->      T1  y
 *     / \                   / \
 *    T1 T2                 T2 T3
 */
No *rotacaoDireita(No *y) {
    No *x = y->esquerda;
    No *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

/*
 * Rotacao simples para a esquerda.
 *
 *      x                     y
 *     / \                   / \
 *    T1  y       ->        x  T3
 *       / \               / \
 *      T2 T3             T1 T2
 */
No *rotacaoEsquerda(No *x) {
    No *y = x->direita;
    No *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

/*
 * Rotacao dupla esquerda-direita:
 * rotacao para a esquerda no filho esquerdo,
 * seguida de rotacao para a direita no proprio no.
 */
No *rotacaoEsquerdaDireita(No *no) {
    no->esquerda = rotacaoEsquerda(no->esquerda);
    return rotacaoDireita(no);
}

/*
 * Rotacao dupla direita-esquerda:
 * rotacao para a direita no filho direito,
 * seguida de rotacao para a esquerda no proprio no.
 */
No *rotacaoDireitaEsquerda(No *no) {
    no->direita = rotacaoDireita(no->direita);
    return rotacaoEsquerda(no);
}

No *rebalancear(No *no) {
    if (no == NULL) {
        return NULL;
    }

    atualizarAltura(no);

    int fator = fatorBalanceamento(no);

    /*
     * Caso esquerda-esquerda (LL)
     */
    if (fator > 1 && fatorBalanceamento(no->esquerda) >= 0) {
        return rotacaoDireita(no);
    }

    /*
     * Caso esquerda-direita (LR)
     */
    if (fator > 1 && fatorBalanceamento(no->esquerda) < 0) {
        return rotacaoEsquerdaDireita(no);
    }

    /*
     * Caso direita-direita (RR)
     */
    if (fator < -1 && fatorBalanceamento(no->direita) <= 0) {
        return rotacaoEsquerda(no);
    }

    /*
     * Caso direita-esquerda (RL)
     */
    if (fator < -1 && fatorBalanceamento(no->direita) > 0) {
        return rotacaoDireitaEsquerda(no);
    }

    return no;
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
        return raiz;
    }

    return rebalancear(raiz);
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
        /*
         * Caso 1: no folha.
         */
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }

        /*
         * Caso 2: apenas filho direito.
         */
        if (raiz->esquerda == NULL) {
            No *temp = raiz->direita;
            free(raiz);
            return temp;
        }

        /*
         * Caso 2: apenas filho esquerdo.
         */
        if (raiz->direita == NULL) {
            No *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        /*
         * Caso 3: dois filhos.
         * Usa o sucessor in-order.
         */
        No *temp = encontrarMenor(raiz->direita);

        raiz->valor = temp->valor;
        raiz->direita = remover(raiz->direita, temp->valor);
    }

    /*
     * Ao retornar pela recursao, a altura e atualizada
     * e a arvore e rebalanceada ate a raiz.
     */
    return rebalancear(raiz);
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

void exibirBalanceamento(No *raiz) {
    if (raiz != NULL) {
        exibirBalanceamento(raiz->esquerda);

        printf("No %d | Altura: %d | Fator de balanceamento: %d\n",
               raiz->valor,
               raiz->altura,
               fatorBalanceamento(raiz));

        exibirBalanceamento(raiz->direita);
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
        printf("5\n");
        printf("Exibir altura e fator de balanceamento\n");
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

            case 5:
                printf("\nAltura da arvore: %d\n", altura(raiz));
                printf("Altura e fator de balanceamento dos nos:\n");

                if (raiz == NULL) {
                    printf("Arvore vazia.\n");
                } else {
                    exibirBalanceamento(raiz);
                }
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