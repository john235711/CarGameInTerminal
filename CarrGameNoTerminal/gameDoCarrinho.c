/* projeto jogo do carrinho */
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define x_janela 30
#define y_janela 2
#define altura_janela 27
#define largura_janela 29
#define posx_inicial ((largura_janela/2) + x_janela)
#define posy_inicial (y_janela + altura_janela - 4)


int randint(int x, int y){
    if (y > x){
        int aux = y;
        y = x;
        x = y;
    }
    return x + rand()%(y+1);

}


float minimo(float x, float y){
    if (x < y) return x;
    return y;
}

void gotoxy(int x, int y){
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

typedef struct{
    float x;
    float y;
}coord;

typedef struct{
    coord coord;
    float velocity[2];
}CARRO;

void desenhar_borda_do_jogo(){
    /*desenhando linhas honrizontais*/

    gotoxy(x_janela, y_janela);
    for (int i = 0; i < largura_janela; i++){
        putchar(219);
    }
    gotoxy(x_janela, y_janela+altura_janela-1);
    for (int i = 0; i < largura_janela; i++){
        putchar(219);
    }
    /*desenhando linhas verticais*/
    gotoxy(x_janela, y_janela);
    for (int i = 0; i < altura_janela; i++){
        putchar(219);
        gotoxy(x_janela, y_janela+i+1);
    }

    gotoxy(x_janela+largura_janela-1, y_janela);
    for (int i = 0; i < altura_janela; i++){
        putchar(219);
        gotoxy(x_janela+largura_janela-1, y_janela+i+1);
    }

}


void atribuir_as_coordenadas_do_carro(CARRO carro, coord coordenadas[8]){
    // vou atribuir na lista de coordenadas todas as coordenadas onde ha presenca do carro
    coordenadas[0] = carro.coord;

    coordenadas[1].x = carro.coord.x; /* cabeca */
    coordenadas[1].y = carro.coord.y-1;

    coordenadas[2].x = carro.coord.x-1; /* lado esquerdo */
    coordenadas[2].y = carro.coord.y;

    coordenadas[3].x = carro.coord.x+1; /* lado direito */
    coordenadas[3].y = carro.coord.y;

    coordenadas[4].x = carro.coord.x; /* parte da coluna */
    coordenadas[4].y = carro.coord.y+1;

    coordenadas[5].x = carro.coord.x; /* parte central de baixo */
    coordenadas[5].y = carro.coord.y+2;

    coordenadas[6].x = carro.coord.x-1; /* parte esquerda de baixo */
    coordenadas[6].y = carro.coord.y+2;

    coordenadas[7].x = carro.coord.x+1; /* parte direita de baixo */
    coordenadas[7].y = carro.coord.y+2;

}
CARRO criar_carro(float x, float y, float velocity[2]){
    CARRO c;
    c.coord.x = x;
    c.coord.y = y;
    c.velocity[0] = velocity[0];
    c.velocity[1] = velocity[1];

    return c;
}


void renderizar_carro(CARRO carro){
    /* apartir de sua posicao posso desenhar as outras partes do carro
      []
    [][k][]  k onde as variaveis x e y estao
      []
    [][][]
    */

    coord coordenadas[8];
    atribuir_as_coordenadas_do_carro(carro, coordenadas);


    for (int i = 0; i < 8; i++){
        int x = (int)coordenadas[i].x;
        int y = (int)coordenadas[i].y;

        /* verifico se as coordenadas estao dentro da janela do jogo */
        if (x > x_janela && x < x_janela+largura_janela-1){
            if (y > y_janela && y < y_janela+altura_janela-1){
                gotoxy(x,y);
                putchar(178);
            }
        }


    }
}

void limpar_rastro_do_carro(CARRO carro){
    coord coordenadas[8];
    atribuir_as_coordenadas_do_carro(carro, coordenadas);


    for (int i = 0; i < 8; i++){
        int x = (int)coordenadas[i].x;
        int y = (int)coordenadas[i].y;

        /* verifico se as coordenadas estao dentro da janela do jogo */
        if (x > x_janela && x < x_janela+largura_janela-1){
            if (y > y_janela && y < y_janela+altura_janela-1){
                gotoxy(x,y);
                putchar(' ');
            }
        }
    }
}

void renderizar_carros_inimigos(CARRO *p, int quant){
    for (int i = 0; i < quant; i++){
        renderizar_carro(*(p+i));
    }
}
void limpar_rastro_dos_carros_inimigos(CARRO *p, int quant){
    for (int i = 0; i < quant; i++){
        limpar_rastro_do_carro(*(p+i));
    }
}
void updade_position_carros_inimigos(CARRO *carro, int quant){
    for (int i = 0; i < quant; i++){
        carro[i].coord.y += carro[i].velocity[1];
        carro[i].coord.x += carro[i].velocity[0];
    }
}

void apagar_carros(CARRO *p, int quant){
    for (int i = 1; i < quant; i ++){
        free(p+i);
    }
    free(p);
}
void draw_line(int x, int y){
    gotoxy(x, y);
    for (int i = 0; i < largura_janela-2; i++){
        putchar('-');
    }
}

void limpar_rastro_linha(int x, int y){
    gotoxy(x, y);
    for (int i = 0; i < largura_janela -2; i++){
        putchar(' ');
    }
}

int carro_colidiu(CARRO c, CARRO *p, int quant){
    coord Cordenadas_jogador[8];
    coord Cordenadas_carro_inimigo[8];
    atribuir_as_coordenadas_do_carro(c, Cordenadas_jogador);

    for (int i = 0; i < quant; i++){
        atribuir_as_coordenadas_do_carro(p[i], Cordenadas_carro_inimigo);

        for (int index_jogador = 0; index_jogador < 8; index_jogador++){
            for (int index_inimigo = 0; index_inimigo < 8; index_inimigo++){
                    /* se o carro sumiu da janela nao tem por que averiguar*/
                    if (Cordenadas_carro_inimigo[0].y > y_janela+altura_janela+3) break;

                    if (Cordenadas_jogador[index_jogador].x == (int)Cordenadas_carro_inimigo[index_inimigo].x &&
                        Cordenadas_jogador[index_jogador].y == (int)Cordenadas_carro_inimigo[index_inimigo].y){
                            return 1;
                    }
            }
        }
    }
    return 0;
}

void gameOver(){
    gotoxy(x_janela+largura_janela+6, y_janela);
    printf("GAME OVER ");
    gotoxy(x_janela+largura_janela+6, y_janela+1);
    printf("VOCE BATEU NO CARRO");
    gotoxy(x_janela+largura_janela+6, y_janela+1);
    printf("DIGITE QUALQUER TECLA PARA VOLTAR AO TELA INCIAL");
    getch();
}

float passagem_de_tempo(time_t tempo_antigo){
    return (float)((clock() - tempo_antigo)/CLOCKS_PER_SEC);
}

void exibir_status(int nivel){
    gotoxy(x_janela+largura_janela, y_janela+(altura_janela/2));
    printf("NIVEL ATUAL: %d", nivel);
    gotoxy(x_janela+largura_janela, y_janela+(altura_janela/2)+2);
    printf("(SE QUISER VOLTAR PARA O MENU APERTE e)");

}
int gameRun(int nivel_do_jogo){
    int y_line_finish = y_janela+1;
    int iniciar_animacao_da_linha_de_chegada = 0;
    int QUANT_CARROS_MAX = nivel_do_jogo*5;
    int quant_carros = 0; /* serve tb para fucionar como indice */
    time_t time_antigo;
    int tecla;
    float velocity[] = {0, 0};
    CARRO c;
    CARRO *carros;


    system("cls");
    srand(time(NULL));
    c = criar_carro(posx_inicial,posy_inicial,velocity);
    desenhar_borda_do_jogo();
    time_antigo = clock();

    carros = (CARRO *) malloc(sizeof(CARRO)*QUANT_CARROS_MAX); /* a cada nivel a quantide de carros vai crecer de 5 em 5*/

    while(1)
    {
        exibir_status(nivel_do_jogo);
        renderizar_carro(c);
        renderizar_carros_inimigos(carros, quant_carros);

        if (iniciar_animacao_da_linha_de_chegada){
            draw_line(x_janela+1, ++y_line_finish);

            if (y_line_finish == c.coord.y){
                return 1; /* cabo o jogo*/
            }
        }


        if (carro_colidiu(c, carros, quant_carros)){
                return 0;
        }


        Sleep(10);
        if (iniciar_animacao_da_linha_de_chegada) limpar_rastro_linha(x_janela+1, y_line_finish);
        limpar_rastro_do_carro(c);
        limpar_rastro_dos_carros_inimigos(carros, quant_carros);
        updade_position_carros_inimigos(carros, quant_carros);



        if (kbhit())
        {
            tecla = getch();

            switch(tolower(tecla))
            {
                case 'a':
                    if (c.coord.x-1 > x_janela+1) c.coord.x--;
                    break;

                case 'd':
                    if (c.coord.x+1 < x_janela+largura_janela-2) c.coord.x++;
                    break;

                case 'e':
                    return 0;
            }
        }

        if (passagem_de_tempo(time_antigo) >= 1.7){

            if (quant_carros < QUANT_CARROS_MAX){ /* inserir novo carro */
                time_antigo = clock();
                float velocity[] = {0, minimo(nivel_do_jogo*0.2, 1)}; // o maximo da velocidade e 1*/
                carros[quant_carros] = criar_carro(randint(x_janela+1, largura_janela-3),y_janela-4, velocity);
                quant_carros++;
            }else{
                /* verificar se o ultimo carro sumiu da janela*/
                if (carros[quant_carros-1].coord.y > y_janela+altura_janela+3){
                    iniciar_animacao_da_linha_de_chegada = 1;
                }
            }
        }

    }
}

int retorno_do_menu(){
    int x = x_janela-20, y = y_janela+20;
    int x_cursor = x+3, y_cursor = y+1;
    int tecla;
    int retorno = 1;
    system("cls");

    printf("DIGITE a tecla E para inserir a escolha\n");

    gotoxy(x, y);
    printf("JOGAR SAIR");
    gotoxy(x_cursor, y_cursor);
    putchar(219);

    while(1){
        tecla = tolower(getch());

        gotoxy(x_cursor, y_cursor);
        putchar(' ');

        switch(tecla){
            case 'a':
                if (x_cursor == x+3){
                    x_cursor = x+8;
                    retorno = 0;
                }
                else{
                    x_cursor = x+3;
                    retorno = 1;
                }
                break;

            case 'd':
                if (x_cursor == x+8){
                    x_cursor = x+3;
                    retorno = 1;
                }else{
                    x_cursor = x+8;
                    retorno = 0;
                }
                break;
            case 'e':
                return retorno;
        }

        gotoxy(x_cursor, y_cursor);
        putchar(219);
    }



}
int main(int argv, char *argc[]){
    int nivel_do_jogo = 1;
    int ganhou = 1;
    int perdeu = 0;
    int resposta;
    int running;

    running = retorno_do_menu();
    while(running){
        resposta = gameRun(nivel_do_jogo);

        if (resposta == ganhou){
            nivel_do_jogo++;
        }

        if (resposta == perdeu){
            gameOver();
            nivel_do_jogo = 1;
            running = retorno_do_menu();
        }

    }
}
