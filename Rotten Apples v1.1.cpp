/*Autoria; Vargas Steeven,Pallango Andrés, ,Muños Francisco, Suares Marcos, Tiamba Henrry*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define TECLA_DERECHA 77
#define TECLA_IZQUIERDA 75
#define Filas 7
#define Colum 10
#define Segundos 1000

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

struct score{
    char nombre[100];
    float puntuacion_max[100];
    float minutos[100];
    float sec[100];
};

typedef struct score punt;

int vidas=3, tablero[Filas][Colum], mov=4, cont=0, cont1=0;
double puntaje = 0;
clock_t start_t;

int menu(const char *titulo, const char *opciones[], int n);
void gotoxy(int x,int y);

main(){

    FILE *puntuacion;
    abrir_fichero(puntuacion);
	while(1){
		SetConsoleTitle("ROTTEN APPLE");
		int tecla = 5;
		int opcion;

		const char *titulo = "Rotten apple";
		const char *opciones[5] = {"JUGAR", "COMO JUGAR", "CREDITOS", "MEJORES PUNTAJES", "SALIR"};
		opcion = menu(titulo, opciones, tecla);

		switch(opcion){
        case 1:
			system("cls");
			load_screen();
		    juego_iniciar(puntuacion);
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
			printf("MEJORES PUNTAJES\n\n");
			printf("--------------------------------\n");
		    mejores_puntaje(puntuacion);
		    break;
		case 5:
		    system("cls");
		    creditos();
		    printf("\n\n GRACIAS POR JUGAR \n\n");
		    exit(0);
		    break;

		}
	}
}

	


void iniciar_pantalla(){
	for(int i=0;i<Filas;i++){
		for(int j=0;j<Colum;j++){
			tablero[i][j]=0;
			if(i == Filas-1 &&((Colum-1)/2)==j){
                tablero[i][j]=4;
			}
		}
	}
}

void juego_iniciar(FILE *puntuacion){
    int tecla, flag=0, counter=0, dificultad=0, spawnrate=0;
    bool ban=true;
	srand((unsigned int)time(NULL));
	iniciar_pantalla();
	do{
		if(kbhit()){
        	tecla=_getch();
        	if(tecla==TECLA_DERECHA || tecla==TECLA_IZQUIERDA){
        		movimiento(tecla);
			}
		}
		impresion();
		start_t = clock()/Segundos;
		if(puntaje<1000){
			dificultad=7;
			spawnrate=3;
		}
		if(puntaje>1000 && puntaje<3000){
			dificultad=5;
			spawnrate=2.5;
		}
		if(puntaje>5000){
			dificultad=3;
			spawnrate=1;
		}
		if(start_t%spawnrate==0 && flag==0){
			spawn();
			flag=1;
		}
		if(flag==1 && start_t%3!=0){
			flag=0;
		}
		ban=!ban;
		if(counter%dificultad==0){
			actualizar();
		}
		counter++;
		system("cls");
	}while(vidas>0);
	system("cls");
	printf("Tu puntaje es: %.0f\n", puntaje);
	printf("Tu tiempo es: %ld\n", start_t);
	_getch();
	system("cls");
	printf("\nLos mejores puntajes son: \n");
	mejores_puntaje(puntuacion);
}

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

void impresion(){
    printf("vidas: %d     Puntaje: %.0f\n\n", vidas, puntaje);
    printf("Tiempo: %ld\n", start_t);
	for(int i=0;i<Filas;i++){
		for(int j=0;j<Colum;j++){
            if(tablero[i][j]==0){
				printf(" ");
            }else{
				if(tablero[i][j]==1){
                    printf("%c", 224);
				}
				if(tablero[i][j]==2){
					printf("%c", 228);
				}
				if(tablero[i][j]==3){
					printf("%c", 209);
				}
				if(tablero[i][j]==4){
					printf("%c", 220);
				}
			}
		}
		printf("\n");
	}
}

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
    tablero[Filas-1][mov] = 4;
}

void abrir_fichero(FILE *archivo){

    if((archivo = fopen("puntajes.txt","rt")) == NULL){
        fclose(archivo);
        archivo = fopen("puntajes.txt","wt");
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

void mejores_puntaje(FILE *archivo){
    int contscore = 0;
    float aux, auxsec;
    char auxnom;
    punt jugador;

    archivo = fopen("puntajes.txt","at");
    fprintf(archivo, "%.0f %.0f\n", puntaje, start_t);
    fclose(archivo);
    for(int i = 0; i < 5; i++ ){
    	jugador.puntuacion_max[i] = 0;
    	jugador.sec[i]=0;
	}
    archivo = fopen("puntajes.txt","rt");
    while((fscanf(archivo, "%f %f", &jugador.puntuacion_max[contscore], &jugador.sec[contscore]))!= EOF){
        contscore++;
    }
    fclose(archivo);
    for(int j = 0; j < contscore; j++){
        for(int i = 0; i < contscore; i++){
            if(jugador.puntuacion_max[i] < jugador.puntuacion_max[i+1]){
                aux = jugador.puntuacion_max[i+1];
                jugador.puntuacion_max[i+1] = jugador.puntuacion_max[i];
                jugador.puntuacion_max[i] = aux;

                auxsec = jugador.sec[i+1];
                jugador.sec[i+1] = jugador.sec[i];
                jugador.sec[i] = auxsec;
            }
        }
    }

    for(int i = 0; i < 5; i++){
        if(jugador.puntuacion_max[i] == puntaje && puntaje != 0){
            printf("%d.- %.0f\n\tTiempo:%.0fsec<--\n", i+1, jugador.puntuacion_max[i], jugador.sec[i]);
            printf("--------------------------------\n");
        }else{
            printf("%d.- %.0f\n\tTiempo:%.0fsec\n", i+1, jugador.puntuacion_max[i], jugador.sec[i]);
            printf("--------------------------------\n");
        }
        Sleep(500);
    }
    _getch();
    printf("\nPresiona una tecla");
}

int menu(const char *titulo, const char *opciones[], int n){
    int seleccion = 1;
    int tecla;
    bool repite = true;

    do{
        system("cls");
        gotoxy(8, 3 + seleccion);printf(">");

        gotoxy(15,2);printf("%s", titulo);

        for(int i = 0; i < n; i++){
            gotoxy(10, 4+i);printf("%s",opciones[i]);
        }

        do{
            tecla = _getch();
        }while(tecla != 72 && tecla != 80 && tecla != 13);

        switch(tecla){
        case 72:
            seleccion--;

            if(seleccion == 0){
                seleccion = n;
            }
            break;
        case 80:
            seleccion++;
            if(seleccion > n){
                seleccion = 1;
            }
            break;
        case 13:
            repite = false;
            break;
        }

    }while(repite);

    return seleccion;
}

 void gotoxy(int x,int y){
      HANDLE hcon;
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);
      COORD dwPos;
      dwPos.X = x;
      dwPos.Y= y;
      SetConsoleCursorPosition(hcon,dwPos);
}

void creditos(){

	printf("\nDesarrollado por:\n");
	printf("/-David Munoz\n /-Pallango Andres\n /-Marcos Suarez\n /-Henry Tiamba\n /-Steeven Vargas\n");
	_getch();
}

void como_jugar(){

	printf("\nComo jugar\n");
	printf("\n -> Mueve  a Gusi el gusanito utilizando las flechas del teclado (<- ->)\n");
	printf("\n -> Contaras con 3 vidas al inicio del juego\n");
	printf("\n -> Con los flechas esquiva las manzanas danadas para no perder vida\n");
	printf("\n -> Si llegas a cojer una manzana danada se restara 2 vidas del  total de vidas\n ");
	printf("\n -> Si llegas a cojer una manzana dorada se te sumara 1 vida al total de vidas \n");
	printf("\n -> Si dejas caer manzanas normales (rojas) no pasa nada, solo ten cuidado de las manzanas danadas\n");

	printf("\n\nPresione cuaquier tecla para regresar al menu principal");
	_getch();
}

void load_screen(){

    printf("\n%c: Manzana ---> +100 Puntos",224);
    printf("\n%c: Manzana Podrida ---> -2 vidas",228);
    printf("\n%c: Manzana Dorada ---> +1 vidas\n",209);

    printf("\n\tCargando\n\t");

    for(int i = 0; i < 8; i ++){
        if(i!= 7 && i != 0){
            printf("-");
            Sleep(500);
        }else if(i == 7){
            printf(">");
            Sleep(500);
        } else {
            printf("<");
            Sleep(250);
        }
    }

    system("cls");
}
