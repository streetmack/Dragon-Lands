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
		PrintLog(L"Недостаточно золота",FG_RED|FG_INT);
		return false;
	}
	g_player->Gold-=cost;
	PrintLog(L"Вы заплатили %d золотых!",FG_WHITE,cost);
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
		Print(L"Кузнец",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: Улучшить оружие - %d золотых",FG_GREEN,1,str++,WeaponCost);
		Print(L"2: Улучшить доспехи - %d золотых",FG_GREEN,1,str++,ArmorCost);
		Print(L"3: Поговорить",FG_GREEN,1,str++);
		if(blacksmith_q)Print(L"4: Квест",FG_GREEN,1,str++);
		else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: Выход",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':if(Pay(WeaponCost))g_player->UpgradeWeapon();break;
			case L'2':if(Pay(ArmorCost))g_player->Armor++;break;
			case L'3':
			{
				PrintLog(L"Кузнец говорит:",FG_WHITE|FG_INT);
				if(blacksmith_q)
				{
					PrintLog(L"Да о чем тут говорить... Все очень хреново",FG_WHITE);
					PrintLog(L"Тролли жутко достали, сжирают все припасы, клиентов отпугивают. Сволочи!",FG_WHITE);
				}else PrintLog(L"Ни одного тролля рядом... Лепота!",FG_WHITE);
				break;
			}
			case L'4':
			{
				if(!blacksmith_q)break;
				if(g_player->count_trolls<10)
				{
					PrintLog(L"Кузнец дает задание:",FG_WHITE|FG_INT);
					PrintLog(L"Чертовы тролли сожрали все мои припасы на зиму.",FG_WHITE);
					PrintLog(L"Убей всех что бродят вокруг и возвращайся за наградой",FG_WHITE);
				}else{
					PrintLog(L"Квест кузница выполнен!",FG_YELLOW|FG_INT);
					PrintLog(L"Ты убил всех троллей! Не передать словами как я счастлив.",FG_WHITE);
					PrintLog(L"В качестве награды я бесплатно улучшу твое снаряжение",FG_WHITE);
					PrintLog(L"+1 к оружию, +1 к доспехам, +100 золотых!",FG_YELLOW|FG_INT);
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
		Print(L"Лекарь",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: Купить зелье - %d золотых",FG_GREEN,1,str++,PotionCost);
		Print(L"2: Излечение - %d золотых",FG_GREEN,1,str++,HealCost);
		Print(L"3: Поговорить",FG_GREEN,1,str++);
		if(drugs==1)Print(L"4: Лекарство",FG_GREEN,1,str++);
		else Print(L"                ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: Выход",FG_GREEN,1,str++);

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
				PrintLog(L"Лекарь говорит:",FG_WHITE|FG_INT);
				PrintLog(L"Всегда берите с собой достаточное количество зелий",FG_WHITE);
				PrintLog(L"На всякий случай, вдруг попадется особо опасный монстр...",FG_WHITE);
				break;
			}
			case L'4':
			{
				PrintLog(L"Вы взяли лекарство для старика.",FG_YELLOW|FG_INT);
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
		Print(L"Торговец",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: Улучшить оружие - %d золотых",FG_GREEN,1,str++,WeaponCost);
		Print(L"2: Купить зелье - %d золотых",FG_GREEN,1,str++,PotionCost);
		Print(L"3: Эссенция Силы - %d золотых",FG_GREEN,1,str++,StrengthCost);
		Print(L"4: Поговорить",FG_GREEN,1,str++);
		if(merchant_q)Print(L"5: Квест",FG_GREEN,1,str++);
		else Print(L"5: Продать всю древесину",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: Выход",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':if(Pay(WeaponCost))g_player->UpgradeWeapon();break;
			case L'2':if(Pay(PotionCost))g_player->Potion++;break;
			case L'3':if(Pay(StrengthCost))g_player->Strength++;break;
			case L'4':
			{
				PrintLog(L"Торговец говорит:",FG_WHITE|FG_INT);
				if(merchant_q)
				{
					PrintLog(L"Из-за гоблинов я не могу заготавливать древесину.",FG_WHITE);
					PrintLog(L"Она нужна мне для постройки частокола",FG_WHITE);
				}
				else PrintLog(L"Я скупаю дерево, так что ты сможешь подразаботать дровосеком",FG_WHITE);
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
					PrintLog(L"Продав все дерево, вы заработали %d золотых",FG_YELLOW|FG_INT,money);
					break;
				}
				if(g_player->Wood<20)
				{
					PrintLog(L"Торговец дает задание:",FG_WHITE|FG_INT);
					PrintLog(L"Гоблины сожгли мою баню. Я скупаю дерево, чтобы построить новую",FG_WHITE);
					PrintLog(L"Принеси мне 20 ед. древесины, и будешь щедро вознагражден",FG_WHITE);
					if(!g_player->bAxe)PrintLog(L"Продавец дает тебе топор. Теперь ты можешь срубать деревья",FG_YELLOW|FG_INT);
					g_player->bAxe=true;
				}else{
					PrintLog(L"Квест продавца выполнен!",FG_YELLOW|FG_INT);
					PrintLog(L"Превосходно, этого количества мне как раз хватит на строительство",FG_WHITE);
					PrintLog(L"Я выдам тебе бесплатную эссенцию Силы и буду скупать все дерево",FG_WHITE);
					PrintLog(L"Вы получаете +1 к Силе и 150 золотых!",FG_YELLOW|FG_INT);
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
		Print(L"Торговец",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: Улучшить оружие - %d золотых",FG_GREEN,1,str++,WeaponCost);
		Print(L"2: Эссенция Силы - %d золотых",FG_GREEN,1,str++,StrengthCost);
		Print(L"3: Поговорить",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: Выход",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':if(Pay(WeaponCost))g_player->UpgradeWeapon();break;
			case L'2':if(Pay(StrengthCost))g_player->Strength++;break;
			case L'3':
			{
				PrintLog(L"Торговец говорит:",FG_WHITE|FG_INT);
				PrintLog(L"Есть еще один торговец, который не прочь скупить древесину",FG_WHITE);
				PrintLog(L"Попробуй его поискать, и сможешь неплохо подзаработать",FG_WHITE);
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
		Print(L"Мэр города",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: Спросить о городе",FG_GREEN,1,str++);
		Print(L"2: Спросить о торговцах",FG_GREEN,1,str++);
		Print(L"3: Спросить о драконе",FG_GREEN,1,str++);
		if(mayor_q)Print(L"4: Квест",FG_GREEN,1,str++);else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: Выход",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':
			{
				PrintLog(L"Мэр говорит:",FG_WHITE|FG_INT);
				PrintLog(L"У нас маленький и спокойный городок.",FG_WHITE);
				if(mayor_q)
				{
					PrintLog(L"Но в последнее время много проблем с крысами",FG_WHITE);
					PrintLog(L"Воруют сыр, разносят микробов, гадят где попало...",FG_WHITE);
					PrintLog(L"Нам удалось временно их изолировать.",FG_WHITE);
				}
				else
				{
					PrintLog(L"С тех пор как не осталось ни одной крысы, дела налаживаются",FG_WHITE);
					PrintLog(L"Хотя кого я обманываю, все еще хуже чем прежде...",FG_WHITE);
					PrintLog(L"Теперь из нашего рациона исчез вкусная шаурма из крысятинки =(",FG_WHITE);
				}
				break;
			}
			case L'2':
			{
				PrintLog(L"Мэр говорит:",FG_WHITE|FG_INT);
				PrintLog(L"У торговца можно улучшить оружие, у лекаря - прикупить зелий",FG_WHITE);
				PrintLog(L"Был еще кузнец, но он свалил на Запад, в горы. И попал прямо к троллям",FG_WHITE);
				break;
			}
			case L'3':
			{
				PrintLog(L"Мэр говорит:",FG_WHITE|FG_INT);
				PrintLog(L"Драконы - очень опасные существа",FG_WHITE);
				PrintLog(L"Чтобы сразиться с ним, тебе требуется как минимум 10 уровень.",FG_WHITE);
				break;
			}
			case L'4':
			{
				if(!mayor_q)break;
				if(g_player->count_rats<12)
				{
					PrintLog(L"Мэр дает задание:",FG_WHITE|FG_INT);
					PrintLog(L"Крысы заполонили город, воруют припасы и разносят чуму",FG_WHITE);
					PrintLog(L"Убей их всех, и получишь щедрое вознаграждение",FG_WHITE);
					if(!open_rats)
					{
						Map *map=GetMapInstance();
						map->Loc().SetChar(20,6,L' ');
						map->Loc().SetChar(16,12,L' ');
						map->Loc().SetChar(40,12,L' ');
						open_rats=true;
					}
				}else{
					PrintLog(L"Квест мэра выполнен!",FG_YELLOW|FG_INT);
					PrintLog(L"Превосходно, ни одной крысы не осталось!",FG_WHITE);
					PrintLog(L"ты получаешь 100 золотых!",FG_YELLOW|FG_INT);
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
		Print(L"Старик",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: Поговорить",FG_GREEN,1,str++);
		if(oldman_q)Print(L"2: Квест",FG_GREEN,1,str++);else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: Выход",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':
			{
				PrintLog(L"Старик говорит:",FG_WHITE|FG_INT);
				if(oldman_q)
				{
					PrintLog(L"Мне срочно нужно лекарство, не то отброшу коньки",FG_WHITE);
				}
				else
				{
					PrintLog(L"Мне теперь лучше... гораздо лучше",FG_WHITE);
				}
				break;
			}
			case L'2':
			{
				if(!oldman_q)break;
				if(drugs<2)
				{
					PrintLog(L"Старик дает задание:",FG_WHITE|FG_INT);
					PrintLog(L"Сходи к лекарю и возьми у него лекарство",FG_WHITE);
					PrintLog(L"Мне требуются 2 красных, 1 синяя и 1 зеленая таблетка",FG_WHITE);
					PrintLog(L"И прихвати склянку спирта... мне нужно для растирания",FG_WHITE);
					drugs=1;
				}
				if(drugs==2)
				{
					PrintLog(L"Квест выполнен!",FG_YELLOW|FG_INT);
					PrintLog(L"Ааа, вот и мое лекарство! Спасибо, герой.",FG_WHITE);
					PrintLog(L"В качестве вечной благодарности я вручаю тебе старую кирку",FG_WHITE);
					PrintLog(L"Старик отдает вам кирку. Теперь вы можете ломать камни",FG_YELLOW|FG_INT);
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
		Print(L"Привратник",FG_WHITE|FG_INT,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"1: Спросить о драконе",FG_GREEN,1,str++);
		Print(L"2: Спросить о кладбище",FG_GREEN,1,str++);
		Print(L"3: Спросить о шахтах",FG_GREEN,1,str++);
		if(guard_q)Print(L"4: Квест",FG_GREEN,1,str++);else Print(L"         ",FG_GREEN,1,str++);
		Print(L"-----------------",FG_GREEN,1,str++);
		Print(L"0: Выход",FG_GREEN,1,str++);

		k=_getch();
		ClearLog();
		switch(k)
		{
			case L'1':
			{
				PrintLog(L"Привратник говорит:",FG_WHITE|FG_INT);
				PrintLog(L"Очень много прид.. ээ... славных героев хотят завалить дракона",FG_WHITE);
				PrintLog(L"Я охраняю вход, чтобы уменьшить статистику их смертности",FG_WHITE);
				break;
			}
			case L'2':
			{
				PrintLog(L"Привратник говорит:",FG_WHITE|FG_INT);
				PrintLog(L"На кладбище похоронены все герои, что пытались убить дракона",FG_WHITE);
				PrintLog(L"Земля проклята, и они восстали в виде нежити, бродят, пристают к туристам",FG_WHITE);
				PrintLog(L"Мы бы их всех уничтожили, но они постоянно респейнятся =(",FG_WHITE);
				break;
			}
			case L'3':
			{
				PrintLog(L"Привратник говорит:",FG_WHITE|FG_INT);
				PrintLog(L"Шахты давно заброшены, ибо там много опасных монстров",FG_WHITE);
				break;
			}
			case L'4':
			{
				if(!guard_q)break;
				if(!g_player->bRelic)
				{
					PrintLog(L"Привратник дает задание:",FG_WHITE|FG_INT);
					PrintLog(L"Иди на запад, к заброшенной шахте, и принеси мне оттуда реликвию.",FG_WHITE);
					PrintLog(L"Там много монстров, и если ты выживешь, значит готов встретить дракона",FG_WHITE);
				}else{
					PrintLog(L"Квест выполнен!",FG_YELLOW|FG_INT);
					PrintLog(L"Замечательно, теперь ты можешь пройти. И возьми это, на всякий случай",FG_WHITE);
					PrintLog(L"Привратник отдал вам 2 Зелья Лечения",FG_YELLOW|FG_INT);
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
	Print(L"Победа!",FG_GREEN|FG_INT,1,str++);
	Print(L"Дракон повержен.",FG_GREEN|FG_INT,1,str++);
}