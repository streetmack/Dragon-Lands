#pragma once

#include <Windows.h>
#include "utils.h"
#include "common.h"

const WORD blacksmith_color=FG_WHITE|FG_INT;
const WORD healer_color=FG_GREEN;
const WORD merchant_color=FG_YELLOW|FG_INT;
const WORD merchant2_color=FG_YELLOW;
const WORD mayor_color=FG_RED|FG_INT;
const WORD oldman_color=FG_CYAN|FG_INT;
const WORD guard_color=FG_PURPLE|FG_INT;

class Map;

//базовый класс для динамических объектов (монстры, игрок, NPC)
class Actor
{
public:
	Map* map;
	wchar_t type;
	WORD color;
	int HP,MaxHP,Gold,level,exp;
	int WeaponDmg;
	int x,y;
	bool dead;

	Actor();
	bool TestCollision(int x,int y);

	virtual void Update() {}
	virtual void Render();
	virtual void Move(int x,int y) {}
	virtual void Die();
};

class Monster:public Actor
{
public:
	virtual void Update();
	virtual void Move(int x,int y);
	void Spawn(wchar_t type,int x,int y);
};

class Player:public Actor
{
public:
	int Armor,Wood,WeaponType,Strength,Potion;
	int time_idle,count_trolls,count_rats;
	bool bDialogMode,bAxe,bPick,bRelic,bVictory;
	WORD NPC_color;

	Player();
	bool CollisionMonster(int _x,int _y);//проверка на взаимодействие с монстрами (true если игрок с ним столкнулся)
	bool CollisionObject(int x,int y);

	void RenderStat();
	virtual void Update();
	virtual void Render();
	virtual void Move(int x,int y);

	void UpgradeWeapon();
	int ExpForLevelUp();
	int MinDamage(); // Минимальный урон
	int MaxDamage(); // Максимальный урон
	void Heal();
};

class Location
{
private:
	wchar_t wchars[map_width][map_height];
	WORD colors[map_width][map_height];
	Monster monster[max_monsters];
public:
	wchar_t GetChar(int x,int y);
	void SetChar(int x,int y,wchar_t chr,bool repaint=true);
	WORD GetColor(int x,int y);
	void SetColor(int x,int y,WORD color,bool repaint=true);
	void Repaint(int x,int y); // Перерисовывает символ
	void RepaintMap();
	void Replace3x3(int left,int top,wchar_t _old,wchar_t _new);
	//заменяет все символы _old в квадрате 3х3 и начальными координатами (left,top) на символы _new
	bool CollisionWorld(int x,int y); // вернет false если символ непроходим
	void Blood(int x,int y); // Кровавое пятно 3х3

	void UpdateMonsters();
	Actor* TestCollision(int x,int y);
	void SpawnMonster(wchar_t type,int x,int y);
};

class Map
{
private:
	Location loc[map_count_y][map_count_x];
	int map_x;
	int map_y; 
public:
	Player player;

	Map() { map_x=0;map_y=0; }
	Location& Loc() { return loc[map_x][map_y]; }
	bool Move(int x,int y);
	void SetCoor(int x,int y) { map_x=x;map_y=y; }
	void LoadMap(); // Загрузка карты
	void Update();
	bool IsBrick(int x,int y);
};

Map* GetMapInstance();