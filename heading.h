#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

int N = 0;
int S = 1;
int O = 2;
int L = 3;
int NE = 4;
int NO = 5;
int SE = 6;
int SO = 7;

struct agentes{
  int posx, posy, suprimentos;
  float bateria, tempo;
};

struct vitima{
  int x, y;
  float estadoCritico, acesso;
};

struct vitima2{
    float estadoCritico, acesso;
};

struct Individuos{
    int *bits;
    float tempo, pont, s, g, vs;
};




void setAgentes(int x, int y, int b, int s, agentes *a);
void agentePosicionGo(agentes *a, char action, int **mapa);
void agentePosicionBack(agentes *a, char action);

int **setambiente(int x, int y, string paredes, string vitimas);
int **start_vasculhador(agentes *a, int** mapa, int x, int y, vitima *lv, int *vcount);
void start_socorrista(agentes *b, int** mapa, int x, int y, vitima *lv, int vcount);

float distanciaH(int x1, int x2, int y1, int y2);

void socorrista_genetico(vitima2 *lv, int nV);
int* mutacao(int* v, int j, int prob);
void pontuacao_individuos(int numI, int tamV, Individuos* listaInd, vitima2 *lv);
void print_melhorI(Individuos a, int tamV);