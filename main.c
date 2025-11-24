#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>

int passaram = 0;
int falharam = 0;

FILE *arquivo;

void testar(int numero, const char *posfixa, float valor_esperado, const char *descricao) {
    printf("\n========== TESTE %d ==========\n", numero);
    fprintf(arquivo, "\n========== TESTE %d ==========\n", numero);
    
    printf("Descricao: %s\n", descricao);
    fprintf(arquivo, "Descricao: %s\n", descricao);
    
    printf("Expressao pos-fixa: %s\n", posfixa);
    fprintf(arquivo, "Expressao pos-fixa: %s\n", posfixa);
    
    float valor = getValorPosFixa((char*)posfixa);
    printf("Valor calculado: %.6f\n", valor);
    fprintf(arquivo, "Valor calculado: %.6f\n", valor);
    
    printf("Valor esperado: %.6f\n", valor_esperado);
    fprintf(arquivo, "Valor esperado: %.6f\n", valor_esperado);
    
    float diferenca = (valor > valor_esperado) ? valor - valor_esperado : valor_esperado - valor;
    printf("Diferenca: %.6f\n", diferenca);
    fprintf(arquivo, "Diferenca: %.6f\n", diferenca);
    
    if (diferenca < 0.01) {
        printf("RESULTADO: OK - PASSOU\n");
        fprintf(arquivo, "RESULTADO: OK - PASSOU\n");
        passaram++;
    } else {
        printf("RESULTADO: ERRO - FALHOU\n");
        fprintf(arquivo, "RESULTADO: ERRO - FALHOU\n");
        falharam++;
    }
}

int main() {
    arquivo = fopen("resultado_testes.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }
    
    printf("========================================\n");
    fprintf(arquivo, "========================================\n");
    printf("AVALIADOR DE EXPRESSOES NUMERICAS\n");
    fprintf(arquivo, "AVALIADOR DE EXPRESSOES NUMERICAS\n");
    printf("BATERIA DE TESTES COMPLETA\n");
    fprintf(arquivo, "BATERIA DE TESTES COMPLETA\n");
    printf("========================================\n");
    fprintf(arquivo, "========================================\n");
    
    printf("\n\n============ TESTES OBRIGATORIOS DA TABELA ============\n");
    fprintf(arquivo, "\n\n============ TESTES OBRIGATORIOS DA TABELA ============\n");
    
    testar(1, "3 4 + 5 *", 35.0, "(3+4)*5");
    testar(2, "7 2 * 4 +", 18.0, "7*2+4");
    testar(3, "8 5 2 4 + * +", 38.0, "8+(5*(2+4))");
    testar(4, "6 2 / 3 + 4 *", 24.0, "(6/2+3)*4");
    testar(5, "9 5 2 8 * 4 + * +", 109.0, "9+(5*(4+8*2))");
    testar(6, "2 3 + log 5 /", 0.14, "log(2+3)/5");
    testar(7, "10 log 3 ^ 2 +", 3.0, "(log(10))^3+2");
    testar(8, "45 60 + 30 cos *", 90.93, "(45+60)*cos(30)");
    testar(9, "0.5 45 sen 2 ^ +", 1.0, "sen(45)^2+0.5");
    
    printf("\n\n============ TESTES ADICIONAIS - BASICOS ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - BASICOS ============\n");
    
    testar(10, "5 3 +", 8.0, "5+3");
    testar(11, "1 1 +", 2.0, "1+1");
    testar(12, "10 20 +", 30.0, "10+20");
    testar(13, "10 2 -", 8.0, "10-2");
    testar(14, "50 30 -", 20.0, "50-30");
    testar(15, "100 1 -", 99.0, "100-1");
    testar(16, "4 5 *", 20.0, "4*5");
    testar(17, "3 7 *", 21.0, "3*7");
    testar(18, "6 6 *", 36.0, "6*6");
    testar(19, "20 4 /", 5.0, "20/4");
    testar(20, "100 10 /", 10.0, "100/10");
    testar(21, "50 5 /", 10.0, "50/5");
    
    printf("\n\n============ TESTES ADICIONAIS - POTENCIA E MODULO ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - POTENCIA E MODULO ============\n");
    
    testar(22, "2 3 ^", 8.0, "2^3");
    testar(23, "3 2 ^", 9.0, "3^2");
    testar(24, "5 2 ^", 25.0, "5^2");
    testar(25, "2 10 ^", 1024.0, "2^10");
    testar(26, "10 2 ^", 100.0, "10^2");
    testar(27, "7 3 %", 1.0, "7 mod 3");
    testar(28, "10 3 %", 1.0, "10 mod 3");
    testar(29, "15 4 %", 3.0, "15 mod 4");
    testar(30, "20 6 %", 2.0, "20 mod 6");
    
    printf("\n\n============ TESTES ADICIONAIS - FUNCOES RAIZ ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - FUNCOES RAIZ ============\n");
    
    testar(31, "4 raiz", 2.0, "sqrt(4)");
    testar(32, "9 raiz", 3.0, "sqrt(9)");
    testar(33, "16 raiz", 4.0, "sqrt(16)");
    testar(34, "25 raiz", 5.0, "sqrt(25)");
    testar(35, "100 raiz", 10.0, "sqrt(100)");
    
    printf("\n\n============ TESTES ADICIONAIS - FUNCOES TRIGONOMETRICAS ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - FUNCOES TRIGONOMETRICAS ============\n");
    
    testar(36, "0 sen", 0.0, "sen(0)");
    testar(37, "90 sen", 1.0, "sen(90)");
    testar(38, "30 sen", 0.5, "sen(30)");
    testar(39, "180 sen", 0.0, "sen(180)");
    testar(40, "0 cos", 1.0, "cos(0)");
    testar(41, "90 cos", 0.0, "cos(90)");
    testar(42, "60 cos", 0.5, "cos(60)");
    testar(43, "180 cos", -1.0, "cos(180)");
    testar(44, "0 tg", 0.0, "tg(0)");
    testar(45, "45 tg", 1.0, "tg(45)");
    
    printf("\n\n============ TESTES ADICIONAIS - FUNCOES LOGARITMO ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - FUNCOES LOGARITMO ============\n");
    
    testar(46, "1 log", 0.0, "log(1)");
    testar(47, "10 log", 1.0, "log(10)");
    testar(48, "100 log", 2.0, "log(100)");
    testar(49, "1000 log", 3.0, "log(1000)");
    
    printf("\n\n============ TESTES ADICIONAIS - EXPRESSOES COM 2 OPERADORES ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - EXPRESSOES COM 2 OPERADORES ============\n");
    
    testar(50, "1 2 + 3 +", 6.0, "(1+2)+3");
    testar(51, "5 5 + 10 +", 20.0, "(5+5)+10");
    testar(52, "2 3 + 4 *", 20.0, "(2+3)*4");
    testar(53, "3 4 * 2 +", 14.0, "(3*4)+2");
    testar(54, "10 2 / 3 -", 2.0, "(10/2)-3");
    testar(55, "20 2 / 5 +", 15.0, "(20/2)+5");
    testar(56, "2 3 ^ 5 +", 13.0, "(2^3)+5");
    testar(57, "3 2 ^ 1 +", 10.0, "(3^2)+1");
    
    printf("\n\n============ TESTES ADICIONAIS - EXPRESSOES COM 3+ OPERADORES ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - EXPRESSOES COM 3+ OPERADORES ============\n");
    
    testar(58, "2 3 + 4 * 5 -", 15.0, "((2+3)*4)-5");
    testar(59, "10 2 / 3 + 4 *", 32.0, "((10/2)+3)*4");
    testar(60, "5 2 ^ 3 * 10 +", 85.0, "((5^2)*3)+10");
    testar(61, "4 raiz 2 + 3 *", 12.0, "(sqrt(4)+2)*3");
    testar(62, "9 raiz 1 + 2 ^", 16.0, "(sqrt(9)+1)^2");
    
    printf("\n\n============ TESTES ADICIONAIS - NUMEROS DECIMAIS ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - NUMEROS DECIMAIS ============\n");
    
    testar(63, "0.5 0.5 +", 1.0, "0.5+0.5");
    testar(64, "1.5 2.5 +", 4.0, "1.5+2.5");
    testar(65, "3.5 1.5 -", 2.0, "3.5-1.5");
    testar(66, "2.5 2 *", 5.0, "2.5*2");
    testar(67, "0.5 4 *", 2.0, "0.5*4");
    testar(68, "5.0 2 /", 2.5, "5.0/2");
    testar(69, "10.0 4 /", 2.5, "10.0/4");
    
    printf("\n\n============ TESTES ADICIONAIS - POTENCIA ASSOCIATIVIDADE DIREITA ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - POTENCIA ASSOCIATIVIDADE DIREITA ============\n");
    
    testar(70, "2 3 2 ^ ^", 512.0, "2^(3^2)");
    testar(71, "2 2 3 ^ ^", 256.0, "2^(2^3)");
    
    printf("\n\n============ TESTES ADICIONAIS - COMPLEXOS ============\n");
    fprintf(arquivo, "\n\n============ TESTES ADICIONAIS - COMPLEXOS ============\n");
    
    testar(72, "8 5 2 4 + * +", 38.0, "8+(5*(2+4))");
    testar(73, "100 10 2 - /", 12.5, "100/(10-2)");
    testar(74, "100 log 5 * 2 +", 12.0, "(log(100)*5)+2");
    testar(75, "16 raiz 3 * 2 +", 14.0, "(sqrt(16)*3)+2");
    
    printf("\n\n========================================\n");
    fprintf(arquivo, "\n\n========================================\n");
    printf("RESUMO DOS TESTES\n");
    fprintf(arquivo, "RESUMO DOS TESTES\n");
    printf("========================================\n");
    fprintf(arquivo, "========================================\n");
    printf("Total de testes: %d\n", passaram + falharam);
    fprintf(arquivo, "Total de testes: %d\n", passaram + falharam);
    printf("Testes aprovados: %d\n", passaram);
    fprintf(arquivo, "Testes aprovados: %d\n", passaram);
    printf("Testes falhados: %d\n", falharam);
    fprintf(arquivo, "Testes falhados: %d\n", falharam);
    printf("Taxa de sucesso: %.1f%%\n", (passaram * 100.0) / (passaram + falharam));
    fprintf(arquivo, "Taxa de sucesso: %.1f%%\n", (passaram * 100.0) / (passaram + falharam));
    printf("========================================\n");
    fprintf(arquivo, "========================================\n");
    
    fclose(arquivo);
    printf("\nArquivo 'resultado_testes.txt' gerado com sucesso!\n");
    
    return 0;
}