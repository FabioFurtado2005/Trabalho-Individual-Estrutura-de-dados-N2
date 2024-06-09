#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cidades.h"

int comp(const void *a, const void *b) {
    Cidade *cidadeA = (Cidade *)a;
    Cidade *cidadeB = (Cidade *)b;
    return cidadeA->Posicao - cidadeB->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (estrada == NULL) {
        printf("Erro ao alocar memória para a estrada.\n");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%d", &estrada->T); // faz a leitura da primeira linha: comprimento da estrada
    fscanf(file, "%d", &estrada->N); // faz a leitura da segunda linha: número de cidades

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (estrada->C == NULL) {
        printf("Erro ao alocar memória para as cidades.\n");
        free(estrada);
        fclose(file);
        return NULL;
    }

    int i;
    for (i = 0; i < estrada->N; i++) {
        fscanf(file, "%d %[^\n]", &estrada->C[i].Posicao, estrada->C[i].Nome);
    }

    fclose(file);

    qsort(estrada->C, estrada->N, sizeof(Cidade), comp);

    return estrada;
}

double calcularvizinho(int posicao1, int posicao2) {
    return fabs(posicao1 - posicao2) / 2.0;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return -1;
    }

    double menorVizi = INFINITY;

    // Calcular vizinhança entre cidades consecutivas
    double Mediavizinhanca = calcularvizinho(estrada->C[0].Posicao, estrada->C[1].Posicao);
    double Primeiravizinhanca = estrada->C[0].Posicao + Mediavizinhanca;

    menorVizi = Primeiravizinhanca;

    if (estrada->N > 2) {
        double Atualvizinhanca;
        int i;
        for (i = 1; i < estrada->N - 1; i++) {
            Atualvizinhanca = calcularvizinho(estrada->C[i].Posicao, estrada->C[i + 1].Posicao) + Mediavizinhanca;
            if (Atualvizinhanca < menorVizi) {
                menorVizi = Atualvizinhanca;
            }
            Mediavizinhanca = calcularvizinho(estrada->C[i].Posicao, estrada->C[i + 1].Posicao);
        }

        // Calcular vizinhança da última cidade com o final da estrada
        Atualvizinhanca = (estrada->T - estrada->C[estrada->N - 1].Posicao) + Mediavizinhanca;
        if (Atualvizinhanca < menorVizi) {
            menorVizi = Atualvizinhanca;
        }
    } else {
        double Segundovizinhanca = (estrada->T - estrada->C[1].Posicao) + Mediavizinhanca;
        if (Primeiravizinhanca > Segundovizinhanca) {
            menorVizi = Segundovizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);

    return menorVizi;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return NULL;
    }

    int iMenorV = 0;
    double menorVizi = INFINITY;

    double Mediavizinhanca = calcularvizinho(estrada->C[0].Posicao, estrada->C[1].Posicao);
    double Primeiravizinhanca = estrada->C[0].Posicao + Mediavizinhanca;

    menorVizi = Primeiravizinhanca;

    if (estrada->N > 2) {
        double Atualvizinhanca;
        int i;
        for (i = 1; i < estrada->N - 1; i++) {
            Atualvizinhanca = calcularvizinho(estrada->C[i].Posicao, estrada->C[i + 1].Posicao) + Mediavizinhanca;
            if (Atualvizinhanca < menorVizi) {
                menorVizi = Atualvizinhanca;
                iMenorV = i;
            }
            Mediavizinhanca = calcularvizinho(estrada->C[i].Posicao, estrada->C[i + 1].Posicao);
        }

        Atualvizinhanca = (estrada->T - estrada->C[estrada->N - 1].Posicao) + Mediavizinhanca;
        if (Atualvizinhanca < menorVizi) {
            menorVizi = Atualvizinhanca;
            iMenorV = estrada->N - 1;
        }
    } else {
        double Segundovizinhanca = (estrada->T - estrada->C[1].Posicao) + Mediavizinhanca;
        if (Primeiravizinhanca > Segundovizinhanca) {
            menorVizi = Segundovizinhanca;
            iMenorV = 1;
        } else {
            menorVizi = Primeiravizinhanca;
            iMenorV = 0;
        }
    }

    char *cidadeMenorViz = strdup(estrada->C[iMenorV].Nome);

    free(estrada->C);
    free(estrada);

    return cidadeMenorViz;
}

int main() {
    const char *nomeArquivo = "arquivo.txt";

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    char *cidadeMenorViz = cidadeMenorVizinhanca(nomeArquivo);

    if (menorVizinhanca == -1 || cidadeMenorViz == NULL) {
        printf("Erro ao processar o arquivo.\n");
    } else {
        printf("Menor vizinhança: %.2f\n", menorVizinhanca);
        printf("Cidade com a menor vizinhança: %s\n", cidadeMenorViz);
        free(cidadeMenorViz);
    }

    return 0;
}

