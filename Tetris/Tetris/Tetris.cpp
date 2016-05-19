// Tetris.cpp: ���������� ����� ����� ��� ����������� ����������.
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>

#define KEY_UP 72     //����������� ������� "�����"
#define KEY_DOWN 80   //����������� ������� "����"
#define KEY_LEFT 75   //����������� ������� "�����"
#define KEY_RIGHT 77  //����������� ������� "������"
#define KEY_SPACE 32  //����������� ������� "������"
#define KEY_ESC 27    //����������� ������� "escape"
#define Sizey_X 16    //������ �������
#define Sizey_Y 18    //������ �������
#define FMAP_COUNTS 7 //���������� ����� �������
#define Field  '\xB0' //������������ ����
#define Block  '\xFE' //������������ ������

void gotoxy(int xpos, int ypos); //�������� ������� ��������� ������� � ����� xpos, ypos
void game_menu();                //������� ����
void display_map();              //����� ����� �� �����
void print_next_map();           //����� ��������� ������ �� �����
int pause();                     //����� � ���� ��� ������� �������
bool move_down(int x, int y);    //����������� �������� ������ ����
void rotate();                   //������� ������
int random(int max);             //������������ ��������� �����
void sleep(int milsec);          //��������
void delete_line();              //�������� ��������� �����
void create_cells();             //�������� ������
void clear_map();                //������� ����� 
void start_game();               //������ ����

int map[Sizey_X][Sizey_Y]{}; //������ ������
int cells[4][4];                   //������
int px, py;                        //���������� �����
int nextmap;                       //��������� ������
int score;                         //����

int figures[FMAP_COUNTS][4][4] = //������������� ����� �������
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

void display_map() //����� ������� �� �����
{
	int i, j;
	int buffer[Sizey_X][Sizey_Y];

	for (i = 0; i < Sizey_Y; i++)          //����� �� ������ �� �����
		for (j = 0; j < Sizey_X; j++) 
			buffer[j][i] = map[j][i]; 
	for (i = 0; i < 4; i++) 
		for (j = 0; j < 4; j++) 
			if (cells[j][i]) 
				buffer[j + px][i + py] = 1; // ���� ����� ������, �� � ������ ���������� 1

	gotoxy(0, 0); //������� � ����� � ������������ 0, 0

	for (i = 0; i < Sizey_Y; i++)
	{
		for (j = 0; j < Sizey_X; j++)
		{
			putchar(buffer[j][i] == 0 ? Field : Block); //���� ������� � ������ �� ����� ������, �� ��������� �����
		}
		putchar('\n');
	}

	gotoxy(Sizey_X + 1, 0); //���� ����
	printf("Score: %i", score); //������� ���������� �����, ���������� � ���� ����
}

void print_next_map() //���������� ��������� ������ ������ � ����
{
	int i, j;

	gotoxy(Sizey_X + 1, 2);
	printf("Next: ");

	for (i = 0; i < 4; i++)
	{
		gotoxy(Sizey_X + 2, i + 3);
		for (j = 0; j < 4; j++)
		{
			printf("%c",figures[nextmap][j][i] == 0 ? ' ' : Block); //���� �� ����� ������, �� ��������� ���������, ����� ��������� �������
		}
	}
}

int pause()
{
	int c;
	if (_kbhit()) //���� ������ �������
	{
		if ((c = _getch()) == KEY_SPACE) c = _getch(); //���� ������ ������� p (�����), �� ������ ����� ������� ��� ����������� ����
		return c;
	}
	return 0;
}

void gotoxy(int xpos, int ypos) //����������� ������� �������� �������� � ����� � ������������ xpos, ypos
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

bool move_down(int x, int y) //������������� �������� ������ ����
{
	int i, j;
	if (x < 0) return 0;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (cells[j][i])
			{
				if ((j + x >= Sizey_X) || (i + y >= Sizey_Y)) return 0;
				if (map[j + x][i + y])
				{
					return 0;
				}
			}
		}
	}
	return 1;
}


int inv(int x)
{
	return ((x * (-1)) + 3);
}

void rotate() //������� ������
{
	int temp[4][4];
	int i, j, sx = 4, sy = 4;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			temp[j][i] = cells[j][i];
			if (cells[j][i] != 0)
			{
				if (i < sx) 
					sx = i;
				if (inv(j) < sy) 
					sy = inv(j);
			}
			cells[j][i] = 0;
		}

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (temp[inv(i)][j]) cells[j - sx][i - sy] = 1;

	if (!move_down(px, py)) 
		for (i = 0; i < 4; i++) 
			for (j = 0; j < 4; j++) 
				cells[j][i] = temp[j][i];
}

int random(int max) //������������
{
	max++;
	return (int)(rand() * max / RAND_MAX);
}

void sleep(int milsec) //��������
{
	clock_t t = clock();
	while (clock() - t < milsec);
}

void delete_line() //����� ��������� �����
{
	int i, j, k, cl;

	for (i = Sizey_Y - 1; i >= 0; i--)
	{
		cl = 1;
		for (j = 0, cl = 1; j < Sizey_X; j++)
		{
			if (!map[j][i]) cl = 0;
		}
		if (cl)
		{
			gotoxy(0, i);
			for (k = 0; k < Sizey_X; k++) putchar('_'), sleep(20);

			score += (((i * (-1)) + Sizey_Y) * 10);

			for (k = i; k > 0; k--)
			{
				for (j = 0; j < Sizey_X; j++)
				{
					map[j][k] = map[j][k - 1];
				}
			}
			i++;
			display_map();
		}
	}
}

void create_cells() //�������� ������
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			cells[i][j] = figures[nextmap][i][j];
	py = 0;
	px = Sizey_X / 2;

	nextmap = random(FMAP_COUNTS - 1);
	print_next_map();
}

void clear_map() //������� ������
{
	int i, j;
	for (i = 0; i < Sizey_X; i++)
		for (j = 0; j < Sizey_Y; j++)
			map[i][j] = 0;
}


void start_game() //����
{
	int i, j, key;
	time_t time;

	system("cls");
	px = Sizey_X / 2;
	py = 0;
	score = 0;
	

	time = clock();

	nextmap = random(FMAP_COUNTS - 1);
	create_cells();

	while (1)
	{
		key = pause();
		switch (key)
		{
		case KEY_UP:
			rotate();
			break;
		case KEY_DOWN:
			for (; move_down(px, py + 1); py++);
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++)
					if (cells[j][i]) map[px + j][py + i] = 1;
			display_map();
			delete_line();
			create_cells();
			break;
		case KEY_LEFT:
			if (move_down(px - 1, py)) px--;
			break;
		case KEY_RIGHT:
			if (move_down(px + 1, py)) px++;
			break;
		case KEY_ESC:
			return;
		}

		if ((clock() - time) > 320)
		{
			time = clock();

			if (!(move_down(px, py + 1)))
			{
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
						if (cells[j][i]) map[px + j][py + i] = 1;

				create_cells();
				delete_line();
			}
			else py++;
		}

		display_map();

		for (i = 0; i < Sizey_X; i++)
		{
			if (map[i][0])
			{
				system("cls");
				gotoxy(2, 8);
				printf("\tGame Over\n");
				return;
			}
		}
	}
}

void game_menu()                      // ������� ����
{
	char key;
	const char* GAME_TITLE = "###########    #########   ###########     #########    ###      ###     ####### \n" //���������
							 "#    #    #    #           #    #    #     #       #     #      # #    #         \n"
							 "#    #    #    #           #    #    #     #       #     #     #  #   #          \n"
							 "     #         #####            #          #########     #   ##   #   #          \n"
						     "     #         #                #          #             #  #     #    #         \n"
							 "     #         #                #          #             # #      #     #        \n"
							 "   #####       #########      #####       ###           ###      ###     ####### \n\n";

	const char* GAME_MENU = "\t 1. Start\n"
							"\t 2. Info\n"
							"\t 3. Exit";

	system("cls"); 
	printf("%s%s", GAME_TITLE, GAME_MENU);

	key = _getch();
	switch (key)
	{
	case '1': 
			  clear_map();
			  start_game(); 
			  printf("\tPress any key to exit the menu...");
			  _getch();
			  game_menu();
			  break;
	case '2':
			  system("cls");
			  printf("\n\tVersion game 1.0 Kurda mark\n");
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
