#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Caso M_PI não exista no compilador
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define STACK_SIZE 512
#define MAX_EXPR 512

// ============================
//     Estruturas de Pilha
// ============================

// Pilha de números (float)
typedef struct {
    float items[STACK_SIZE];
    int top;
} PilhaNumeros;

// Pilha de operadores (+ - * / etc)
typedef struct {
    char items[STACK_SIZE];
    int top;
} PilhaOperadores;

// Item da pilha que guarda expressão e precedência
typedef struct {
    char expr[256];
    int prec;
} ItemPilha;

// Pilha de expressões com precedência
typedef struct {
    ItemPilha items[STACK_SIZE];
    int top;
} PilhaExpressoes;


// ============================
//     Funções da Pilha Numérica
// ============================

void pushNum(PilhaNumeros *p, float val) {
    if (p->top < STACK_SIZE - 1) {
        p->items[p->top++] = val;
    }
}

float popNum(PilhaNumeros *p) {
    if (p->top > 0) {
        return p->items[--p->top];
    }
    return 0;
}


// ============================
//     Funções da Pilha Operadores
// ============================

void pushOp(PilhaOperadores *p, char op) {
    if (p->top < STACK_SIZE - 1) {
        p->items[p->top++] = op;
    }
}

char popOp(PilhaOperadores *p) {
    if (p->top > 0) {
        return p->items[--p->top];
    }
    return '\0';
}

char topOp(PilhaOperadores *p) {
    if (p->top > 0) {
        return p->items[p->top - 1];
    }
    return '\0';
}


// ============================
//     Funções da Pilha Expressões
// ============================

void pushExpr(PilhaExpressoes *p, const char *expr, int prec) {
    if (p->top < STACK_SIZE - 1) {
        strcpy(p->items[p->top].expr, expr);
        p->items[p->top].prec = prec;
        p->top++;
    }
}

ItemPilha popExpr(PilhaExpressoes *p) {
    ItemPilha vazio = {"", 0};
    if (p->top > 0) {
        return p->items[--p->top];
    }
    return vazio;
}


// ============================
//          Utilidades
// ============================

int isFuncao(const char *token) {
    return (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
            strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
            strcmp(token, "log") == 0);
}

int precedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

int ehEsquerda(char op) {
    return (op != '^');
}

void removerEspacos(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (!isspace(str[i])) {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}


// ============================
//   Tokenização
// ============================

char** tokenizar(char *expr, int *count) {
    char **tokens = (char**)malloc(512 * sizeof(char*));
    *count = 0;
    int i = 0, j;

    while (i < strlen(expr)) {

        if (isspace(expr[i])) {
            i++;
            continue;
        }

        // Número
        if (isdigit(expr[i]) || expr[i] == '.') {
            j = 0;
            char num[50] = "";
            while (i < strlen(expr) && (isdigit(expr[i]) || expr[i] == '.')) {
                num[j++] = expr[i++];
            }
            num[j] = '\0';

            tokens[*count] = malloc(strlen(num) + 1);
            strcpy(tokens[*count], num);
            (*count)++;
        }

        // Palavra (ex: raiz, sen...)
        else if (isalpha(expr[i])) {
            j = 0;
            char word[50] = "";
            while (i < strlen(expr) && isalpha(expr[i])) {
                word[j++] = expr[i++];
            }
            word[j] = '\0';

            tokens[*count] = malloc(strlen(word) + 1);
            strcpy(tokens[*count], word);
            (*count)++;
        }

        // Operador ou parênteses
        else if (strchr("+-*/%^()", expr[i])) {
            char op[2] = { expr[i], '\0' };
            tokens[*count] = malloc(2);
            strcpy(tokens[*count], op);
            (*count)++;
            i++;
        }

        else {
            i++;
        }
    }

    return tokens;
}


// ============================
//     INFIXA → PÓS-FIXA
// ============================

char* infixaParaPosfixa(char *infixa) {
    int count;
    char **tokens = tokenizar(infixa, &count);

    char *posfixa = malloc(MAX_EXPR);
    strcpy(posfixa, "");

    PilhaOperadores pilha;
    pilha.top = 0;

    for (int i = 0; i < count; i++) {
        char *token = tokens[i];

        // Número
        if (isdigit(token[0]) || (token[0] == '.' && strlen(token) > 1)) {
            strcat(posfixa, token);
            strcat(posfixa, " ");
        }

        // Função
        else if (isFuncao(token)) {
            pushOp(&pilha, 'F');
        }

        // Operador
        else if (strchr("+-*/%^", token[0])) {

            while (pilha.top > 0 && topOp(&pilha) != '(' && topOp(&pilha) != 'F') {
                char op = topOp(&pilha);

                int deve_pop = 0;

                if (ehEsquerda(token[0]))
                    deve_pop = (precedencia(op) >= precedencia(token[0]));
                else
                    deve_pop = (precedencia(op) > precedencia(token[0]));

                if (deve_pop) {
                    char str[3];
                    sprintf(str, "%c ", op);
                    strcat(posfixa, str);
                    popOp(&pilha);
                } else break;
            }

            pushOp(&pilha, token[0]);
        }

        // Abre parênteses
        else if (token[0] == '(')
            pushOp(&pilha, '(');

        // Fecha parênteses
        else if (token[0] == ')') {
            while (pilha.top > 0 && topOp(&pilha) != '(') {

                char op = topOp(&pilha);
                if (op == 'F') { popOp(&pilha); break; }

                char str[3];
                sprintf(str, "%c ", op);
                strcat(posfixa, str);
                popOp(&pilha);
            }

            if (pilha.top > 0 && topOp(&pilha) == '(')
                popOp(&pilha);
        }
    }

    // Desempilha o resto
    while (pilha.top > 0) {
        char op = popOp(&pilha);
        if (op != '(' && op != 'F') {
            char str[3];
            sprintf(str, "%c ", op);
            strcat(posfixa, str);
        }
    }

    // Remove último espaço
    if (strlen(posfixa) > 0 && posfixa[strlen(posfixa) - 1] == ' ')
        posfixa[strlen(posfixa) - 1] = '\0';

    for (int i = 0; i < count; i++) free(tokens[i]);
    free(tokens);

    return posfixa;
}


// ============================
//     Avaliação PÓS-FIXA
// ============================

float avaliarPosfixa(char *posfixa) {
    PilhaNumeros pilha;
    pilha.top = 0;

    char *copia = malloc(strlen(posfixa) + 1);
    strcpy(copia, posfixa);

    char *token = strtok(copia, " ");

    while (token != NULL) {

        // Número
        if (isdigit(token[0]) || (token[0] == '.' && strlen(token) > 1) ||
            (token[0] == '-' && strlen(token) > 1)) {

            pushNum(&pilha, atof(token));
        }

        // Operador binário
        else if (strlen(token) == 1 && strchr("+-*/%^", token[0])) {

            float b = popNum(&pilha);
            float a = popNum(&pilha);
            float resultado = 0;

            switch (token[0]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/': resultado = b != 0 ? a / b : 0; break;
                case '%': resultado = (int)a % (int)b; break;
                case '^': resultado = pow(a, b); break;
            }

            pushNum(&pilha, resultado);
        }

        // Funções matemáticas
        else if (strcmp(token, "raiz") == 0)
            pushNum(&pilha, sqrt(popNum(&pilha)));

        else if (strcmp(token, "sen") == 0)
            pushNum(&pilha, sin(popNum(&pilha) * M_PI / 180));

        else if (strcmp(token, "cos") == 0)
            pushNum(&pilha, cos(popNum(&pilha) * M_PI / 180));

        else if (strcmp(token, "tg") == 0)
            pushNum(&pilha, tan(popNum(&pilha) * M_PI / 180));

        else if (strcmp(token, "log") == 0) {
            float a = popNum(&pilha);
            pushNum(&pilha, a > 0 ? log10(a) : 0);
        }

        token = strtok(NULL, " ");
    }

    free(copia);
    return pilha.top > 0 ? pilha.items[0] : 0;
}


// ============================
//     PÓS-FIXA → INFIXA
//   (COM PARÊNTESES MÍNIMOS)
// ============================

char* posfixaParaInfixa(char *posfixa) {
    
    // Quebra tokens
    char **tokens = malloc(512 * sizeof(char*));
    int count = 0;

    char *copia = malloc(strlen(posfixa) + 1);
    strcpy(copia, posfixa);
    char *token = strtok(copia, " ");

    while (token != NULL && count < 511) {
        tokens[count] = malloc(strlen(token) + 1);
        strcpy(tokens[count], token);
        count++;
        token = strtok(NULL, " ");
    }
    free(copia);

    PilhaExpressoes pilha;
    pilha.top = 0;

    for (int i = 0; i < count; i++) {

        // Número
        if (isdigit(tokens[i][0]) || (tokens[i][0] == '.' && strlen(tokens[i]) > 1)) {
            pushExpr(&pilha, tokens[i], 999); // Precedência máxima
        }

        // Operador binário
        else if (strlen(tokens[i]) == 1 && strchr("+-*/%^", tokens[i][0])) {

            ItemPilha b = popExpr(&pilha);
            ItemPilha a = popExpr(&pilha);

            int precAtual = precedencia(tokens[i][0]);
            char resultado[256] = "";
            char temp[256];

            // Operando esquerdo precisa parênteses?
            int parentEsq = 0;
            if (a.prec < precAtual) {
                parentEsq = 1;
            }
            // Para subtração e divisão: a - (b+c) precisa parênteses
            if (a.prec == precAtual && !ehEsquerda(tokens[i][0])) {
                parentEsq = 0; // Associatividade à direita (^)
            }

            // Operando direito precisa parênteses?
            int parentDir = 0;
            if (b.prec < precAtual) {
                parentDir = 1;
            }
            // Para operadores associativos à esquerda
            if (b.prec == precAtual && ehEsquerda(tokens[i][0])) {
                parentDir = 1;
            }
            // Para subtração: a - (b-c) ou a - (b+c)
            if (b.prec <= precAtual && (tokens[i][0] == '-' || tokens[i][0] == '/')) {
                if (b.prec == 1 && tokens[i][0] == '-') parentDir = 1;
                if (b.prec <= 2 && tokens[i][0] == '/') parentDir = 1;
            }

            // Monta a expressão
            if (parentEsq) {
                sprintf(resultado, "(%s)", a.expr);
            } else {
                strcpy(resultado, a.expr);
            }

            sprintf(temp, "%s%c", resultado, tokens[i][0]);
            strcpy(resultado, temp);

            if (parentDir) {
                sprintf(temp, "%s(%s)", resultado, b.expr);
            } else {
                sprintf(temp, "%s%s", resultado, b.expr);
            }

            pushExpr(&pilha, temp, precAtual);
        }

        // Função
        else if (isFuncao(tokens[i])) {
            ItemPilha a = popExpr(&pilha);
            char resultado[256];
            sprintf(resultado, "%s(%s)", tokens[i], a.expr);
            pushExpr(&pilha, resultado, 999); // Funções têm precedência máxima
        }
    }

    char *resultado = malloc(MAX_EXPR);
    if (pilha.top > 0) {
        strcpy(resultado, pilha.items[0].expr);
    } else {
        strcpy(resultado, "");
    }

    for (int i = 0; i < count; i++) free(tokens[i]);
    free(tokens);

    return resultado;
}


// ============================
//        Funções públicas
// ============================

char* getFormaInFixa(char *Str) {
    if (Str == NULL || strlen(Str) == 0) return NULL;
    
    char *resultado = posfixaParaInfixa(Str);
    if (resultado == NULL) return NULL;
    
    // Remove todos os espaços
    removerEspacos(resultado);
    
    return resultado;
}

float getValorPosFixa(char *StrPosFixa) {
    if (StrPosFixa == NULL || strlen(StrPosFixa) == 0) return 0;
    return avaliarPosfixa(StrPosFixa);
}