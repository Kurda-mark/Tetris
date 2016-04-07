// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

using namespace std;

#define KEY_UP 72		// Определение клавиши "вверх"
#define KEY_DOWN 80		// Определение клавиши "вниз"
#define KEY_LEFT 75		// Определение клавиши "влево"
#define KEY_RIGHT 77	// Определение клавиши "вправо"
#define KEY_SPACE 32	// Определение клавиши "пробел"
#define KEY_ESC 27		// Определение клавиши "escape"
#define KEY_ENTER 13	// Определение клавиши "enter"
#define Field '\xB0'	// Закрасить пустое поле
#define Block '\xFE'	// Закрасить кубики
#define Sizey_X 15		// Ширина колодца
#define Sizey_Y 16		// Высота колодца

int map[Sizey_Y][Sizey_X]{ };                           // Карта
bool cells[4][4]{ };
int xp = 5, yp = 0;
bool flag = true;

void clearfigure(bool temp[4][4])                           // Очистка фигуры
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			temp[i][j] = false;
}

void newfigure(int c)                                           // Вшитые фигуры
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

void Rotate(int c)                                              //Перевороты фигуры  
{

	switch (c)
	{
	case 1:                                            // Для линии 2 варианта (стоит,лежит)
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
	case 2: return;                                     // Для куба нет переворотов
	}

	bool tempfigure[4][4];                                  // Создание временного булевого массива для переворота фигуры
	clearfigure(tempfigure);                                // Очистка временного массива 
	for (int j = 4 - 1, c = 0; j >= 0; j--, c++)            // Переворачиваем фигуру на 90 градусов во временном массиве
		for (int i = 0; i<4; i++)
			tempfigure[c][i] = cells[i][j];
	clearfigure(cells);                                     // Очищаем масив для фигуры
	for (int i = 0; i<4; i++)                               // Заносим фигуру из временного массива в обычный
		for (int j = 0; j<4; j++)
			cells[i][j] = tempfigure[i][j];
}

void move_down_clear()                     // 
{
	for (int y = yp; y < (yp + 4); y++)
		for (int x = xp; x < (xp + 4); x++)
			map[y][x] = 0;
}


void move_down()                     // Движение вниз
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


void displaymap()                                    //Вывод карты
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

void gamemenu()                      // Игровое меню
{
	char c;
	const char* GAME_TITLE = "###########    #########   ###########     #########    ###      ###     ####### \n" //заголовок
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
}                                    // Игровое меню


int main()
{
	gamemenu();
	return 0;
}