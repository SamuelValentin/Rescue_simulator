#include "heading.h"

int main(){
  
  string line, text, dados, aux, vitimas, paredes;
  int n_line, i, j, m = 32 , n = 32, apx, apy, vcount=0;
  
  vitima2 lv[42];
  
  //leitura difacesso.txt
  ifstream myfileA("config/difacesso.txt");
	if (myfileA.is_open()) {
      j = 0;
      while (getline(myfileA, line)){
        vcount = 0;
        for(i = 0; i<line.size(); i++){
          //tira o titulo
          if(line[i] == ' '){
            vcount++;
            if(vcount == 6)
              break;
          }  
        }
        aux = line.substr(i+1, line.size()+1);
        m = stof(aux);
        lv[j].acesso = m;
        j++;
      }
      //fecha arquivo
      myfileA.close();
	}
	else{
    cout << "\nUnable to open file";
    return 0;
  }

  //leitura SinaisVitais.txt
  ifstream myfileB("config/sinaisvitais.txt");
	if (myfileB.is_open()) {
      j = 0;
      while (getline(myfileB, line)){
        vcount = 0;
        for(i = 0; i<line.size(); i++){
          //tira o titulo
          if(line[i] == ' '){
            vcount++;
            if(vcount == 5)
              break;
          }  
        }
        aux = line.substr(i+3, line.size()+1);
        m = stof(aux);
        if(m == 0)
          lv[j].estadoCritico = 100;
        else  
          lv[j].estadoCritico = m%100;
        j++;
      }
      //fecha arquivo
      myfileB.close();
	}
	else{
    cout << "\nUnable to open file";
    return 0;
  }

  socorrista_genetico(lv, 42);
  /*
  // Leitura do ambiente.txt
  ifstream myfile("config/ambiente.txt");
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
      setAgentes(apx, apy, 1000, 0, &vasculhador);
      mapaSoc = start_vasculhador(&vasculhador, mapa, m, n, lv, &vcount);

      //Socorrista - Busca informada - A*
      cout << "Socorrista trabalhando" << endl;
      agentes socorrista;
      setAgentes(apx, apy, 100, 0, &socorrista);
      start_socorrista(&socorrista, mapa, m, n, lv, vcount);

	}
	else{
    cout << "\nUnable to open file";
    return 0;
  }*/
		
  return 0;
}

// Funções parte 1 ----------------------------------

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

// Vasculhador ---------------------------------------------

int** start_vasculhador(agentes *a, int** mapa, int x, int y, vitima *lv, int *vcount){
  int **mapaV, i, j, action, vadj[8];

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
          a->bateria= a->bateria-2;
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
  }

  for(i = 0 ; i < x ; i++){
    for(j = 0 ; j < y ; j++){
        cout << mapaV[i][j] << " ";
    }
    cout << endl;
  }

  for(i = 0 ; i < x ; i++){
    cout << lv[i].x << "," << lv[i].y <<endl;
  }

  *vcount = count;

  return mapaV;

}

// Socorrista parte 1 ----------------------------------------------

float distanciaH(int x1, int x2, int y1, int y2){
  float dh;

  dh = sqrt(pow(x2-x1,2)+pow(y2-y1,2));

  return dh;
}

void start_socorrista(agentes *b, int** mapa, int x, int y, vitima *lv, int nv){

  int action, vcout;
  float heuristica, fh, aux;
  vitima a;
  //Analise do mapa do vasculhador


  a.x = lv[0].x;
  a.y = lv[0].y;
  vcout = 0;

  //calculo heuristico

  while(b->bateria - (b->posx+b->posy) > 0 && vcout < nv){
      fh=999.0;
      if(mapa[b->posx][b->posy] == 2){
        vcout++;
        cout << "resgatou vitim:" << a.x << "," << a.y << endl;
        a.x = lv[vcout].x;
        a.y = lv[vcout].y;
      }

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


// Ambiente ---------------------------------

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
          aux = paredes.substr(i-2,i-1);
          px = stoi(aux);
        }
        if(paredes[i+2] != ' '){
          aux = paredes.substr(i+1,i+2);
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

//---------------------------------------------------------------------- PARTE 2

void socorrista_genetico(vitima2 *lv, int nV){
  vitima2 *individuoP;

  srand (time(NULL));

  int numI=8, tamV=42, i ,j, timeL=800, pontA, numPais=4, herenca, geracoes = 10;
  int *array[10], pont[numI], temp[numI], pontP[2];

  Individuos listaInd[numI], listaPais[numPais], melhorInd;

  melhorInd.pont = 38;
  melhorInd.bits = NULL;
  melhorInd.tempo = 13;

  listaPais[0].pont = 0;
  for(j=0; j<numPais; j++){
    listaPais[j].pont = 0;
  }

  //array = new int(10);

  //Geração dos individuos
  for(j=0; j<numI; j++){
    listaInd[j].bits = new int(tamV);
    for(i=0; i<tamV; i++){
      listaInd[j].bits[i] = rand()%2;
    }
    if(j<numPais)
      listaPais[j] = listaInd[j];
  }

//while()
  
  for(int k=0; k<geracoes; k++){
        cout << "-----------------------" << endl;
        cout << "Pais:" << endl;
        //Print pais
        for(j=0; j<numPais; j++){
          for(i=0; i<tamV; i++){
            cout << listaPais[j].bits[i];
          }
          cout << endl;
        }

        cout << "-----------------------" << endl;
        cout << "Filhos:" << endl;
        //print Filhos --------
        
        for(i=0; i<numI; i++){
          for(j=0; j<tamV; j++){
            herenca = rand()%numPais;
            listaInd[i].bits[j] = listaPais[herenca].bits[j];
            listaInd[i].bits = mutacao(listaInd[i].bits, j, 25);
          }
        }

        pontuacao_individuos(numI, tamV, listaInd, lv);
        

        //Seleçao de pais
        for(j=0; j<numPais; j++){
          for(i=0; i<numI; i++){
            if(listaPais[j].pont <= listaInd[i].pont && listaInd[i].tempo <= timeL && listaInd[i].bits != listaPais[j-1].bits){
              if(listaPais[j].pont == listaInd[i].pont){
                if(listaPais[j].tempo > listaInd[i].tempo)
                  listaPais[j] = listaInd[i];
                else
                  listaPais[j] = listaInd[i];
              }else{
                listaPais[j] = listaInd[i];
              }
              listaInd[i].pont = 0;     
            }
          }
          if(melhorInd.pont < listaPais[j].pont)
                melhorInd = listaPais[j];
        }

        print_melhorI(melhorInd, tamV);
  }
  
  for(i=0; i<tamV; i++){
    cout << melhorInd.bits[i];
  }
  cout << " = "<< melhorInd.pont <<endl;
}

int* mutacao(int* v, int j, int prob){

  int mut = rand()%100;

  if(mut < prob){
    if(v[j] == 0)
      v[j] = 1;
    else  
      v[j] = 0;
  }

  return v;
}

void pontuacao_individuos(int numI, int tamV, Individuos* listaInd, vitima2 *lv){
  int tempo, pontA;
  float g=0, s=0;

// Calculo do tempo e custo
        for(int j=0; j<numI; j++){
          tempo=0;
          pontA=0;
          for(int i=0; i<tamV; i++){
                if(listaInd[j].bits[i] == 1){
                  tempo = tempo + lv[i].acesso;
                  g = g + lv[i].estadoCritico;
                  //printf("// %f", lv[i].estadoCritico);
                  //pontuaçao
                  if(lv[i].estadoCritico>=0 && lv[i].estadoCritico <20)
                    pontA = pontA + (/*lv[i].estadoCritico**/1);
                  else if(lv[i].estadoCritico>=20 && lv[i].estadoCritico <40)
                    pontA = pontA + (2);
                  else if(lv[i].estadoCritico>=40 && lv[i].estadoCritico <60)
                    pontA = pontA + (3);
                  else if(lv[i].estadoCritico>=60 && lv[i].estadoCritico <80)
                    pontA = pontA + (4);
                  else if(lv[i].estadoCritico>=80 && lv[i].estadoCritico <=100)
                    pontA = pontA + (5);
                  s++;
                }
                printf("%d", listaInd[j].bits[i]);
          }
          listaInd[j].tempo = tempo;
          printf(" : tempo = %f", listaInd[j].tempo);
          listaInd[j].pont = pontA;
          printf(" / pontuacao = %f", listaInd[j].pont);
          listaInd[j].vs = s;
          printf(" / pontuacao = %f", listaInd[j].vs);
          s = s / tempo;
          listaInd[j].s = s;
          printf(" / S = %f", listaInd[j].s);
          listaInd[j].g = g / 100;
          printf(" / G = %f\n", listaInd[j].g);
        }

}

void print_melhorI(Individuos a, int tamV){

    cout << "-----------------------" << endl;
    cout << "Melhor caso:" << endl;

    for(int i=0; i<tamV; i++){
      cout << a.bits[i];
    }
    cout << " = "<< a.pont << " / Vitimas salvas = " << a.vs << " / S = " << a.s << " / G = " << a.g <<endl;

}