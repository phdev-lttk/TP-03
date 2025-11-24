# 📘 README -- Avaliador de Expressões Numéricas (C)

Este projeto implementa um **avaliador completo de expressões
matemáticas**, capaz de:

-   Converter **notação infixa → pós-fixa**
-   Converter **notação pós-fixa → infixa**
-   Avaliar expressões matemáticas completas\
-   Manipular funções matemáticas especiais\
-   Utilizar **pilhas** como estrutura de dados principal

O trabalho segue rigorosamente os requisitos da disciplina **Estrutura
de Dados -- UCB (2025/2)**.

## 📂 Estrutura do Projeto

    📁 projeto/
    │-- expressao.h        // Cabeçalho fornecido pela disciplina (não deve ser alterado)
    │-- expressao.c        // Implementação completa (converter e avaliar expressões)
    │-- main.c             // Arquivo de testes criado pelo aluno
    │-- README.md          // Este arquivo

Compilação recomendada:

``` bash
gcc expressao.c main.c -o expressao.exe -lm
```

## 🧠 Funcionalidades Implementadas

### ✔ Conversões entre notações

-   **Infixa → Pós-fixa**
-   **Pós-fixa → Infixa**

Ambas implementadas manualmente com **pilhas**, seguindo o algoritmo
*Shunting Yard*.

### ✔ Avaliação de expressões matemáticas

O programa suporta todas as operações obrigatórias:

  Tipo                    Operações
  ----------------------- -------------------------------------------
  **Aritméticas**         `+`, `-`, `*`, `/`, `%`, `^`
  **Funções especiais**   `raiz`, `sen`, `cos`, `tg`, `log`
  **Parâmetros**          seno/cosseno/tangente operam em **graus**

### ✔ Estrutura de Dados

Foram implementadas pilhas específicas:

-   `PilhaNumeros` → cálculos
-   `PilhaOperadores` → conversão infixa→pós-fixa
-   `PilhaStrings` → conversão pós-fixa→infixa

## 📌 Funções Principais (API Pública)

``` c
char* getFormaInFixa(char *StrPosFixa);
// Converte expressão pós-fixa para forma infixa.

float getValorPosFixa(char *StrPosFixa);
// Avalia expressão pós-fixa e retorna valor numérico.
```

## 🧪 Tabela Oficial de Testes (PDF)

  Nº   Pós-fixa              Infixa Esperada     Valor
  ---- --------------------- ------------------- ---------
  1    `3 4 + 5 *`           `(3+4)*5`           **35**
  2    `7 2 * 4 +`           `7*2+4`             **18**
  3    `8 5 2 4 + * +`       `8+5*(2+4)`         **38**
  4    `6 2 / 3 + 4 *`       `(6/2+3)*4`         **24**
  5    `9 5 2 8 * 4 + * +`   `9+5*(4+8*2)`       **109**
  6    `2 3 + log 5 /`       `log(2+3)/5`        \~0.14
  7    `10 log 3 ^ 2 +`      `(log10)^3+2`       3
  8    `45 60 + 30 cos *`    `(45+60)*cos(30)`   \~90.93
  9    `0.5 45 sen 2 ^ +`    `0.5+sen(45)^2`     1

## 🛠 Tecnologias utilizadas

-   Linguagem C
-   Bibliotecas padrão (`stdio`, `stdlib`, `string.h`, `ctype.h`,
    `math.h`)
-   Estruturas de pilha implementadas manualmente
-   Shunting Yard para parsing

## 🧩 Exemplo de Uso (main.c)

``` c
#include <stdio.h>
#include "expressao.h"

int main() {
    char expr[] = "3 4 + 5 *";

    printf("Infixa: %s\n", getFormaInFixa(expr));
    printf("Valor: %.2f\n", getValorPosFixa(expr));

    return 0;
}
```

Saída:

    Infixa: (3+4)*5
    Valor: 35.00

## 📧 Autor

Projeto desenvolvido por **Pablo Henrique Anastácio de Souza**\
Disciplina: **Estrutura de Dados -- 2025/2**\
Professor: **Marcelo Eustáquio**
