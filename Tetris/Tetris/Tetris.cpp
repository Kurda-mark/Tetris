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
#define Field  '\xB1' //������������ ����
#define Block  '\xFE' //������������ ������
#define Gran '\xB0'   //�������



void gotoxy(int xpos, int ypos); //������� ��������� ������� � ����� xpos, ypos
void game_menu();                //������� ����
int pause();                     //����� � ���� ��� ������� �������
int random(int number);          //������������ ��������� �����
void start_game();               //������ ����


class TGameTetris
{
public:
	int map[Sizey_X][Sizey_Y]{};       //������ ������
	int cells[4][4];                   //������
	int Xpos, Ypos;                    //���������� �����
	int score;                         //����
	int level;						   //�������

	TGameTetris();
	~TGameTetris();
	void display_map();              //����� ����� �� �����
	void print_next_map();           //����� ��������� ������ �� �����
	bool move_down(int x, int y);    //�������� ������ ����
	void rotate();                   //������� ������
	void delete_line();              //�������� ��������� �����
	void create_cells();             //�������� ������
	void clear_map();                //������� ����� 

private:
	int nextmap;                       //��������� ������

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

void TGameTetris::display_map() //����� ������� �� �����
{
	int i = 0, j = 0;
	int buffer[Sizey_X][Sizey_Y]{};

	for (i = 0; i < Sizey_Y; i++)          //����� �� ������ �� �����
		for (j = 0; j < Sizey_X; j++)
			buffer[j][i] = map[j][i];
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (cells[j][i])
				buffer[j + Xpos][i + Ypos] = 1; // ���� ����� ������ �� 1

	gotoxy(1, 1);

	for (i = 0; i < Sizey_Y; i++)
	{
		gotoxy(1, i);
		for (j = 0; j < Sizey_X; j++)
		{
			if (buffer[j][i] == 0)			//���� ������� � ������ �� ����� ������, �� ��������� ����� ����� ������
				printf("%c", Field);
			else
				printf("%c", Block);
		}
		printf("\n");
	}
	gotoxy(Sizey_X + 3, 1);
	printf("Score: %d", score); //������� ���������� �����, ���������� � ���� ����
	gotoxy(Sizey_X + 3, 3);
	printf("Level: %d", level); //������� �������
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

void TGameTetris::print_next_map() //���������� ��������� ������ ������ � ����
{
	int i = 0, j = 0;

	gotoxy(Sizey_X + 3, 5);
	printf("Next: ");

	for (i = 0; i < 4; i++)
	{
		gotoxy(Sizey_X + 4, i + 7);
		for (j = 0; j < 4; j++)
		{
			if (figures[nextmap][j][i] == 0)  //���� �� ����� ������, �� ��������� ���������, ����� ��������� �������
				printf(" ");
			else
				printf("%c", Block);
		}
	}
}

bool TGameTetris::move_down(int x, int y) //�������� ������ ����
{
	int i = 0, j = 0;
	if (x < 0)
		return 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			if (cells[j][i])
			{
				if ((j + x >= Sizey_X) || (i + y >= Sizey_Y))  // �������� �� ����� �� ������� �������
					return 0;
				else if (map[j + x][i + y] != 0)
					return 0;
			}
		}
	return 1;
}



void TGameTetris::rotate() //������� ������
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


void TGameTetris::delete_line() //���������� ��������� �����
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

void TGameTetris::create_cells() //�������� ������
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

void TGameTetris::clear_map() //������� ������
{
	int i = 0, j = 0;
	for (i = 0; i < Sizey_X; i++)
		for (j = 0; j < Sizey_Y; j++)
			map[i][j] = 0;
}

TGameTetris GameTetris; // �������� ���� ������

// --------------------- CLASS END ---------------------


int random(int number) //������������
{
	return (int)(rand() * number / RAND_MAX);
}

int pause()
{
	int c;
	if (_kbhit()) //���� ������ �������
	{
		if ((c = _getch()) == KEY_SPACE) //���� ������ ������� p (�����), �� ������ ����� ������� ��� ����������� ����
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

void gotoxy(int xpos, int ypos) //����������� ������� �������� �������� � ����� � ������������ xpos, ypos
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}



void start_game() //����
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

void game_menu()                      // ������� ����
{
	char key;
	const char* GAME_TITLE = "###########    #########   ###########     ########    #####      ##### \n" //���������
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