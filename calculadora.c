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
    int top;        // Índice do topo
} PilhaNumeros;

// Pilha de operadores (+ - * / etc)
typedef struct {
    char items[STACK_SIZE];
    int top;
} PilhaOperadores;

// Pilha de strings (usada para converter pós-fixa → infixa)
typedef struct {
    char items[STACK_SIZE][100];
    int top;
} PilhaStrings;


// ============================
//     Funções da Pilha Numérica
// ============================

// Empilha um número
void pushNum(PilhaNumeros *p, float val) {
    if (p->top < STACK_SIZE - 1) {
        p->items[p->top++] = val;
    }
}

// Desempilha um número
float popNum(PilhaNumeros *p) {
    if (p->top > 0) {
        return p->items[--p->top];
    }
    return 0; // Retorno padrão
}


// ============================
//     Funções da Pilha Operadores
// ============================

// Empilha operador
void pushOp(PilhaOperadores *p, char op) {
    if (p->top < STACK_SIZE - 1) {
        p->items[p->top++] = op;
    }
}

// Desempilha operador
char popOp(PilhaOperadores *p) {
    if (p->top > 0) {
        return p->items[--p->top];
    }
    return '\0';
}

// Retorna operador do topo sem remover
char topOp(PilhaOperadores *p) {
    if (p->top > 0) {
        return p->items[p->top - 1];
    }
    return '\0';
}


// ============================
//     Funções da Pilha Strings
// ============================

// Empilha string
void pushStr(PilhaStrings *p, const char *str) {
    if (p->top < STACK_SIZE - 1) {
        strcpy(p->items[p->top], str);
        p->top++;
    }
}

// Desempilha string
char* popStr(PilhaStrings *p) {
    if (p->top > 0) {
        return p->items[--p->top];
    }
    return "";
}


// ============================
//          Utilidades
// ============================

// Verifica se o token é uma função (sen, cos, log...)
int isFuncao(const char *token) {
    return (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
            strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
            strcmp(token, "log") == 0);
}

// Retorna a precedência do operador
int precedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

// Define se um operador é associativo à esquerda
int ehEsquerda(char op) {
    return (op != '^'); // ^ é associativo à direita
}


// ============================
//   Tokenização (quebra expressão em partes)
// ============================

// Converte string em lista de tokens (números, operadores, palavras...)
char** tokenizar(char *expr, int *count) {
    char **tokens = (char**)malloc(512 * sizeof(char*));
    *count = 0;
    int i = 0, j;

    while (i < strlen(expr)) {

        // Ignora espaços
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
//       (Shunting Yard)
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

        // Número → vai direto para saída
        if (isdigit(token[0]) || (token[0] == '.' && strlen(token) > 1)) {
            strcat(posfixa, token);
            strcat(posfixa, " ");
        }

        // Função → marca com F
        else if (isFuncao(token)) {
            pushOp(&pilha, 'F');
        }

        // Operador
        else if (strchr("+-*/%^", token[0])) {

            // Desempilha operadores enquanto tiver maior precedência
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

        // Fecha parênteses → desempilha até achar '('
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
    if (posfixa[strlen(posfixa) - 1] == ' ')
        posfixa[strlen(posfixa) - 1] = '\0';

    // Libera tokens
    for (int i = 0; i < count; i++) free(tokens[i]);
    free(tokens);

    return posfixa;
}


// ============================
//     Avaliação PÓS-FIXA
// ============================

// Calcula o valor final da expressão pós-fixa
float avaliarPosfixa(char *posfixa) {
    PilhaNumeros pilha;
    pilha.top = 0;

    char *copia = malloc(strlen(posfixa) + 1);
    strcpy(copia, posfixa);

    char *token = strtok(copia, " ");

    while (token != NULL) {

        // Número → empilha
        if (isdigit(token[0]) || (token[0] == '.' && strlen(token) > 1) ||
            (token[0] == '-' && strlen(token) > 1)) {

            pushNum(&pilha, atof(token));
        }

        // Operador binário
        else if (strlen(token) == 1 && strchr("+-*/%^%", token[0])) {

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
    return pilha.items[0];
}


// ============================
//     PÓS-FIXA → INFIXA
// ============================

// Reconstroi uma expressão infixa a partir da pós-fixa
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

    PilhaStrings pilha;
    pilha.top = 0;

    for (int i = 0; i < count; i++) {

        // Número
        if (isdigit(tokens[i][0]) ||
            (tokens[i][0] == '.' && strlen(tokens[i]) > 1)) {

            pushStr(&pilha, tokens[i]);
        }

        // Operador binário
        else if (strlen(tokens[i]) == 1 &&
                 strchr("+-*/%^%", tokens[i][0])) {

            char *b = popStr(&pilha);
            char *a = popStr(&pilha);

            char resultado[256];
            sprintf(resultado, "(%s%c%s)", a, tokens[i][0], b);

            pushStr(&pilha, resultado);
        }

        // Função
        else if (isFuncao(tokens[i])) {

            char *a = popStr(&pilha);
            char resultado[256];
            sprintf(resultado, "%s(%s)", tokens[i], a);

            pushStr(&pilha, resultado);
        }
    }

    // Resultado final
    char *resultado = malloc(MAX_EXPR);
    strcpy(resultado, popStr(&pilha));

    for (int i = 0; i < count; i++) free(tokens[i]);
    free(tokens);

    return resultado;
}


// ============================
//        Funções públicas
// ============================

// Converte pós-fixa → infixa (interface externa)
char* getFormaInFixa(char *Str) {
    if (Str == NULL || strlen(Str) == 0) return NULL;
    return posfixaParaInfixa(Str);
}

// Avalia expressão pós-fixa (interface externa)
float getValorPosFixa(char *StrPosFixa) {
    if (StrPosFixa == NULL || strlen(StrPosFixa) == 0) return 0;
    return avaliarPosfixa(StrPosFixa);
}
