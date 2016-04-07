// ConsoleApplication1.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

using namespace std;

#define KEY_UP 72		// ����������� ������� "�����"
#define KEY_DOWN 80		// ����������� ������� "����"
#define KEY_LEFT 75		// ����������� ������� "�����"
#define KEY_RIGHT 77	// ����������� ������� "������"
#define KEY_SPACE 32	// ����������� ������� "������"
#define KEY_ESC 27		// ����������� ������� "escape"
#define KEY_ENTER 13	// ����������� ������� "enter"
#define Field '\xB0'	// ��������� ������ ����
#define Block '\xFE'	// ��������� ������
#define Sizey_X 15		// ������ �������
#define Sizey_Y 16		// ������ �������

int map[Sizey_Y][Sizey_X]{ };                           // �����
bool cells[4][4]{ };
int xp = 5, yp = 0;
bool flag = true;

void clearfigure(bool temp[4][4])                           // ������� ������
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			temp[i][j] = false;
}

void newfigure(int c)                                           // ������ ������
{
	clearfigure(cells);
	switch (c)
	{

	case 1:	for (int i = 0; i < 4; i++) 
				cells[0][i] = true;		
			break;						
										
	case 2:	for (int i = 0; i <= 1; i++)
				for (int j = 0; j <= 1; j++)
					cells[i][j] = true;
			break;

	case 3:	for (int i = 0; i <= 2; i++)
				cells[0][i] = true;
			cells[1][2] = true;
			break;

	case 4:	for (int i = 0; i <= 2; i++)
				cells[0][i] = true;
			cells[1][0] = true;
			break;

	case 5:	for (int i = 0; i <= 2; i++)
				cells[1][i] = true;
			cells[0][1] = true;
			break;

	case 6:	cells[0][0] = true;
			cells[1][0] = true;
			cells[1][1] = true;
			cells[2][1] = true;
			break;

	case 0:	cells[0][1] = true;
			cells[1][0] = true;
			cells[1][1] = true;
			cells[2][0] = true;
			break;

	default: 
		cout << "error function newfigure";
		break;
	}
}

void Rotate(int c)                                              //���������� ������  
{

	switch (c)
	{
	case 1:                                            // ��� ����� 2 �������� (�����,�����)
	{
		if (cells[0][0] == true)
		{
			clearfigure(cells);
			for (int i = 0; i<4; i++)
				cells[i][1] = true;
		}
		else
		{
			clearfigure(cells);
			for (int i = 0; i<4; i++)
				cells[0][i] = true;
		}
	}
	case 2: return;                                     // ��� ���� ��� �����������
	}

	bool tempfigure[4][4];                                  // �������� ���������� �������� ������� ��� ���������� ������
	clearfigure(tempfigure);                                // ������� ���������� ������� 
	for (int j = 4 - 1, c = 0; j >= 0; j--, c++)            // �������������� ������ �� 90 �������� �� ��������� �������
		for (int i = 0; i<4; i++)
			tempfigure[c][i] = cells[i][j];
	clearfigure(cells);                                     // ������� ����� ��� ������
	for (int i = 0; i<4; i++)                               // ������� ������ �� ���������� ������� � �������
		for (int j = 0; j<4; j++)
			cells[i][j] = tempfigure[i][j];
}

void move_down_clear()                     // 
{
	for (int y = yp; y < (yp + 4); y++)
		for (int x = xp; x < (xp + 4); x++)
			map[y][x] = 0;
}


void move_down()                     // �������� ����
{
	int z = 0, z1 = 0;
	for (int y = yp; y < (yp + 4); y++)
	{
		for (int x = xp; x < (xp + 4); x++)
		{
			map[y][x] = cells[z][z1];
			z1++;
		}
		z++;
		z1 = 0;
	}
		
}

void move_down_status() 
{
	if (xp < 0)
		xp = 0;
	if (xp > 12)
		xp = 12;
}


void displaymap()                                    //����� �����
{
	for (int i = 0; i < Sizey_Y; i++)
	{
		for (int j = 0; j < Sizey_X; j++)
		{
			if (map[i][j] == 0)
				cout << Field;
			else
				cout << Block;
		}
		cout << endl;
	}
}

void startgame()
{
	int c;
	int typefigure = 4;
	system("cls");
	newfigure(typefigure);
	while (TRUE)
	{
		if (_kbhit())
		{
			c = _getch();
			switch (c)
			{
			case KEY_LEFT : xp--; break;

			case KEY_RIGHT:	xp++; break;

			case KEY_UP: Rotate(typefigure);break;

			case KEY_DOWN: yp += 3;
			}
		}

		yp++;
		move_down_status();
		move_down();
		displaymap();
		Sleep(150);
		if (flag == true)
			move_down_clear();
	/*	else
		{
			xp = 5, yp = 0;
			typefigure = 5;//rand() % 6;
			newfigure(typefigure);
		}*/
		system("cls");
	}
}

void gamemenu()                      // ������� ����
{
	char c;
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

	system("cls"); printf("%s%s", GAME_TITLE, GAME_MENU);

	c = _getch();
	switch (c)
	{
	case '1': startgame();break;
	case '2': 
		system("cls");
		cout << "Version game 1.0 Kurda mark";
		_getch();
		break;
	case '3': exit(0);
	}
}                                    // ������� ����


int main()
{
	gamemenu();
	return 0;
}