// Tetris.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>


#define KEY_UP 72     //определение клавиши "вверх"
#define KEY_DOWN 80   //определение клавиши "вниз"
#define KEY_LEFT 75   //определение клавиши "влево"
#define KEY_RIGHT 77  //определение клавиши "вправо"
#define KEY_SPACE 32  //определение клавиши "пробел"
#define KEY_ESC 27    //определение клавиши "escape"
#define Sizey_X 16    //ширина колодца
#define Sizey_Y 18    //высота колодца
#define FMAP_COUNTS 7 //количество фигур тетриса
#define Field  '\xB1' //закрашивание поля
#define Block  '\xFE' //закрашивание фигуры
#define Gran '\xB0'   //границы



void gotoxy(int xpos, int ypos); //функции помещения курсора в точку xpos, ypos
void game_menu();                //игровое меню
int pause();                     //пауза в игре при нажатии пробела
int random(int number);          //рандомизация выпадения фигур
void start_game();               //начало игры


class TGameTetris
{
public:
	int map[Sizey_X][Sizey_Y]{};       //размер экрана
	int cells[4][4];                   //фигура
	int Xpos, Ypos;                    //координаты фигур
	int score;                         //счет
	int level;						   //уровень

	TGameTetris();
	~TGameTetris();
	void display_map();              //вывод карты на экран
	void print_next_map();           //вывод следующей фигуры на экран
	bool move_down(int x, int y);    //движения фигуры вниз
	void rotate();                   //поворот фигуры
	void delete_line();              //удалении собранной линии
	void create_cells();             //создание фигуры
	void clear_map();                //очистка карты 

private:
	int nextmap;                       //следующая фигура

	int figures[FMAP_COUNTS][4][4] = //инициализация фигур тетриса
	{
		{
			{ 1, 1, 0, 0 },
			{ 1, 1, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 },
			{ 1, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0 },
			{ 1, 1, 1, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 1, 1, 1, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 0, 1, 1, 0 },
			{ 1, 1, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 1, 1, 0, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		},
		{
			{ 1, 1, 1, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 0, 0 },
			{ 0, 0, 0, 0 }
		}
	};
};

// --------------------- CLASS BEGIN ---------------------

TGameTetris::TGameTetris()
{
	Xpos = Sizey_X / 2;
	Ypos = 0;
	score = 0;
	nextmap = random(FMAP_COUNTS);
	level = 1;
}

TGameTetris::~TGameTetris()
{
}

void TGameTetris::display_map() //вывод колодца на экран
{
	int i = 0, j = 0;
	int buffer[Sizey_X][Sizey_Y]{};

	for (i = 0; i < Sizey_Y; i++)          //вывод из буфера на экран
		for (j = 0; j < Sizey_X; j++)
			buffer[j][i] = map[j][i];
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (cells[j][i])
				buffer[j + Xpos][i + Ypos] = 1; // если часть фигуры то 1

	gotoxy(1, 1);

	for (i = 0; i < Sizey_Y; i++)
	{
		gotoxy(1, i);
		for (j = 0; j < Sizey_X; j++)
		{
			if (buffer[j][i] == 0)			//если элемент в буфере не часть фигуры, то закрасить полем иначе блоком
				printf("%c", Field);
			else
				printf("%c", Block);
		}
		printf("\n");
	}
	gotoxy(Sizey_X + 3, 1);
	printf("Score: %d", score); //вывести количество очков, набираемое в ходе игры
	gotoxy(Sizey_X + 3, 3);
	printf("Level: %d", level); //вывести уровень
	for (i = 0; i < 18; i++) {
		gotoxy(Sizey_X + 1, i);
		printf("%c", Gran);
	}
	for (i = 0; i < 18; i++) {
		gotoxy(i, Sizey_Y);
		printf("%c", Gran);
	}
	for (i = 0; i < 18; i++) {
		gotoxy(0, i);
		printf("%c", Gran);
	}
	for (i = 18; i < 32; i++) {
		gotoxy(i, 0);
		printf("%c", Gran);
	}
	for (i = 0; i < 19; i++) {
		gotoxy(32, i);
		printf("%c", Gran);
	}
	for (i = 18; i < 32; i++) {
		gotoxy(i, Sizey_Y);
		printf("%c", Gran);
	}
}

void TGameTetris::print_next_map() //напечатать следующую идущую фигуру в поле
{
	int i = 0, j = 0;

	gotoxy(Sizey_X + 3, 5);
	printf("Next: ");

	for (i = 0; i < 4; i++)
	{
		gotoxy(Sizey_X + 4, i + 7);
		for (j = 0; j < 4; j++)
		{
			if (figures[nextmap][j][i] == 0)  //если не часть фигуры, то закрасить пробелами, иначе закрасить фигурой
				printf(" ");
			else
				printf("%c", Block);
		}
	}
}

bool TGameTetris::move_down(int x, int y) //движения фигуры вниз
{
	int i = 0, j = 0;
	if (x < 0)
		return 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			if (cells[j][i])
			{
				if ((j + x >= Sizey_X) || (i + y >= Sizey_Y))  // Проверка на выход за границы колодца
					return 0;
				else if (map[j + x][i + y] != 0)
					return 0;
			}
		}
	return 1;
}



void TGameTetris::rotate() //поворот фигуры
{
	int temp[4][4]{};
	int i = 0, j = 0, sx = 4, sy = 4;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			temp[j][i] = cells[j][i];
			if (cells[j][i] != 0)
			{
				if (i < sx)
					sx = i;
				if (((j * (-1)) + 3) < sy)
					sy = ((j * (-1)) + 3);
			}
			cells[j][i] = 0;
		}

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (temp[((i * (-1)) + 3)][j]) cells[j - sx][i - sy] = 1;

	if (!move_down(Xpos, Ypos))
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				cells[j][i] = temp[j][i];
}


void TGameTetris::delete_line() //уничтожить собранную линию
{
	int i = 0, j = 0, k = 0, c = 0;

	for (i = Sizey_Y - 1; i >= 0; i--)
	{
		c = 1;
		for (j = 0, c = 1; j < Sizey_X; j++)
		{
			if (!map[j][i]) c = 0;
		}
		if (c)
		{
			gotoxy(0, i);
			for (k = 0; k < Sizey_X; k++) putchar('_'), Sleep(20);
			score += (((i * (-1)) + Sizey_Y) * 10);
			for (k = i; k > 0; k--)
			{
				for (j = 0; j < Sizey_X; j++)
					map[j][k] = map[j][k - 1];
			}
			i++;
			display_map();
		}
	}
}

void TGameTetris::create_cells() //создание фигуры
{
	int i = 0, j = 0;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			cells[i][j] = figures[nextmap][i][j];
	Ypos = 0;
	Xpos = Sizey_X / 2;

	nextmap = random(FMAP_COUNTS);
	print_next_map();
}

void TGameTetris::clear_map() //очистка экрана
{
	int i = 0, j = 0;
	for (i = 0; i < Sizey_X; i++)
		for (j = 0; j < Sizey_Y; j++)
			map[i][j] = 0;
}

TGameTetris GameTetris; // Создание игры Тетрис

// --------------------- CLASS END ---------------------


int random(int number) //рандомизация
{
	return (int)(rand() * number / RAND_MAX);
}

int pause()
{
	int c;
	if (_kbhit()) //если нажата клавиша
	{
		if ((c = _getch()) == KEY_SPACE) //если нажата клавиша p (пауза), то нажать любую клавишу для продолжения игры
		{
			system("cls");
			gotoxy(0, 0);
			const char* PAUSE = "######     #    #     #  #####  ####### \n"
				"#     #   # #   #     # #     # #       \n"
				"#     #  #   #  #     # #       #       \n"
				"######  #     # #     #  #####  #####   \n"
				"#       ####### #     #       # #       \n"
				"#       #     # #     # #     # #       \n"
				"#       #     #  #####   #####  #######\n\n";
			printf("%s", PAUSE);
			c = _getch();
			system("cls");
			GameTetris.print_next_map();
		}
		return c;
	}
	return 0;
}

void gotoxy(int xpos, int ypos) //стандартная функция перехода курсором в точку с координатами xpos, ypos
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}



void start_game() //игра
{
	int i = 0, j = 0, pick = 0;
	int Ttime = 320;
	time_t time;
	system("cls");
	time = clock();
	GameTetris.clear_map();
	GameTetris.create_cells();
	GameTetris.score = 0;
	while (1)
	{
		pick = pause();
		switch (pick)
		{
		case KEY_UP:
			GameTetris.rotate();
			break;
		case KEY_DOWN:
			for (; GameTetris.move_down(GameTetris.Xpos, GameTetris.Ypos + 1); GameTetris.Ypos++);
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					if (GameTetris.cells[j][i]) GameTetris.map[GameTetris.Xpos + j][GameTetris.Ypos + i] = 1;
			GameTetris.display_map();
			GameTetris.delete_line();
			GameTetris.create_cells();
			break;
		case KEY_LEFT:
			if (GameTetris.move_down(GameTetris.Xpos - 1, GameTetris.Ypos)) GameTetris.Xpos--;
			break;
		case KEY_RIGHT:
			if (GameTetris.move_down(GameTetris.Xpos + 1, GameTetris.Ypos)) GameTetris.Xpos++;
			break;
		case KEY_ESC:
			return;
		}

		if ((clock() - time) > Ttime)
		{
			time = clock();

			if (!(GameTetris.move_down(GameTetris.Xpos, GameTetris.Ypos + 1)))
			{
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						if (GameTetris.cells[j][i])
							GameTetris.map[GameTetris.Xpos + j][GameTetris.Ypos + i] = 1;
				GameTetris.create_cells();
				GameTetris.delete_line();
			}
			else
				GameTetris.Ypos++;
			if (GameTetris.score < 50)
			{
				GameTetris.level = 1;
				Ttime = 320;
			}
			else if ((GameTetris.score > 50) && (GameTetris.score < 100)) {
				GameTetris.level = 2;
				Ttime = 160;
			}
			else if (GameTetris.score > 100)
			{
				GameTetris.level = 3;
				Ttime = 100;
			}

		}

		GameTetris.display_map();

		for (i = 0; i < Sizey_X; i++)
		{
			if (GameTetris.map[i][0])
			{
				system("cls");
				gotoxy(0, 0);
				const char* GAME_OVER = " #####     #    #     # #######    ####### #     # ####### ######         #    ##\n"
										"#     #   # #   ##   ## #          #     # #     # #       #     #       ###  #   \n"
										"#        #   #  # # # # #          #     # #     # #       #     #        #  #    \n"
										"#  #### #     # #  #  # #####      #     # #     # #####   ######            #    \n"
										"#     # ####### #     # #          #     #  #   #  #       #   #          #  #    \n"
										"#     # #     # #     # #          #     #   # #   #       #    #        ###  #   \n"
										" #####  #     # #     # #######    #######    #    ####### #     #        #    ##\n\n";
				printf("%s", GAME_OVER);
				return;
			}
		}
	}
}

void game_menu()                      // Игровое меню
{
	char key;
	const char* GAME_TITLE = "###########    #########   ###########     ########    #####      ##### \n" //заголовок
		"#    #    #    #           #    #    #     #      #      #       #     #\n"
		"#    #    #    #           #    #    #     #      #      #       #      \n"
		"     #         #####            #          ########      #        ##### \n"
		"     #         #                #          # #           #             #\n"
		"     #         #                #          #   #         #       #     #\n"
		"   #####       #########      #####       ###    #     #####	  ##### \n\n";

	const char* GAME_MENU = "\t 1. Start\n"
		"\t 2. Info\n"
		"\t 3. Exit\t\n";

	system("cls");
	printf("%s%s", GAME_TITLE, GAME_MENU);

	key = _getch();
	switch (key)
	{
	case '1':
		start_game();
		printf("\tPress any key to exit the menu...");
		_getch();
		game_menu();
		break;
	case '2':
		system("cls");
		printf("\n\tVersion game 0.98 Kurda mark\n\n");
		printf("\t\x1B       Left \n"
			"\t\x1A       Right\n"
			"\t\x19       Down\n"
			"\t\x18       Rotate\n"
			"\tSpace   Pause\n"
			"\tESC     Exit\n\n");
		printf("\tPress any key to exit the menu...");
		_getch();
		game_menu();
		break;
	case '3': exit(0);
	default: game_menu();
	}
}


void main(void)
{
	game_menu();
}