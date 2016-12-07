#include "dialogs.h"

Player *g_player=0;

bool blacksmith_q=true;
bool merchant_q=true;
bool mayor_q=true;
bool oldman_q=true;
bool guard_q=true;

bool open_rats=false;
int drugs=0;

void Init_Dialog(Player *player)
{
	g_player=player;
}

bool Pay(int cost)
{
	if(g_player->Gold<cost)
	{
		PrintLog(L"������������ ������",FG_RED|FG_INT);
		return false;
	}
	g_player->Gold-=cost;
	PrintLog(L"�� ��������� %d �������!",FG_WHITE,cost);
	return true;
}

void Blacksmith_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int k=L'A';
	while(k!=L'0')
	{
		int WeaponCost=g_player->WeaponDmg*(6*g_player->WeaponDmg);
		int ArmorCost=g_player->Armor*(8*g_player->Armor);

		int str=1;
		Print(L"������",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: �������� ������ - %d �������",FG_GREEN,1,str++,WeaponCost);
		Print(L"2: �������� ������� - %d �������",FG_GREEN,1,str++,ArmorCost);
		Print(L"3: ����������",FG_GREEN,1,str++);
		if(blacksmith_q)Print(L"4: �����",FG_GREEN,1,str++);
		else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: �����",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':if(Pay(WeaponCost))g_player->UpgradeWeapon();break;
			case L'2':if(Pay(ArmorCost))g_player->Armor++;break;
			case L'3':
			{
				PrintLog(L"������ �������:",FG_WHITE|FG_INT);
				if(blacksmith_q)
				{
					PrintLog(L"�� � ��� ��� ��������... ��� ����� �������",FG_WHITE);
					PrintLog(L"������ ����� �������, ������� ��� �������, �������� ����������. �������!",FG_WHITE);
				}else PrintLog(L"�� ������ ������ �����... ������!",FG_WHITE);
				break;
			}
			case L'4':
			{
				if(!blacksmith_q)break;
				if(g_player->count_trolls<10)
				{
					PrintLog(L"������ ���� �������:",FG_WHITE|FG_INT);
					PrintLog(L"������� ������ ������� ��� ��� ������� �� ����.",FG_WHITE);
					PrintLog(L"���� ���� ��� ������ ������ � ����������� �� ��������",FG_WHITE);
				}else{
					PrintLog(L"����� ������� ��������!",FG_YELLOW|FG_INT);
					PrintLog(L"�� ���� ���� �������! �� �������� ������� ��� � ��������.",FG_WHITE);
					PrintLog(L"� �������� ������� � ��������� ������ ���� ����������",FG_WHITE);
					PrintLog(L"+1 � ������, +1 � ��������, +100 �������!",FG_YELLOW|FG_INT);
					g_player->UpgradeWeapon();g_player->Armor++;g_player->Gold+=100;
					blacksmith_q=false;
				}
				break;
			}
			case L'0':return;
		}
		g_player->RenderStat();
	}
}

void Healer_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int k=L'A';
	while(k!=L'0')
	{
		int PotionCost=g_player->level*5+(g_player->Potion*5);
		int HealCost=g_player->level*5;

		int str=1;
		Print(L"������",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: ������ ����� - %d �������",FG_GREEN,1,str++,PotionCost);
		Print(L"2: ��������� - %d �������",FG_GREEN,1,str++,HealCost);
		Print(L"3: ����������",FG_GREEN,1,str++);
		if(drugs==1)Print(L"4: ���������",FG_GREEN,1,str++);
		else Print(L"                ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: �����",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':if(Pay(PotionCost))g_player->Potion++;break;
			case L'2':
			{
				if(g_player->HP<g_player->MaxHP)if(Pay(HealCost))g_player->HP=g_player->MaxHP;
				break;
			}
			case L'3':
			{
				PrintLog(L"������ �������:",FG_WHITE|FG_INT);
				PrintLog(L"������ ������ � ����� ����������� ���������� �����",FG_WHITE);
				PrintLog(L"�� ������ ������, ����� ��������� ����� ������� ������...",FG_WHITE);
				break;
			}
			case L'4':
			{
				PrintLog(L"�� ����� ��������� ��� �������.",FG_YELLOW|FG_INT);
				drugs=2;
				break;
			}
			case L'0':return;
		}
		g_player->RenderStat();
	}
}

void Merchant_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int k=L'A';
	while(k!=L'0')
	{
		int WeaponCost=g_player->WeaponDmg*(6*g_player->WeaponDmg);
		int PotionCost=g_player->level*5+(g_player->Potion*5);
		int StrengthCost=g_player->Strength*(3*g_player->Strength);

		int str=1;
		Print(L"��������",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: �������� ������ - %d �������",FG_GREEN,1,str++,WeaponCost);
		Print(L"2: ������ ����� - %d �������",FG_GREEN,1,str++,PotionCost);
		Print(L"3: �������� ���� - %d �������",FG_GREEN,1,str++,StrengthCost);
		Print(L"4: ����������",FG_GREEN,1,str++);
		if(merchant_q)Print(L"5: �����",FG_GREEN,1,str++);
		else Print(L"5: ������� ��� ���������",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: �����",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':if(Pay(WeaponCost))g_player->UpgradeWeapon();break;
			case L'2':if(Pay(PotionCost))g_player->Potion++;break;
			case L'3':if(Pay(StrengthCost))g_player->Strength++;break;
			case L'4':
			{
				PrintLog(L"�������� �������:",FG_WHITE|FG_INT);
				if(merchant_q)
				{
					PrintLog(L"��-�� �������� � �� ���� ������������� ���������.",FG_WHITE);
					PrintLog(L"��� ����� ��� ��� ��������� ���������",FG_WHITE);
				}
				else PrintLog(L"� ������ ������, ��� ��� �� ������� ������������� ����������",FG_WHITE);
				break;
			}
			case L'5':
			{
				if(!merchant_q)
				{
					if(g_player->Wood<1)break;
					int money=g_player->Wood*5;
					g_player->Gold+=money;
					g_player->Wood=0;
					PrintLog(L"������ ��� ������, �� ���������� %d �������",FG_YELLOW|FG_INT,money);
					break;
				}
				if(g_player->Wood<20)
				{
					PrintLog(L"�������� ���� �������:",FG_WHITE|FG_INT);
					PrintLog(L"������� ������ ��� ����. � ������ ������, ����� ��������� �����",FG_WHITE);
					PrintLog(L"������� ��� 20 ��. ���������, � ������ ����� ������������",FG_WHITE);
					if(!g_player->bAxe)PrintLog(L"�������� ���� ���� �����. ������ �� ������ ������� �������",FG_YELLOW|FG_INT);
					g_player->bAxe=true;
				}else{
					PrintLog(L"����� �������� ��������!",FG_YELLOW|FG_INT);
					PrintLog(L"�����������, ����� ���������� ��� ��� ��� ������ �� �������������",FG_WHITE);
					PrintLog(L"� ����� ���� ���������� �������� ���� � ���� ������� ��� ������",FG_WHITE);
					PrintLog(L"�� ��������� +1 � ���� � 150 �������!",FG_YELLOW|FG_INT);
					g_player->Wood-=20;
					g_player->Gold+=150;
					g_player->Strength++;
					merchant_q=false;
				}
				break;
			}
			case L'0':return;
		}
		g_player->RenderStat();
	}
}

void Merchant2_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int k=L'A';
	while(k!=L'0')
	{
		int WeaponCost=g_player->WeaponDmg*(6*g_player->WeaponDmg);
		int StrengthCost=g_player->Strength*(3*g_player->Strength);

		int str=1;
		Print(L"��������",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: �������� ������ - %d �������",FG_GREEN,1,str++,WeaponCost);
		Print(L"2: �������� ���� - %d �������",FG_GREEN,1,str++,StrengthCost);
		Print(L"3: ����������",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: �����",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':if(Pay(WeaponCost))g_player->UpgradeWeapon();break;
			case L'2':if(Pay(StrengthCost))g_player->Strength++;break;
			case L'3':
			{
				PrintLog(L"�������� �������:",FG_WHITE|FG_INT);
				PrintLog(L"���� ��� ���� ��������, ������� �� ����� ������� ���������",FG_WHITE);
				PrintLog(L"�������� ��� ��������, � ������� ������� �������������",FG_WHITE);
				break;
			}
			case L'0':return;
		}
		g_player->RenderStat();
	}
}

void Mayor_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int k=L'A';
	while(k!=L'0')
	{
		int str=1;
		Print(L"��� ������",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: �������� � ������",FG_GREEN,1,str++);
		Print(L"2: �������� � ���������",FG_GREEN,1,str++);
		Print(L"3: �������� � �������",FG_GREEN,1,str++);
		if(mayor_q)Print(L"4: �����",FG_GREEN,1,str++);else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: �����",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':
			{
				PrintLog(L"��� �������:",FG_WHITE|FG_INT);
				PrintLog(L"� ��� ��������� � ��������� �������.",FG_WHITE);
				if(mayor_q)
				{
					PrintLog(L"�� � ��������� ����� ����� ������� � �������",FG_WHITE);
					PrintLog(L"������ ���, �������� ��������, ����� ��� ������...",FG_WHITE);
					PrintLog(L"��� ������� �������� �� �����������.",FG_WHITE);
				}
				else
				{
					PrintLog(L"� ��� ��� ��� �� �������� �� ����� �����, ���� ������������",FG_WHITE);
					PrintLog(L"���� ���� � ���������, ��� ��� ���� ��� ������...",FG_WHITE);
					PrintLog(L"������ �� ������ ������� ����� ������� ������ �� ���������� =(",FG_WHITE);
				}
				break;
			}
			case L'2':
			{
				PrintLog(L"��� �������:",FG_WHITE|FG_INT);
				PrintLog(L"� �������� ����� �������� ������, � ������ - ��������� �����",FG_WHITE);
				PrintLog(L"��� ��� ������, �� �� ������ �� �����, � ����. � ����� ����� � �������",FG_WHITE);
				break;
			}
			case L'3':
			{
				PrintLog(L"��� �������:",FG_WHITE|FG_INT);
				PrintLog(L"������� - ����� ������� ��������",FG_WHITE);
				PrintLog(L"����� ��������� � ���, ���� ��������� ��� ������� 10 �������.",FG_WHITE);
				break;
			}
			case L'4':
			{
				if(!mayor_q)break;
				if(g_player->count_rats<12)
				{
					PrintLog(L"��� ���� �������:",FG_WHITE|FG_INT);
					PrintLog(L"����� ���������� �����, ������ ������� � �������� ����",FG_WHITE);
					PrintLog(L"���� �� ����, � �������� ������ ��������������",FG_WHITE);
					if(!open_rats)
					{
						Map *map=GetMapInstance();
						map->Loc().SetChar(20,6,L' ');
						map->Loc().SetChar(16,12,L' ');
						map->Loc().SetChar(40,12,L' ');
						open_rats=true;
					}
				}else{
					PrintLog(L"����� ���� ��������!",FG_YELLOW|FG_INT);
					PrintLog(L"�����������, �� ����� ����� �� ��������!",FG_WHITE);
					PrintLog(L"�� ��������� 100 �������!",FG_YELLOW|FG_INT);
					g_player->Gold+=100;
					GetMapInstance()->Loc().SetChar(41,9,L' ');
					mayor_q=false;
				}
				break;
			}
			case L'0':return;
		}
		g_player->RenderStat();
	}
}

void OldMan_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int k=L'A';
	while(k!=L'0')
	{
		int str=1;
		Print(L"������",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: ����������",FG_GREEN,1,str++);
		if(oldman_q)Print(L"2: �����",FG_GREEN,1,str++);else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: �����",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':
			{
				PrintLog(L"������ �������:",FG_WHITE|FG_INT);
				if(oldman_q)
				{
					PrintLog(L"��� ������ ����� ���������, �� �� ������� ������",FG_WHITE);
				}
				else
				{
					PrintLog(L"��� ������ �����... ������� �����",FG_WHITE);
				}
				break;
			}
			case L'2':
			{
				if(!oldman_q)break;
				if(drugs<2)
				{
					PrintLog(L"������ ���� �������:",FG_WHITE|FG_INT);
					PrintLog(L"����� � ������ � ������ � ���� ���������",FG_WHITE);
					PrintLog(L"��� ��������� 2 �������, 1 ����� � 1 ������� ��������",FG_WHITE);
					PrintLog(L"� �������� ������� ������... ��� ����� ��� ����������",FG_WHITE);
					drugs=1;
				}
				if(drugs==2)
				{
					PrintLog(L"����� ��������!",FG_YELLOW|FG_INT);
					PrintLog(L"���, ��� � ��� ���������! �������, �����.",FG_WHITE);
					PrintLog(L"� �������� ������ ������������� � ������ ���� ������ �����",FG_WHITE);
					PrintLog(L"������ ������ ��� �����. ������ �� ������ ������ �����",FG_YELLOW|FG_INT);
					g_player->bPick=true;
					oldman_q=false;
				}
				break;
			}
			case L'0':return;
		}
		g_player->RenderStat();
	}
}

void Guard_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int k=L'A';
	while(k!=L'0')
	{
		int str=1;
		Print(L"����������",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: �������� � �������",FG_GREEN,1,str++);
		Print(L"2: �������� � ��������",FG_GREEN,1,str++);
		Print(L"3: �������� � ������",FG_GREEN,1,str++);
		if(guard_q)Print(L"4: �����",FG_GREEN,1,str++);else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: �����",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':
			{
				PrintLog(L"���������� �������:",FG_WHITE|FG_INT);
				PrintLog(L"����� ����� ����.. ��... ������� ������ ����� �������� �������",FG_WHITE);
				PrintLog(L"� ������� ����, ����� ��������� ���������� �� ����������",FG_WHITE);
				break;
			}
			case L'2':
			{
				PrintLog(L"���������� �������:",FG_WHITE|FG_INT);
				PrintLog(L"�� �������� ���������� ��� �����, ��� �������� ����� �������",FG_WHITE);
				PrintLog(L"����� ��������, � ��� �������� � ���� ������, ������, �������� � ��������",FG_WHITE);
				PrintLog(L"�� �� �� ���� ����������, �� ��� ��������� ����������� =(",FG_WHITE);
				break;
			}
			case L'3':
			{
				PrintLog(L"���������� �������:",FG_WHITE|FG_INT);
				PrintLog(L"����� ����� ���������, ��� ��� ����� ������� ��������",FG_WHITE);
				break;
			}
			case L'4':
			{
				if(!guard_q)break;
				if(!g_player->bRelic)
				{
					PrintLog(L"���������� ���� �������:",FG_WHITE|FG_INT);
					PrintLog(L"��� �� �����, � ����������� �����, � ������� ��� ������ ��������.",FG_WHITE);
					PrintLog(L"��� ����� ��������, � ���� �� ��������, ������ ����� ��������� �������",FG_WHITE);
				}else{
					PrintLog(L"����� ��������!",FG_YELLOW|FG_INT);
					PrintLog(L"������������, ������ �� ������ ������. � ������ ���, �� ������ ������",FG_WHITE);
					PrintLog(L"���������� ����� ��� 2 ����� �������",FG_YELLOW|FG_INT);
					GetMapInstance()->Loc().SetChar(16,8,L' ');
					g_player->Potion+=2;
					guard_q=false;
				}
				break;
			}
			case L'0':return;
		}
		g_player->RenderStat();
	}
}

void Lose_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int str=1;
	Print(L"GAME OVER",FG_RED|FG_INT,1,str++);
}

void Win_Dialog()
{
	system("cls");
	PrintBorder();
	g_player->RenderStat();

	int str=1;
	Print(L"������!",FG_GREEN|FG_INT,1,str++);
	Print(L"������ ��������.",FG_GREEN|FG_INT,1,str++);
}