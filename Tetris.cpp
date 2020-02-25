#include "Tetris.h"

Tetris::Tetris() : Par(24, 19)
{
	for (int i = 0; i < 4; i++) {
		figX[i] = figX_old[i] = 8;
		figY[i] = figY_old[i] = 2;
	}
	X_play_area_size = 15; //Требование ТЗ
	Y_play_area_size = Y_SIZE - 5;

	//Обрисовка контуром всех разделов 
	for (int x = 0; x < X_SIZE + 1; x++) {
		//Контур всего поля символом '#' по горизонтали
		SetChar(x, 0, L'#');
		SetChar(x, Y_SIZE, L'#');

		//Контур блока со счетом игры
		SetChar(x, Y_play_area_size + 1, L'#');
	}
	for (int y = 0; y < Y_SIZE + 1; y++) {
		//Контур всего поля символом '#' по вертикали
		SetChar(0, y, L'#');
		SetChar(X_SIZE, y, L'#');

		//Контур блока для превью следующей фигуры
		if (y <= Y_play_area_size)
			SetChar(X_play_area_size + 1, y, L'#');
	}
	//Заполнение игрового поля символом '-'
	for (int x = 1; x < X_play_area_size + 1; x++) {
		for (int y = 1; y < Y_play_area_size + 1; y++) {
			SetChar(x, y, '-');
		}
	}
	//Заполнение превью и счета
	string s = "Score: 0";
	for (int i = 0; i < s.length(); i++)
		SetChar(3 + i, Y_SIZE - 2, s[i]);
	cf = rand() % 5;
	nf = rand() % 5;
	ShowNextFig();

}

void Tetris::KeyPressed(int btnCode)
{
	if (btnCode == 75) { //стрелка вправо
		figX[0]--;
		Draw();
		if (!Check())
			figX[0]++;
	}
	else if (btnCode == 77) { //стрелка влево
		figX[0]++;
		Draw();
		if (!Check())
			figX[0]--;
	}
	else if (btnCode == 32) { //пробел
		int a = position;
		position = (position + 1) % 4;
		Draw();
		if (!Check())
			position = a;
	}
	else if (btnCode == 80) //стрелка вниз
		speed = 0.05;
}

void Tetris::Draw()
{
	if (cf == 3) position %= 2;
	if (cf == 4) position = 0;
	switch (position)
	{
	case 0:
		for (int i = 0; i < 4; i++) {
			figX[i] = figX[0] + fig[cf][0][i];
			figY[i] = figY[0] + fig[cf][1][i];
		}
		break;
	case 1:
		for (int i = 0; i < 4; i++) {
			figX[i] = figX[0] + fig[cf][1][i];
			figY[i] = figY[0] - fig[cf][0][i];
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++) {
			figX[i] = figX[0] - fig[cf][0][i];
			figY[i] = figY[0] - fig[cf][1][i];
		}
		break;
	case 3:
		for (int i = 0; i < 4; i++) {
			figX[i] = figX[0] - fig[cf][1][i];
			figY[i] = figY[0] + fig[cf][0][i];
		}
		break;
	default:
		break;
	}
}

void Tetris::UpdateF(float deltaTime)
{
	k += deltaTime;
	if (k >= speed) {
		k = 0;
		figY[0]++;
		clear_old_area();
	}
	Draw();
	if (!Check()) {
		figY[0]--;
		Draw();
		CreateFig();
		clear_fill_row();

		for (int i = 0; i < 4; i++) {
			figX[i] = figX_old[i] = 8;
			figY[i] = figY_old[i] = 2;
		}
		cf = nf;
		nf = rand() % 5;
		position = rand() % 4;
		speed = 0.5;

		ShowNextFig();
	}
	CreateFig();
}

void Tetris::CreateFig() {
	for (int i = 0; i < 4; i++) {
		SetChar(figX[i], figY[i], mainsymb);
		figX_old[i] = figX[i];
		figY_old[i] = figY[i];
	}
}

bool Tetris::Check() {
	clear_old_area();
	if (GetChar(8, 3) != '-') exit(0);
	for (int i = 0; i < 4; i++)
		if (GetChar(figX[i], figY[i]) != '-')
			return false;
}

void Tetris::ShowNextFig() {
	for (int i = 0; i < 4; i++)
		SetChar(X_SIZE - 4 + fig[cf][0][i], 6 + fig[cf][1][i], L' ');
	for (int i = 0; i < 4; i++)
		SetChar(X_SIZE - 4 + fig[nf][0][i], 6 + fig[nf][1][i], mainsymb);
}

void Tetris::clear_old_area() {
	for (int i = 0; i < 4; i++)
		SetChar(figX_old[i], figY_old[i], L'-');
}

void Tetris::clear_fill_row() {
	bool fl;
	for (int i = Y_play_area_size; i > 2; i--) {
		fl = true;
		for (int j = 1; j <= X_play_area_size; j++)
			if (GetChar(j, i) == '-') {
				fl = false;
				break;
			}
		if (fl) {
			for (int y = i; y > 1; y--)
				for (int x = 1; x <= X_play_area_size; x++)
					SetChar(x, y, GetChar(x, y - 1) == '-' ? '-' : mainsymb);
			i = Y_play_area_size + 1;
			score += 100;
		}
	}
	string s = to_string(score);
	for (int i = 0; i < s.length(); i++)
		SetChar(10 + i, Y_SIZE - 2, s[i]);
}