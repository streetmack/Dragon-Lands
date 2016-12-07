#pragma once

#include <conio.h>
#include "maps.h"
#include "utils.h"
#include "common.h"

class Dialog
{
protected:
	wchar_t text[255];
	wchar_t button;
public:
	Dialog() { wcscpy_s(text,255,L"[ошибка]");button=L'0'; }
};

void Init_Dialog(Player *player);
//void Blacksmith_Dialog();
void Healer_Dialog();
//void Merchant_Dialog();
//void Merchant2_Dialog();
//void Mayor_Dialog();
//void OldMan_Dialog();
//void Guard_Dialog();

//void Lose_Dialog();
//void Win_Dialog();