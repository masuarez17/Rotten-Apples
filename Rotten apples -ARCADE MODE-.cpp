/*Autoria; Vargas Steeven,Pallango Andrés, ,Muñoz David, Suares Marcos, Tiamba Henrry*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>
#include <mmsystem.h>

#define TECLA_DERECHA 77
#define TECLA_IZQUIERDA 75
#define TECLA_ARRIBA 72
#define TECLA_ABAJO 80
#define TECLA_ENTER 13
#define Filas 7
#define Colum 10
#define TIME_PER_SECONDS 1000

#define INTENSITY 0x0008
#define BLACK 0x0000

#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

#define BACKGROUND_BLUE 0x0010
#define BACKGROUND_GREEN 0x0020
#define BACKGROUND_RED 0x0040
#define BACKGROUND_INTENSITY 0x0080

void iniciar_pantalla();
void actualizar();
void spawn();
void impresion();
void movimiento(int tecla);
void abrir_fichero(FILE *archivo);
void mejores_puntaje(FILE *archivo);
void juego_iniciar(FILE *puntuacion);
void creditos();
void como_jugar();
void load_screen();
int menu(const char *titulo, const char *opciones[], int n);
void gotoxy(int x,int y);
void welcome_screen();
void OcultarCursor();
void marcosSup(int x, int y, int tam);
void marcosInf(int x, int y, int tam);
void marcosLat(int x, int y, int tam);
bool tamanoPantalla(int Ancho, int Alto);
void dibujar(int color);
void musica(int op);
void configuracion_musica();

//En esta estructura se guardaran los puntajes, tiempos, y nicknames de los jugadores
struct score{
    char nom[100];
    char nom2[100];
    char nom3[100];
    float puntuacion_max[100];
    int minutos[100];
    int sec[100];
};

typedef struct score punt;

int vidas=3, tablero[Filas][Colum], mov=(Colum/2)-1, cont=0, cont1=0, seconds = 0, minutes = 0;
double puntaje = 0;
char nickname[3];

main(){
    //Se abre o se crea el fichero donde se guardaran los puntajes, tiempo y nombre de los jugadores
    FILE *registro_jugador;
    abrir_fichero(registro_jugador);
    OcultarCursor();
    tamanoPantalla(50,15);

    welcome_screen();
    //Opciones donde, dependiendo de la seleccion del jugador ejecutara una funcion
	while(1){
        HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTitle("ROTTEN APPLES");
        SetConsoleTextAttribute(ColorText, WHITE);
		int tecla = 6;
		int opcion;
		//Las opciones y titulo del menu
		const char *titulo = "MENU";
		const char *opciones[6] = {"JUGAR", "COMO JUGAR", "CREDITOS", "MEJORES PUNTAJES", "MUSICA", "SALIR"};
		opcion = menu(titulo, opciones, tecla);
		//Acciones a realizar si se escoge una de las opciones
		switch(opcion){
        case 1:
			system("cls");
			load_screen();
		    juego_iniciar(registro_jugador);
		    puntaje = 0;
		    vidas = 3;
		    seconds = 0;
		    minutes = 0;
		    break;
		case 2:
		    system("cls");
		    como_jugar();
		    break;
		case 3:
			system("cls");
		    creditos();
		    break;
        case 4:
			system("cls");
		    mejores_puntaje(registro_jugador);
		    break;
        case 5:
			system("cls");
		    configuracion_musica();
		    break;
		case 6:
		    system("cls");
		    creditos();
		    exit(0);
		    break;

		}
	}
}
//Se inicia la matriz que hara de tablero del jugador
void iniciar_pantalla(){
	for(int i=0;i<Filas;i++){
		for(int j=0;j<Colum;j++){
			tablero[i][j]=0;
			if(i == Filas-1 &&((Colum-1)/2)==j){
                tablero[i][j]=4;
                mov = (Colum-1)/2;
			}
		}
	}
}

void juego_iniciar(FILE *puntuacion){

    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
    clock_t play_t, inicio_t;

    int tecla, flag=0, counter=0, dificultad=0, spawnrate=0;
    int timeexternal = 1;
    int tutorial_mode = 1, instruccion_simple = 1;
	srand((unsigned int)time(NULL));
	iniciar_pantalla();
    //Se inicia el tiempo
	inicio_t = clock();
	//Comineza la partida
	do{
        play_t = clock()-inicio_t;
        SetConsoleTextAttribute(ColorText, WHITE);
        marcosSup(1,1,48);
        marcosInf(1,13,48);
        marcosLat(1,2,11);
        marcosLat(48,2,11);
        //Se registra la tecla que el usuario aplastara
		if(_kbhit()){
        	tecla=_getch();
        	if(tecla==TECLA_DERECHA || tecla==TECLA_IZQUIERDA){
        		movimiento(tecla);
			}
		}
		//Se imprime la pantalla
		impresion();
		//Aumento de dificultad
		if(puntaje<1000){
			dificultad=7;
			spawnrate=3;
			if(puntaje > 300){
				instruccion_simple = 0;
			}
		}
		if(puntaje>1000 && puntaje<3000){
			dificultad=5;
			spawnrate=2.5;
			tutorial_mode = 0;
		}
		if(puntaje>5000){
			dificultad=3;
			spawnrate=1;
		}
		//Se generan las manzanas
		if((play_t/TIME_PER_SECONDS)%spawnrate==0 && flag==0){
			spawn();
			flag=1;
		}
		if(flag==1 &&(play_t/TIME_PER_SECONDS)%3!=0){
			flag=0;
		}
		if(counter%dificultad==0){
			actualizar();
		}
		counter++;
		system("cls");

        if(play_t > TIME_PER_SECONDS*timeexternal){
            seconds +=1;
            timeexternal += 1;
            if(seconds > 59){
                minutes += 1;
                seconds = 0;
            }
        }
        if(tutorial_mode == 1){
        	SetConsoleTextAttribute(ColorText, INTENSITY);
            gotoxy(18, 12);
            printf("CALENTAMIENTO");
		}
		if(instruccion_simple == 1){
        	SetConsoleTextAttribute(ColorText, INTENSITY);
            gotoxy(33, 9);
            printf("<- ->");
            gotoxy(30, 10);
            printf("MOVIMIENTO");
		}


	}while(vidas>0);
	system("cls");

    SetConsoleTextAttribute(ColorText, WHITE);
    marcosSup(1,1,48);
    marcosInf(1,13,48);
    marcosLat(1,2,11);
    marcosLat(48,2,11);
	//Al final de la partida se muestran los puntajes y el tiempo
	SetConsoleTextAttribute(ColorText, WHITE);
	gotoxy(15,6);
	printf("TU PUNTAJE ES: %.0f\n", puntaje);
	gotoxy(15,7);
	if(minutes < 10 && seconds < 10){
        printf("TU TIEMPO ES: 0%d:0%d\n", minutes, seconds);
	} else if (minutes < 10){
	    printf("TU TIEMPO ES: 0%d:%d\n", minutes, seconds);
    } else  if (seconds < 10){
        printf("TU TIEMPO ES: %d:0%d\n", minutes, seconds);
    } else {
        printf("TU TIEMPO ES: %d:%d\n", minutes, seconds);
    }

    _getch();
    //Se pide el nombre el jugador
    do{
        system("cls");
        SetConsoleTextAttribute(ColorText, WHITE);
        marcosSup(1,1,48);
        marcosInf(1,13,48);
        marcosLat(1,2,11);
        marcosLat(48,2,11);
        gotoxy(15, 5);
        printf("INGRESA TU NICKNAME: ");
        gotoxy(23,8);
        printf("%c%c%c", 238,238,238);
        gotoxy(23,7);
        gets(nickname);
    }while(strlen(nickname)!=3);

    for(int i = 0; i < 3; i++){
        nickname[i] = toupper(nickname[i]);
    }
    //Se guarda todo en el fichero correspondiente
    puntuacion = fopen("puntajes.txt","at");
    fprintf(puntuacion, "%.0f %d:%d %s\n", puntaje, minutes, seconds, nickname);
    fclose(puntuacion);

	system("cls");
	mejores_puntaje(puntuacion);
}
//Actualizara el movimiento de las manzanas
void actualizar(){
	int aux=0;
	for(int i=Filas-1;i>=0;i--){
		for(int j=0;j<Colum;j++){

			if(tablero[i][j]==2 && i==Filas-2 && j==mov){
				vidas-=2;
			}
			if(tablero[i][j]==3 && i==Filas-2 && j==mov){
				vidas++;
			}
			if(tablero[i][j]==1 && i==Filas-2 && j==mov){
				puntaje+=100;
			}
			if(i!=Filas-1){
                if((tablero[i+1][j] == 4 && j == mov)){
                    tablero[i+1][j-1]=0;
                    tablero[i+1][j+1]=0;
                }else{
                    aux=tablero[i][j];
                    tablero[i][j]=0;
                    tablero[i+1][j]=aux;
                }
			}

		}
	}
}
//Genera las manzanas que caeran
void spawn(){
	int pos;
	pos=rand()%10;
	if(cont1!=2){
		if(cont==5){
			tablero[0][pos]=2;
			cont1++;
			cont=0;
		}
		else{
			tablero[0][pos]=1;
			cont++;
		}
	}
	else{
		tablero[0][pos]=3;
		cont1=0;
	}
}
//Imprime todo los que los jugadores veran
void impresion(){

/*    marcosSup(1,1,48);
    marcosInf(1,13,48);
    marcosLat(1,2,11);
    marcosLat(48,2,11);*/

    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(ColorText, WHITE);
    gotoxy(30,5);
    printf("Vidas: %d", vidas);
    gotoxy(30,6);
    printf("Puntaje: %.0f", puntaje);
    gotoxy(30,7);
    if(seconds < 10 && minutes < 10){
        printf("Tiempo: 0%d:0%d", minutes, seconds);
    } else if(seconds < 10){
        printf("Tiempo: %d:0%d", minutes, seconds);
    } else if(minutes < 10){
        printf("Tiempo: 0%d:%d", minutes, seconds);
    } else {
        printf("Tiempo: %d:%d", minutes, seconds);
    }

    /*marcosSup(9,3,Colum+2);
    marcosInf(9,Filas+3,Colum+2);*/
    marcosLat(9,4,Filas);
    marcosLat(9+Colum+1,4,Filas);

	for(int i=0;i<Filas;i++){
        gotoxy(10,4+i);
		for(int j=0;j<Colum;j++){
            if(tablero[i][j]==0){
				printf(" ");
            }else{
				if(tablero[i][j]==1){
                    SetConsoleTextAttribute(ColorText, LIGHTRED);
                    printf("%c", 224);
				}
				if(tablero[i][j]==2){
				    SetConsoleTextAttribute(ColorText, LIGHTMAGENTA);
					printf("%c", 228);
				}
				if(tablero[i][j]==3){
				    SetConsoleTextAttribute(ColorText, YELLOW);
                    printf("%c", 209);
				}
				if(tablero[i][j]==4){
				    SetConsoleTextAttribute(ColorText, LIGHTGREEN);
					printf("%c", 220);
				}
			}
		}
		printf("\n");
	}
}
//El movimiento del gusano sera de las teclas <- y ->
void movimiento(int tecla){
    switch(tecla){
    case TECLA_DERECHA:
        mov++;
        if(mov > Colum-1){
            mov = Colum-1;
        }
        break;
    case TECLA_IZQUIERDA:
        mov--;
        if(mov < 0){
            mov = 0;
        }
        break;
    }
    //El movimiento se almacena en una matriz
    tablero[Filas-1][mov] = 4;
}
//Se comprueba si el fichero existe con anterioridad, si no es asi lo crea
void abrir_fichero(FILE *archivo){

    if((archivo = fopen("puntajes.txt","rt")) == NULL){
        fclose(archivo);
        archivo = fopen("puntajes.txt","wt");
        //Imprime una secuencia de datos predefinidos para que no exista ningun error cuando sea abierto el fichero
        fprintf(archivo, "%.0f %d:%d %c%c%c\n", puntaje, minutes, seconds, '-','-','-','-');
        fclose(archivo);
        printf("Generando archivos");
        Sleep(500);
        printf(".");
        Sleep(500);
        printf(".");
        Sleep(500);
        printf(".");
        Sleep(500);
        system("cls");
        printf("Presione cualquier tecla para continuar");
        _getch();
    }
}
//Muestra los puntajes
void mejores_puntaje(FILE *archivo){
    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
    int contscore = 0;
    float aux;
    int auxsec, auxmin;
    char auxnom, auxnom2, auxnom3;

    SetConsoleTextAttribute(ColorText, WHITE);
    marcosSup(1,1,48);
    marcosInf(1,13,48);
    marcosLat(1,2,11);
    marcosLat(48,2,11);
    gotoxy(16,2);
    printf("MEJORES PUNTAJES");
    gotoxy(2,3);
    printf("----------------------------------------------");
    //Se inicia la variable para los puntajes, que seran almacenados en la estructura
    punt jugador;
    //Se inician las 5 primeras opciones de manera que si no hay puntaje se muestren datos predefinidos, asi mismo, se encera la variable
    for(int i = 0; i < 10; i++ ){
    	jugador.puntuacion_max[i] = 0;
    	jugador.sec[i] = 0;
    	jugador.minutos[i] = 0;
    	jugador.nom[i] = '-';
    	jugador.nom2[i] = '-';
    	jugador.nom3[i] = '-';
	}
    archivo = fopen("puntajes.txt","rt");
    //Se extraen los datos del fichero
    while((fscanf(archivo, "%f %d:%d %c%c%c", &jugador.puntuacion_max[contscore], &jugador.minutos[contscore], &jugador.sec[contscore], &jugador.nom[contscore], &jugador.nom2[contscore], &jugador.nom3[contscore]))!= EOF){
        contscore++;
        if(contscore > 99){
            contscore = 99;
        }
    }
    fclose(archivo);
    //Se organizan los puntajes de mayor a menor
    for(int j = 0; j < contscore; j++){
        for(int i = 0; i < contscore; i++){
            if(jugador.puntuacion_max[i] < jugador.puntuacion_max[i+1]){
                aux = jugador.puntuacion_max[i+1];
                jugador.puntuacion_max[i+1] = jugador.puntuacion_max[i];
                jugador.puntuacion_max[i] = aux;

                auxsec = jugador.sec[i+1];
                jugador.sec[i+1] = jugador.sec[i];
                jugador.sec[i] = auxsec;

                auxmin = jugador.minutos[i+1];
                jugador.minutos[i+1] = jugador.minutos[i];
                jugador.minutos[i] = auxmin;

                auxnom = jugador.nom[i+1];
                jugador.nom[i+1] = jugador.nom[i];
                jugador.nom[i] = auxnom;

                auxnom2 = jugador.nom2[i+1];
                jugador.nom2[i+1] = jugador.nom2[i];
                jugador.nom2[i] = auxnom2;

                auxnom3 = jugador.nom3[i+1];
                jugador.nom3[i+1] = jugador.nom3[i];
                jugador.nom3[i] = auxnom3;
            }
        }
    }
    //Si la cantidad de puntajes registrados supera 100, el fichero sera eliminado, y generado nuevamente
    //Guardando unicamente los primero 5 mejores puntajes
    if(contscore >= 99){
        archivo = fopen("puntajes.txt","wt");
        for(int i = 0; i < 5; i++){
            fprintf(archivo, "%.0f %d:%d %c%c%c\n", jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i], jugador.nom[i], jugador.nom2[i], jugador.nom3[i]);
        }
        fclose(archivo);
    }
    //Se imprimen los mejores 5 puntajes
    for(int i = 0; i < 5; i++){
        if((jugador.puntuacion_max[i] == puntaje)&&(jugador.sec[i] == seconds)&&(jugador.minutos[i] == minutes)&&(puntaje != 0)){

            if(jugador.minutos[i] < 10 && jugador.sec[i] < 10){
                SetConsoleTextAttribute(ColorText, LIGHTBLUE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f\tTiempo: 0%d:0%d\n", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            } else if(jugador.minutos[i] < 10){
                SetConsoleTextAttribute(ColorText, LIGHTBLUE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f\tTiempo: 0%d:%d\n", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            } else if(jugador.sec[i] < 10){
                SetConsoleTextAttribute(ColorText, LIGHTBLUE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f\tTiempo: %d:0%d\n", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            } else {
                SetConsoleTextAttribute(ColorText, LIGHTBLUE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f\tTiempo: %d:%d\n", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            }

        }else{
            if(jugador.minutos[i] < 10 && jugador.sec[i] < 10){
                SetConsoleTextAttribute(ColorText, WHITE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f  Tiempo: 0%d:0%d", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            } else if(jugador.minutos[i] < 10){
                SetConsoleTextAttribute(ColorText, WHITE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f  Tiempo: 0%d:%d", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            } else if(jugador.sec[i] < 10){
                SetConsoleTextAttribute(ColorText, WHITE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f  Tiempo: %d:0%d", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            } else {
                SetConsoleTextAttribute(ColorText, WHITE);
                gotoxy(3,(4+i)+i);
                printf("%d.- %c%c%c -> %.0f  Tiempo: %d:%d", i+1, jugador.nom[i], jugador.nom2[i], jugador.nom3[i], jugador.puntuacion_max[i], jugador.minutos[i], jugador.sec[i]);
            }
        }
        Sleep(500);
    }
    _getch();
}
//Impresion del menu
int menu(const char *titulo, const char *opciones[], int n){

    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
    int seleccion = 1;
    int tecla;
    bool repite = true;

    do{
        system("cls");
        SetConsoleTextAttribute(ColorText, WHITE);
        marcosSup(1,1,48);
        marcosInf(1,13,48);
        marcosLat(1,2,11);
        marcosLat(48,2,11);
        //Permitira mover el cursor
        SetConsoleTextAttribute(ColorText, INTENSITY);
        gotoxy(15, 4 + seleccion);
        printf(">");

        SetConsoleTextAttribute(ColorText, WHITE);
        gotoxy(22,3);
        printf("%s", titulo);

        for(int i = 0; i < n; i++){
            if(seleccion == i+1){
                SetConsoleTextAttribute(ColorText, INTENSITY);
                gotoxy(16, 5+i);
                printf("  %s",opciones[i]);
            } else {
                SetConsoleTextAttribute(ColorText, WHITE);
                gotoxy(16, 5+i);
                printf("%s",opciones[i]);
            }
        }

        do{
            tecla = _getch();
        }while(tecla != TECLA_ARRIBA && tecla != TECLA_ABAJO && tecla != TECLA_ENTER);

        switch(tecla){
        case TECLA_ARRIBA:
            seleccion--;

            if(seleccion == 0){
                seleccion = n;
            }
            break;
        case TECLA_ABAJO:
            seleccion++;
            if(seleccion > n){
                seleccion = 1;
            }
            break;
        case TECLA_ENTER:
            repite = false;
            break;
        }
        //Se va a ejecutar la ocion hasta que el ciclo haya concluido con un enter
    }while(repite);
    //Se regresa el valor de la seleccion
    return seleccion;
}

void creditos(){
    //Los integrantes del grupo
    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(ColorText, WHITE);
    marcosSup(1,1,48);
    marcosInf(1,13,48);
    marcosLat(1,2,11);
    marcosLat(48,2,11);
    gotoxy(17,2);
	printf("DESARROLLADO POR:");
	gotoxy(3,5);
	printf("Marcos Suarez");
	gotoxy(3,6);
    printf("David Munoz");
    gotoxy(3,7);
    printf("Steeven Vargas");
    gotoxy(3,8);
    printf("Henry Tiamba");
    gotoxy(3,9);
    printf("Pallango Andres");
    gotoxy(17,11);
    printf("GRACIAS POR JUGAR");
    gotoxy(0,14);
	_getch();
}

void como_jugar(){
    //Instrucciones basicas acerca de la dinamica del juego
    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(ColorText, WHITE);
    marcosSup(1,1,48);
    marcosInf(1,13,48);
    marcosLat(1,2,11);
    marcosLat(48,2,11);

    SetConsoleTextAttribute(ColorText, WHITE);
    gotoxy(19,2);
	printf("COMO JUGAR");
    gotoxy(2,5);
	printf("+ Mueve a Gusi usando las flechas(<- ->).");
    gotoxy(2,6);
	printf("+ Contaras con 3 vidas al inicio del juego.");
    gotoxy(2,7);
	printf("+ La manzana podrida, te quita 2 vidas.");
    gotoxy(2,8);
	printf("+ La manzana Dorada te da 1 vida.");
    gotoxy(2,9);
	printf("+ Las manzanas rojas te dan 100 puntos.");
	_getch();
}
//Pantalla de carga
void load_screen(){
    //Pantalla de carga, que muestra un pequeño consejo o intruccion sobre el juego
	HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(ColorText, WHITE);
    marcosSup(1,1,48);
    marcosInf(1,13,48);
    marcosLat(1,2,11);
    marcosLat(48,2,11);

    gotoxy(4,3);
    SetConsoleTextAttribute(ColorText, RED);
    printf("%c: Manzana ---> +100 Puntos",224);
    gotoxy(4,4);
    SetConsoleTextAttribute(ColorText, LIGHTMAGENTA);
	printf("%c: Manzana Podrida ---> -2 vidas",228);
    gotoxy(4,5);
    SetConsoleTextAttribute(ColorText, YELLOW);
    printf("%c: Manzana Dorada ---> +1 vidas",209);
    gotoxy(18,7);
    SetConsoleTextAttribute(ColorText, WHITE);
    printf("Cargando");

    gotoxy(17,8);
    for(int i = 0; i < 10; i ++){
        printf("%c", 220);
        Sleep(100);
    }

    system("cls");
}
//Pantalla de incio
void welcome_screen(){
    int conscreen = 0, conscreen2 = 0, tecla=0, secret = 0;
    int color = 0, musica_num = 1;
    srand((unsigned int)time(NULL));
    //Ejecucion de la musica de manera aletoria
    musica_num=rand()%6;
    musica(musica_num);
    //Eleccion del diseño de la manzana de manera aletoria
	color=rand()%3;
    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);

    do{
        system("cls");
        dibujar(color);
        SetConsoleTextAttribute(ColorText, WHITE);
        marcosSup(1,1,48);
        marcosInf(1,13,48);
        marcosLat(1,2,11);
        marcosLat(48,2,11);
        gotoxy(17, 7);
        printf("ROTTEN APPLES");

        if(secret == 114){
            SetConsoleTextAttribute(ColorText, INTENSITY);
            gotoxy(18, 12);
            printf("ARCADE MODE");
        }

        if(conscreen2 == 0){
            SetConsoleTextAttribute(ColorText, INTENSITY);
            gotoxy(16, 8);
            printf("-PRESIONA ENTER-");
            conscreen2++;
        } else {
            SetConsoleTextAttribute(ColorText, BLACK);
            gotoxy(16, 8);
            printf("-PRESIONA ENTER-");
            conscreen2 = 0;
        }

        Sleep(100);

        if(_kbhit()){
        	tecla = _getch();
        	if(tecla == TECLA_ENTER){
        		conscreen = 1;
			}
            if(tecla == 114){
                secret = 114;
            }
		}
		//la pantalla se mantendra hasta que el usuario presione enter
    }while(conscreen == 0);
}
//Dibujo de la manzana mostrada al incio
char manzana[12][21]={
        "       X       ",
        "        X      ",
        "       X       ",
        "   XXXXXXXXX    ",
        " XXXXXXXXXXXXX ",
        "XXXXXXXXXXXXXXX",
        "XXXXXXXXXXXXXXX",
        " XXXXXXXXXXXXX ",
        "   XXXXXXXXX   ",
        "     XXXXX     ",
};
//Diseño de la manzana mostrada al inicio
void dibujar(int color){
    HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);

    for(int i = 0; i < 12; i++){
        gotoxy(16, 2+i);
        for(int j = 0; j < 21; j++){
            if(color == 0){
                if(manzana[i][j]=='X'){
                    SetConsoleTextAttribute(ColorText, LIGHTGREEN);
                    printf("%c", 219);
                } else {
                    printf(" ");
                }
            } else if (color == 1){
                if(manzana[i][j]=='X'){
                    SetConsoleTextAttribute(ColorText, LIGHTRED);
                    printf("%c", 219);
                } else {
                    printf(" ");
                }
            } else if(color == 2){
                if(i >= 0 && i<3){
                    if(manzana[i][j]=='X'){
                    SetConsoleTextAttribute(ColorText, LIGHTGREEN);
                    printf("%c", 228);
                    } else {
                        printf(" ");
                    }
                } else {
                    if(manzana[i][j]=='X'){
                        if(i == 3){
                            if(j >= 5 && j <= 9){
                                SetConsoleTextAttribute(ColorText, LIGHTGREEN);
                                printf("%c", 228);
                            } else {
                                SetConsoleTextAttribute(ColorText, LIGHTRED);
                                printf("%c", 224);
                            }
                        } else {
                            SetConsoleTextAttribute(ColorText, LIGHTRED);
                            printf("%c", 224);
                        }
                    } else {
                        printf(" ");
                    }
                }
            }
        }
    }
}
//Musica que se reproducira de manera aletoria cuando el juego sea ejecutado
void musica(int op){
    if(op == 0){
        PlaySound(TEXT("Pistas\\RISE.wav"), NULL, SND_ASYNC | SND_LOOP);
    } else if (op == 1){
        PlaySound(TEXT("Pistas\\Thunderstruck.wav"), NULL, SND_ASYNC | SND_LOOP);
    } else if (op == 2){
        PlaySound(TEXT("Pistas\\Shelter.wav"), NULL, SND_ASYNC | SND_LOOP);
    } else if (op == 3){
        PlaySound(TEXT("Pistas\\Terminator.wav"), NULL, SND_ASYNC | SND_LOOP);
    } else if (op == 4){
        PlaySound(TEXT("Pistas\\The Phoenix.wav"), NULL, SND_ASYNC | SND_LOOP);
    } else if (op == 5){
        PlaySound(TEXT("Pistas\\High Hopes.wav"), NULL, SND_ASYNC | SND_LOOP);
    }
}
//Menu de configuracion donde el usuario puede elegir una de las pistas predefinidas o apagar la musica
void configuracion_musica(){

    int regresar = 0;

	while(regresar == 0){
        HANDLE ColorText = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTitle("ROTTEN APPLES");
        SetConsoleTextAttribute(ColorText, WHITE);
		int tecla = 8;
		int opcion;

		const char *titulo = "MUSICA";
		const char *opciones[8] = {"HIGH HOPES", "RISE", "SHELTER", "TERMINATOR THEME", "THE PHOENIX", "THUNDERSTRUCK", "APAGAR MUSICA", "REGRESAR"};
		opcion = menu(titulo, opciones, tecla);

		switch(opcion){
        case 1:
            PlaySound(NULL, 0, 0);
            musica(5);
		    break;
		case 2:
		    PlaySound(NULL, 0, 0);
		    musica(0);
		    break;
		case 3:
		    PlaySound(NULL, 0, 0);
		    musica(2);
		    break;
        case 4:
            PlaySound(NULL, 0, 0);
            musica(3);
		    break;
        case 5:
            PlaySound(NULL, 0, 0);
            musica(4);
		    break;
		case 6:
		    PlaySound(NULL, 0, 0);
		    musica(1);
		    break;
        case 7:
		    PlaySound(NULL, 0, 0);
		    break;
		case 8:
		    regresar = 1;
		    break;
		}
	}
}
//Marco superior
void marcosSup(int x, int y, int tam){
    for(int i = 0; i < tam; i++){
            if(i==0){
                gotoxy(x+i, y);
                printf("%c", 201);
            }else if(i == tam-1){
                gotoxy(x+i, y);
                printf("%c", 187);
            }else{
                gotoxy(x+i, y);
                printf("%c", 205);
        }
    }
}
//Marco Inferior
void marcosInf(int x, int y, int tam){
    for(int i = 0; i < tam; i++){
            if(i==0){
                gotoxy(x+i, y);
                printf("%c", 200);
            }else if(i == tam-1){
                gotoxy(x+i, y);
                printf("%c", 188);
            }else{
                gotoxy(x+i, y);
                printf("%c", 205);
        }
    }
}
//Marco lateral
void marcosLat(int x, int y, int tam){
    for(int i = 0; i < tam; i++){
        gotoxy(x, y+i);
        printf("%c", 186);
    }
}
//Cambia el tamaño de la consola
bool tamanoPantalla(int Ancho, int Alto){
	_COORD Coordenada;
	Coordenada.X = Ancho;
	Coordenada.Y = Alto;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Right = Ancho - 1;
	Rect.Bottom = Alto - 1;
	HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hConsola, Coordenada);
	SetConsoleWindowInfo(hConsola, TRUE, &Rect);
	return TRUE;
}
//Disposicion en X e Y
 void gotoxy(int x,int y){
      HANDLE hcon;
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);
      COORD dwPos;
      dwPos.X = x;
      dwPos.Y= y;
      SetConsoleCursorPosition(hcon,dwPos);
}
//Oculta el cursor
void OcultarCursor(){
    HANDLE Cursor = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(Cursor, &info);
}

