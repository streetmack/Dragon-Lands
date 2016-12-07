#include <Windows.h>
#include <conio.h>
#include <wchar.h>
#include <ctime>
#include <math.h>

#include "maps.h" // ����� ����
#include "utils.h" 
#include "dialogs.h" // ������� 

Map *map=GetMapInstance();

int wmain()
{
	srand((unsigned int)time(0));
	InitStdIO();
	map->LoadMap();
	map->Update();
	Init_Dialog(&map->player);

	int k=0;
	while(k!=27)
	{
		if(map->player.dead)
		{
			//Lose_Dialog(); // ������ ������ �����
			break; 
		}
		if(map->player.bVictory)
		{
			//Win_Dialog(); // ���������� ������ � ������ ��� ��������
			break;
		}
		if(map->player.bDialogMode)
		{
			switch(map->player.NPC_color)
			{
				//case blacksmith_color:Blacksmith_Dialog();break; // ������
				//case healer_color:Healer_Dialog();break; // ������
				//case merchant_color:Merchant_Dialog();break; //
				//case merchant2_color:Merchant2_Dialog();break; // 
				//case mayor_color:Mayor_Dialog();break; // ���
				//case oldman_color:OldMan_Dialog();break; // ������
				//case guard_color:Guard_Dialog();break; // ����������
			}
			map->player.bDialogMode=false;
			map->Loc().RepaintMap(); // �������������� �����
 			map->player.RenderStat(); // ��������� �����
		}
		
		else
		{
			map->Update();
			if(!map->player.dead)k=_getch();
			else k=L'A';
			
			ClearLog();
			switch(k)
			{
				case 'h':map->player.Heal();break;
				case 224://����������� ���, ������� �� �������
					{
					k=_getch();
					switch(k)
					{ // case key_code:map->player.Move(X,Y); break;
						case 75:map->player.Move(-1,0); break; // �����
						case 77:map->player.Move(1,0); break; // ������
						case 72:map->player.Move(0,-1); break; // �����
						case 80:map->player.Move(0,1); break; // ����
					}
					break;
				}
			}
		}
	}
	ClearLog();
	PrintLog(L"������� ����� ������� ��� ������",FG_WHITE|FG_INT);
	getch();
};