#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

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
  float estadoCritico;
};


void setAgentes(int x, int y, int b, int s, agentes *a);
void agentePosicionGo(agentes *a, char action, int **mapa);
void agentePosicionBack(agentes *a, char action);

int **setambiente(int x, int y, string paredes, string vitimas);
int **start_vasculhador(agentes *a, int** mapa, int x, int y, vitima *lv);
void start_socorrista(agentes *b, int** mapa, int x, int y, vitima *lv);

float distanciaH(int x1, int x2, int y1, int y2);


int main(){
  
  string line, text, dados, aux, vitimas, paredes;
  int n_line, i, m = 10 , n = 10, apx, apy;
  
  vitima lv[5];

  //leitura config.txt
  ifstream myfileA("ambiente.txt");
	if (myfileA.is_open()) {
      while (getline(myfileA, line)){
        for(i = 0; i<line.size(); i++){
          //tira o titulo
          if(line[i] == '=')
            break;
        }
        text = line.substr(0, i);

        if(text == "maxLin"){
          aux = line.substr(i+1, i+1);
          m = stoi(aux);
          cout << m <<endl;
        }else if(text == "maxCol"){
          n = stoi(dados);
        }
      }
      //fecha arquivo
      myfileA.close();
	}
	else{
    cout << "\nUnable to open file";
    return 0;
  }
  // Leitura do Ambiente.txt
  ifstream myfile("ambiente.txt");
	if (myfile.is_open()) {
      while (getline(myfile, line)){
        for(i = 0; i<line.size(); i++){
          //tira o titulo
          if(line[i] == ' ')
            break;
        }
        text = line.substr(0, i);
        dados = line.substr(i+1,line.size());

        if(text == "Agente"){
          aux = line.substr(i+1, i+1);
          apx = stoi(aux);
          aux = line.substr(i+3, i+3);
          apy = stoi(aux);
        }else if(text == "Parede"){
          paredes = dados;
        }else if(text == "Vitima"){
          vitimas = dados;
        }else if(text == "Objetivo"){
          //Opcional - objetivo final
        }

      }
      //fecha arquivo
      myfile.close();

      //cout << vitimas << "\n" << paredes << endl;
      int **mapa, **mapaSoc;
      mapa = setambiente(m, n, paredes, vitimas);

      //Vasculhador - Busca cega - profundidade
      cout << "Vasculhador trabalhando" << endl;
      agentes vasculhador;
      setAgentes(apx, apy, 240, 0, &vasculhador);
      mapaSoc = start_vasculhador(&vasculhador, mapa, m, n, lv);

      //Socorrista - Busca informada - A*
      cout << "Socorrista trabalhando" << endl;
      agentes socorrista;
      setAgentes(apx, apy, 240, 0, &socorrista);
      start_socorrista(&socorrista, mapa, m, n, lv);

	}
	else{
    cout << "\nUnable to open file";
    return 0;
  }
		
  return 0;
}

void setAgentes(int x, int y, int b, int s, agentes *a){
  a->bateria = b;
  a->posx = x;
  a->posy = y;
  a->suprimentos = s;
  a->tempo = 0;
}

void agentePosicionGo(agentes *a, int action, int **mapa){

  //Movimentação do agente
  if(action == N){
    a->posx = a->posx - 1;
    a->bateria--;
    a->tempo++;
  }else if(action == S){
    a->posx = a->posx + 1;
    a->bateria--;
    a->tempo++;
  }else if(action == O){
    a->posy = a->posy - 1;
    a->bateria--;
    a->tempo++;
  }else if(action == L){
    a->posy = a->posy + 1;
    a->bateria--;
    a->tempo++;
  }else if(action == NE){ //NORDESTE
    a->posx = a->posx - 1;
    a->posy = a->posy + 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }else if(action == NO){ //NOROESTE
    a->posx = a->posx - 1;
    a->posy = a->posy - 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }else if(action == SE){ //SUDESTE
    a->posx = a->posx + 1;
    a->posy = a->posy + 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }else if(action == SO){ //SUDOESTE
    a->posx = a->posx + 1;
    a->posy = a->posy - 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }


}

void agentePosicionBack(agentes *a, int action){

  //Movimentação do agente
  if(action == N){
    a->posx = a->posx + 1;
    a->bateria--;
    a->tempo++;
  }else if(action == S){
    a->posx = a->posx - 1;
    a->bateria--;
    a->tempo++;
  }else if(action == O){
    a->posy = a->posy + 1;
    a->bateria--;
    a->tempo++;
  }else if(action == L){
    a->posy = a->posy - 1;
    a->bateria--;
    a->tempo++;
  }else if(action == NE){ //NORDESTE
    a->posx = a->posx + 1;
    a->posy = a->posy - 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }else if(action == NO){ //NOROESTE
    a->posx = a->posx + 1;
    a->posy = a->posy + 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }else if(action == SE){ //SUDESTE
    a->posx = a->posx - 1;
    a->posy = a->posy - 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }else if(action == SO){ //SUDOESTE
    a->posx = a->posx - 1;
    a->posy = a->posy + 1;
    a->bateria = a->bateria - 1.5;
    a->tempo = a->tempo + 1.5;
  }

}

int** start_vasculhador(agentes *a, int** mapa, int x, int y, vitima *lv){
  int vcount=0, **mapaV, i, j, action, vadj[8];

  vadj[0] = S;
  vadj[1] = SE;
  vadj[2] = L;
  vadj[3] = NE;
  vadj[4] = N; 
  vadj[5] = NO;
  vadj[6] = O;
  vadj[7] = SO;

  int count=0;

  //Criação do mapa do vasculhador
  mapaV = new int*[x];
  for(i = 0 ; i < x ; i++){
    mapaV[i] = new int[y];
    for(j = 0 ; j < y ; j++){
        mapaV[i][j] = 9;
    }
  }

  //Busca
  i = 0;
  action = S;
  
  while(a->bateria - (a->posx+a->posy) > 0){
    agentePosicionGo(a, action, mapa);
    if(a->posx < x && a->posy < y && a->posx >= 0 && a->posy >=0){
        if(mapaV[a->posx][a->posy] == 0 || mapaV[a->posx][a->posy] == 1){
          agentePosicionBack(a, action);
          if(i+1 < 8)
            i++;
          else{
            i = 0;
          }
          action = vadj[i];
        }else if(mapa[a->posx][a->posy] == 2){
          mapaV[a->posx][a->posy] = 2;
          lv[count].x = a->posx;
          lv[count].y = a->posy;
          count++;
        }else{
          //mapaV[a->posx][a->posy] = mapa[a->posx][a->posy];
          mapaV[a->posx][a->posy] = 0;
          if(mapa[a->posx][a->posy] == 1){
            mapaV[a->posx][a->posy] = 1;
            agentePosicionBack(a, action);
            if(i+1 < 8)
              i++;
            else{
              i = 0;
            }
            action = vadj[i];
          }
        }
    }else{
      agentePosicionBack(a, action);
      if(i+1 < 8)
        i++;
      else{
            i = 0;
          }
      action = vadj[i];
    }
    vcount++;
  }

  for(i = 0 ; i < x ; i++){
    for(j = 0 ; j < y ; j++){
        cout << mapaV[i][j] << " ";
    }
    cout << endl;
  }

  return mapaV;

}

float distanciaH(int x1, int x2, int y1, int y2){
  float dh;

  dh = sqrt(pow(x2-x1,2)+pow(y2-y1,2));

  return dh;
}

void start_socorrista(agentes *b, int** mapa, int x, int y, vitima *lv){

  int vcout = 0, action;
  float heuristica, fh, aux;
  vitima a;
  //Analise do mapa do vasculhador


  a.x = lv[0].x;
  a.y = lv[0].y;

  //calculo heuristico

  while(vcout == 0){
      fh=999.0;
      if(mapa[b->posx][b->posy] == 2)
        vcout = 1;

      if(distanciaH(b->posx+1,a.x,b->posy,a.y) + 1.0 < fh && b->posx < x){
          if(mapa[b->posx+1][b->posy] != 1){
            fh = distanciaH(b->posx+1,a.x,b->posy,a.y) + 1 ;
            action = S;
          } 
      }
      if(distanciaH(b->posx,a.x,b->posy+1,a.y) + 1.0 < fh && b->posy < y ){
          if(mapa[b->posx][b->posy+1] != 1 && mapa[b->posx][b->posy+1] != 8){
            fh = distanciaH(b->posx,a.x,b->posy+1,a.y) + 1 ;
            action = L;
          }    
      }
      if(distanciaH(b->posx+1,a.x,b->posy+1,a.y) + 1.5 < fh && (b->posx < x && b->posy < y)){
          if(mapa[b->posx+1][b->posy+1] != 1 && mapa[b->posx+1][b->posy+1] != 8){
            fh = distanciaH(b->posx+1,a.x,b->posy+1,a.y) + 1.5 ;
            action = SE;
          }
      }
      if(distanciaH(b->posx-1,a.x,b->posy+1,a.y) + 1.5 < fh && (b->posx >= 0 && b->posy < y)){
          if(mapa[b->posx-1][b->posy+1] != 1 && mapa[b->posx-1][b->posy+1] != 8){
            fh = distanciaH(b->posx-1,a.x,b->posy+1,a.y) + 1.5 ;
            action = NE;
          }
      }
      if(distanciaH(b->posx-1,a.x,b->posy,a.y) + 1 < fh && b->posx >= 0){
          if(mapa[b->posx-1][b->posy] != 1 && mapa[b->posx-1][b->posy] != 8){
            fh = distanciaH(b->posx-1,a.x,b->posy,a.y) + 1 ;
            action = N;
          }
      }
      if(distanciaH(b->posx-1,a.x,b->posy-1,a.y) + 1.5 < fh && (b->posx >= 0 && b->posy >= 0)){
          if(mapa[b->posx-1][b->posy-1] != 1 && mapa[b->posx-1][b->posy-1] != 8){
            fh = distanciaH(b->posx-1,a.x,b->posy-1,a.y) + 1.5 ;
            action = NO;
          }
      }
      if(distanciaH(b->posx,a.x,b->posy-1,a.y) + 1 < fh && b->posy >= 0){
          if(mapa[b->posx][b->posy-1] != 1 && mapa[b->posx][b->posy-1] != 8){
            fh = distanciaH(b->posx,a.x,b->posy-1,a.y) + 1 ;
            action = O;
          }
      }if(distanciaH(b->posx+1,a.x,b->posy-1,a.y) + 1.5 < fh && (b->posx >= 0 && b->posy < y)){
          if(mapa[b->posx-1][b->posy+1] != 1 && mapa[b->posx-1][b->posy+1] != 8){
            fh = distanciaH(b->posx-1,a.x,b->posy+1,a.y) + 1.5 ;
            action = SO;
          }
      }
      cout << b->posx << "," << b->posy <<endl;
      agentePosicionGo(b, action, mapa);
      
  }

}




int **setambiente(int x, int y, string paredes, string vitimas){
  int i,j,px,py;
  int** mapa;
  string aux;

  mapa = new int*[x];
  for(i = 0 ; i < x ; i++){
    mapa[i] = new int[y];
    for(j = 0 ; j < y ; j++){
        mapa[i][j] = 0;
    }
  }

  for(i = 0; i<paredes.size(); i++){
      if(paredes[i] == ','){
          aux = paredes[i-1];
          px = stoi(aux);

          aux = paredes[i+1];
          py = stoi(aux);
  
        if(paredes[i-2] != ' ' && i > 3){
          cout << "opa" << endl;
          aux = paredes.substr(i-2,i-1);
          cout << aux << endl;
          px = stoi(aux);
        }
        if(paredes[i+2] != ' '){
          cout << "opa" << endl;
          aux = paredes.substr(i+1,i+2);
          cout << aux << endl;
          py = stoi(aux);
        }
          
        
        mapa[px][py] = 1;
      }
         
  }

  for(i = 0; i<vitimas.size(); i++){
      if(vitimas[i] == ','){
          aux = vitimas[i-1];
          px = stoi(aux);

          aux = vitimas[i+1];
          py = stoi(aux);
  
        if(vitimas[i-2] != ' ' && i > 3){
          aux = vitimas.substr(i-2,i-1);
          px = stoi(aux);
        }
        if(vitimas[i+2] != ' '){
          aux = vitimas.substr(i+1,i+2);
          py = stoi(aux);
        }
          
        mapa[px][py] = 2;
      }
         
  }

  for(i = 0 ; i < x ; i++){
    for(j = 0 ; j < y ; j++){
        cout << mapa[i][j] << " ";
    }
    cout << endl;
  }




  return mapa;
}

//Fazer a matrix espelho

