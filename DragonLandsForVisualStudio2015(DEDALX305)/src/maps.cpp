#include "maps.h"
#include "maps_data.h"

Map g_map;

//class Location

wchar_t Location::GetChar(int x,int y)
{
	if(!TestCoord(x,y))return L'\0';
	return wchars[x][y];
}

void Location::SetChar(int x,int y,wchar_t chr,bool repaint)
{
	if(!TestCoord(x,y))return;
	wchars[x][y]=chr;
	if(repaint)Repaint(x,y);
}

WORD Location::GetColor(int x,int y)
{
	if(!TestCoord(x,y))return 0;
	return colors[x][y];
}

void Location::SetColor(int x,int y,WORD color,bool repaint)
{
	if(!TestCoord(x,y))return;
	colors[x][y]=color;
	if(repaint)Repaint(x,y);
}

void Location::Repaint(int x,int y) // Перерисовывает символ
{
	if(!TestCoord(x,y))return;
	PrintChar(GetChar(x,y),GetColor(x,y),x+1,y+1);
}

void Location::UpdateMonsters()
{
	for(int i=0;i<max_monsters;i++)monster[i].Update();
}

void Location::RepaintMap()
{
	//system("cls"); // Очистка экрана
	
	// Загрузка карты в цикле
	for(int y=0;y<map_height;y++)
		for(int x=0;x<map_width;x++)
		{
			wchar_t chr=GetChar(x,y);
			WORD color=GetColor(x,y);
			if(chr==0x2591&&color==(FG_RED|FG_INT))chr=L' ';
			if(chr==0x2020)
			{
				if(Random(30))SpawnMonster(L'З',x,y);
			}
			SetChar(x,y,chr,false);
			SetColor(x,y,color,true);
		}
	//PrintBorder(); // Рамка, которая разделяет окно на части
}

void Location::Replace3x3(int left,int top,wchar_t _old,wchar_t _new)
{
	for(int y=top;y<top+3;y++)
		for(int x=left;x<left+3;x++)
			if(GetChar(x,y)==_old)SetChar(x,y,_new,false);
}

bool Location::CollisionWorld(int x,int y) // Коллизии
{
	if(!TestCoord(x,y))return true;
	wchar_t chr=GetChar(x,y);
	if(chr==0x2591)return false;
	if(chr==0x25BC)return false;
	if(chr==L'$')return true;
	if(chr==0x2020)return true;
	if(chr>=0x2500&&chr<=0x266B)return true;

	return false;
}

void Location::Blood(int _x,int _y) // Кровь монстров
{
	for(int y=_y-1;y<=_y+1;y++)
		for(int x=_x-1;x<=_x+1;x++)
		{
			if(GetChar(x,y)==L' ')if(Random())
			{
				SetColor(x,y,FG_RED|FG_INT);
				SetChar(x,y,0x2591,true);
			}
		}

	SetColor(_x,_y,FG_RED|FG_INT);
	SetChar(_x,_y,0x2591,true);
}

Actor* Location::TestCollision(int x,int y)
{
	for(int i=0;i<max_monsters;i++)
	{
		if(monster[i].dead)continue;
		if((monster[i].x==x)&&(monster[i].y==y))return &monster[i];

	}
	return 0;
}

void Location::SpawnMonster(wchar_t type,int x,int y) // Спавн монстров
{
	for(int i=0;i<max_monsters;i++)
	{
		if(!monster[i].dead)continue;
		monster[i].Spawn(type,x,y);
		return;
	}
}

//class Map

bool Map::Move(int x,int y)
{
	if((map_x+x)==map_count_x||(map_x+x)<0)return false;
	if((map_y+y)==map_count_y||(map_y+y)<0)return false;
	map_x+=x;
	map_y+=y;
	return true;
}

bool Map::IsBrick(int x,int y)
{
	wchar_t chr=Loc().GetChar(x,y);
	if(chr>=0x2500&&chr<=0x256C)return true;
	if(chr==L'B')return true;
	return false;
}

void Map::LoadMap()
{
	// Очистка и загрузка карты
	for(int j=0;j<map_count_y;j++)
		for(int i=0;i<map_count_x;i++)
		{
			SetCoor(i,j);
			for(int y=0;y<map_height;y++)
				for(int x=0;x<map_width;x++)
				{
					Loc().SetChar(x,y,maps[j][i][y][x],false); //особая индексация массива
					Loc().SetColor(x,y,FG_WHITE|FG_INT,false);
					wchar_t chr=Loc().GetChar(x,y);
					switch(chr)
					{
						//монстры
						case L'к':Loc().SpawnMonster(L'К',x,y);break;
						case L'т':Loc().SpawnMonster(L'Т',x,y);break;
						case L'з':Loc().SpawnMonster(L'З',x,y);break;
						case L'д':Loc().SpawnMonster(L'Д',x,y);break;
						case L'п':Loc().SpawnMonster(L'П',x,y);break;
						case L'г':Loc().SpawnMonster(L'Г',x,y);break;
						case L'н':Loc().SpawnMonster(L'Н',x,y);break;
						default:continue;
					}
					Loc().SetChar(x,y,L' ',false);
				}
		}

	//создание объектов
	for(int j=0;j<map_count_y;j++)
		for(int i=0;i<map_count_x;i++)
		{
			SetCoor(i,j);
			for(int y=0;y<map_height;y++)
				for(int x=0;x<map_width;x++)
				{
					wchar_t chr=Loc().GetChar(x,y);
					switch(chr)
					{
						//пыль возле скал и камней
						case L'R':Loc().Replace3x3(x-1,y-1,L' ',L'D');break;
						case L'r':Loc().Replace3x3(x-1,y-1,L' ',L'D');break;
						case L'S':Loc().Replace3x3(x-1,y-1,L' ',L'D');break;
						//трава возле деревьев и мелкая трава
						case L'T':Loc().Replace3x3(x-1,y-1,L' ',L'g');break;
						case L'G':Loc().Replace3x3(x-1,y-1,L' ',L'g');break;
						//вода
						case L'W':Loc().Replace3x3(x-1,y-1,L' ',L'w');Loc().Replace3x3(x-1,y-1,L'D',L'w');Loc().Replace3x3(x-1,y-1,L'g',L'w');break;
					}
				}
		}

	// Применение псевдографики и раскрашивание.
	for(int j=0;j<map_count_y;j++)
		for(int i=0;i<map_count_x;i++)
		{
			SetCoor(i,j);
			for(int y=0;y<map_height;y++)
				for(int x=0;x<map_width;x++)
				{
					wchar_t chr=Loc().GetChar(x,y);
					WORD color=Loc().GetColor(x,y);
					color=FG_WHITE|FG_INT;
					switch(chr)
					{
						case L'R':chr=0x25B2;color=FG_WHITE|BG_WHITE|BG_INT;break; // Камень
						case L'r':chr=0x2592;color=FG_WHITE|BG_WHITE|BG_INT;break; // Камень-стена (Разрушаемый)
						case L'D':chr=0x2591;color=FG_WHITE;break; // Пыль
						case L'S':chr=0x25A0;color=FG_WHITE|FG_INT;break; // Камень
						case L'T':chr=0x2663;color=FG_GREEN|FG_INT|BG_GREEN;break; // Дерево
						case L'G':chr=L' ';color=FG_GREEN|FG_INT|BG_GREEN;break; // Трава
						case L'g':chr=0x2591;color=FG_GREEN;break;//Grass (Small)
						case L'W':chr=L'~';color=FG_BLUE|FG_INT|BG_BLUE;break; // Вода
						case L'w':chr=0x2593;color=FG_BLUE|FG_INT|BG_BLUE;break; // Вода (Small)
						case L'$':color=FG_YELLOW|FG_INT;break; // Золото
						case L'*':chr=0x2666;color=FG_YELLOW|FG_INT;break; // Лут
						case L'!':chr=0x2020;color=FG_WHITE;break; // Крест (респавн нежити)
						case L'@':chr=0x2665;color=FG_RED|FG_INT;break; // Восстановление здоровья

						case L'0':chr=0x25BC;color=FG_PURPLE|FG_INT;break; // Реликвия
						case L'1':chr=0x263B;color=blacksmith_color;break; // Кузнец
						case L'2':chr=0x263B;color=healer_color;break; // Лекарь
						case L'3':chr=0x263B;color=merchant_color;break; // Торговец
						case L'4':chr=0x263B;color=mayor_color;break; // Мэр города
						case L'5':chr=0x263B;color=merchant2_color;break; // Торговец в городе
						case L'6':chr=0x263B;color=oldman_color;break; // Cтарик
						case L'7':chr=0x263B;color=guard_color;break; // Привратник

						case L'B':
						{
							color=FG_WHITE|FG_INT;
							int brick=0;
							if(IsBrick(x,y-1))brick+=1;
							if(IsBrick(x+1,y))brick+=2;
							if(IsBrick(x,y+1))brick+=4;
							if(IsBrick(x-1,y))brick+=8;
							switch(brick)
							{
								case 0:chr=0x256C;break;
								case 1:chr=0x2551;break;
								case 2:chr=0x2550;break;
								case 3:chr=0x255A;break;
								case 4:chr=0x2551;break;
								case 5:chr=0x2551;break;
								case 6:chr=0x2554;break;
								case 7:chr=0x2560;break;
								case 8:chr=0x2550;break;
								case 9:chr=0x255D;break;
								case 10:chr=0x2550;break;
								case 11:chr=0x2569;break;
								case 12:chr=0x2557;break;
								case 13:chr=0x2563;break;
								case 14:chr=0x2566;break;
								case 15:chr=0x256C;break;
							}
							//chr=0x255A;color=FG_WHITE;
							break;
						}
					}
					Loc().SetChar(x,y,chr,false);
					Loc().SetColor(x,y,color,false);			
				}
		}

	SetCoor(0,0);

	Loc().RepaintMap();
}

void Map::Update()
{
	Loc().UpdateMonsters();
	player.Render();
	player.Update();
	PrintChar(L' ',FG_CYAN,map_width+1,map_height);
}

Map* GetMapInstance()
{
	return &g_map;
}
