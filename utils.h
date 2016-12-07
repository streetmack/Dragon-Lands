#pragma once

#include <Windows.h>
#include <wchar.h>
#include "common.h"

// Функции для работы с консолью и текстом

int Rand(int _min,int _max);
bool Random(int probability=50); // Вероятность события в процентах (по умолчанию 50/50)
void Swap(int &a,int &b);
bool TestCoord(int x,int y);
int CalcDamage(int WeaponDmg);

void InitStdIO();
void Print(const wchar_t *text,WORD color,int x=-1,int y=-1,...);
void PrintChar(wchar_t chr,WORD color,int x=-1,int y=-1);
void PrintLog(const wchar_t *text,WORD color,...);
void PrintBorder();
void PrintStat(const wchar_t *text,int _y,WORD color,...);
void ClearLog();

const wchar_t* MonsterName(wchar_t type,wchar_t Case=L'N'); // N G D
const wchar_t* WeaponName(int WeaponType); // Тип оружия
void PrintPlayerDealDamage(wchar_t MonsterType,int WeaponType,int damage); // Урон герою
void PrintPlayerKillMonster(wchar_t MonsterType,int WeaponType,int exp);
void PrintMonsterDealDamage(wchar_t MonsterType,int damage); // Урон противнику