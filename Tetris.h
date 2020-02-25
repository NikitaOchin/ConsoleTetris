#pragma once
#include "BaseApp.h"

class Tetris : public BaseApp
{
private:
	typedef BaseApp Par;

	wchar_t mainsymb = L'⧈';
	double k = 0;//счетчик
	double speed = 0.5;
	int score = 0;

	int cf;//current figure
	int nf;//next figure
	int position = 0;//отвечает за повороты

	//Расположение элементов фигур, относительно центрального элемента. Последовательность фигур: |.:.|..:|:..|....|::|
	int fig[5][2][4] =
	{
		{ {0,-1,0,1}, {0,0,-1, 0} },
		{ {0,-1,1,1}, {0,0,0,-1} },
		{ {0,-1,-1,1}, {0,0,-1, 0} },
		{ {0,-1,1,-2}, {0,0,0, 0} },
		{ {0,-1,0,-1}, {0,0,-1, -1} }
	};
	//Расположение фигуры на игровом поле. Первые элементы массивов - центр фигуры.
	int figX[4];
	int figY[4];
	int figX_old[4];
	int figY_old[4];

	//Размер игрового поля
	int X_play_area_size;
	int Y_play_area_size;

	/* Функция отвечает за заполнения массивов figX и figY с помощью массива fig, отвечающий за
	относительное расположения фигур. Зависит от переменых: cf - номер текущей фигуры, и position - поворот фигуры */
	void Draw();
	//Функция заполняет массив символов на экране, элементами фигуры, с помощью функции SetChar.
	void CreateFig();
	//Проверяет, не налезет ли фигура на: другую фигуру, или на контур игрового поля.
	bool Check();
	//Показывает на экране следующую фигуру
	void ShowNextFig();
	//При изменении расположения фигуры - стирает с экрана её прошлое расположение
	void clear_old_area();
	//Убирает заполненную строку, при этом увеличивая счет (score) на 100 единиц 
	void clear_fill_row();

public:
	Tetris();
	virtual void KeyPressed(int btnCode);
	virtual void UpdateF(float deltaTime);
};

