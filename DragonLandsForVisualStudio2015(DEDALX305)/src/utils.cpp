#include "utils.h"

HANDLE hStdout, hStdin;

//logs
const int def_border_x=map_width;
const int def_log_y=18;
int log_y=def_log_y;

int Rand(int _min,int _max)
{
	if(_min>=_max)return _min;
	return (rand()%(_max+1-_min))+_min;
}

bool Random(int probability)
{
	if (probability>(rand()%100)) return true;
	else return false;
}

void Swap(int &a,int &b)
{
	int c=a;
	a=b;
	b=c;
}

bool TestCoord(int x,int y)
{
	if (x<0||x>=map_width-1||y<0||y>=map_height) return false;
	else return true;	
}

int CalcDamage(int WeaponDmg) // Вычисление урона
{
	int damage=Rand(WeaponDmg/2,WeaponDmg);
	if(damage<1)damage=1;
	return damage;
}

void InitStdIO() // Инициализация ввода-вывода
{
	hStdin = GetStdHandle(STD_INPUT_HANDLE); 
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTitleW(L"Dragon Lands"); // Название окошка с игрой
    SetConsoleTextAttribute(hStdout,FOREGROUND_RED);
}

void Print(const wchar_t *text,WORD color,int x,int y,...)
{
	DWORD Written;
	COORD coord={x,y};
	SetConsoleTextAttribute(hStdout,color);
	if(x>=0&&y>=0)SetConsoleCursorPosition(hStdout,coord);

	static wchar_t str[max_str];
	va_list args;
	va_start(args,y);
	vswprintf(str,max_str,text,args);
	WriteConsoleW(hStdout,str,(DWORD)wcslen(str),&Written,0);
}

void PrintChar(wchar_t chr,WORD color,int x,int y)
{
	DWORD Written;
	COORD coord={x,y};
	SetConsoleTextAttribute(hStdout,color);
	if(x>=0&&y>=0)SetConsoleCursorPosition(hStdout,coord);
	WriteConsoleW(hStdout,&chr,1,&Written,0);
}

void PrintLog(const wchar_t *text,WORD color,...)
{
	static wchar_t str[max_str];
	va_list args;
	va_start(args,color);
	vswprintf(str,max_str,text,args);

	if(log_y>def_log_y+4)return;
	Print(str,color,1,log_y);
	log_y++;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(hStdout,&csbiInfo);
	SMALL_RECT rect;
	rect.Top=-csbiInfo.srWindow.Top;
	rect.Bottom=-csbiInfo.srWindow.Top;
	rect.Left=0;
	rect.Right=0;
	SetConsoleWindowInfo(hStdout,false,&rect);
}

void PrintBorder()
{
	const int width=74;
	const int height=24;
	WORD color=FG_WHITE;
	wchar_t str[width];
	wmemset(str,0x2550,width-1);
	str[width-1]=0;
	str[def_border_x-1]=0x2564;
	Print(L"\u2554%s\u2557",color,0,0,str);
	for(int h=1;h<height-1;h++)
	{
		PrintChar(0x2551,color,0,h);
		if(h<def_log_y)PrintChar(0x2502,color,def_border_x,h);
		PrintChar(0x2551,color,width,h);
	}
	str[def_border_x-1]=0x2550;
	Print(L"\u255A%s\u255D",color,0,height-1,str);

	wmemset(str,0x2500,width-1);
	str[def_border_x-1]=0x2534;
	Print(L"\u255F%s\u2562",color,0,def_log_y-1,str);
}

void PrintStat(const wchar_t *text,int _y,WORD color,...)
{
	const int len=19;
	static wchar_t str[len];
	wmemset(str,L' ',len);
	str[len-1]=0;
	Print(str,FG_WHITE,def_border_x+1,_y);
	if(!text)return;
	va_list args;
	va_start(args,color);
	vswprintf(str,len,text,args);
	Print(str,color,def_border_x+1,_y);
}

void ClearLog()
{
	const int len=def_border_x+18;
	static wchar_t clr[len];
	wmemset(clr,L' ',len-1);
	clr[len-1]=0;
	for(int i=def_log_y;i<log_y;i++)
		Print(clr,FG_WHITE,1,i);
	log_y=def_log_y;
}

const wchar_t* ReturnCase(wchar_t Case,wchar_t* nominative,wchar_t* genitive,wchar_t* dative)
{
	switch(Case)
	{
		case L'N':return nominative;
		case L'G':return genitive;
		case L'D':return dative;
		default: return L"неизвестное";
	}
}

const wchar_t* MonsterName(wchar_t type,wchar_t Case)
{
	switch(type)
	{
		case L'И':return ReturnCase(Case,L"Игрок",L"Игрока",L"Игроку");
		case L'П':return ReturnCase(Case,L"Паук",L"Паука",L"Пауку");
		case L'К':return ReturnCase(Case,L"Крыса",L"Крысу",L"Крысе");
		case L'Г':return ReturnCase(Case,L"Гоблин",L"Гоблина",L"Гоблину");
		case L'Н':return ReturnCase(Case,L"Наемник",L"Наемника",L"Наемнику");
		case L'Т':return ReturnCase(Case,L"Тролль",L"Тролля",L"Троллю");
		case L'З':return ReturnCase(Case,L"Зомби",L"Зомби",L"Зомби");
		case L'Д':return ReturnCase(Case,L"Дракон",L"Дракона",L"Дракону");
		default: return L" неизвестно ";
	}
}

const wchar_t* WeaponName(int WeaponType)
{
	switch(WeaponType)
	{
		case 1:return L"Нож";
		case 2:return L"Меч";
		case 3:return L"Цеп";
		default: return L"[error]";
	}
}

void PrintPlayerDealDamage(wchar_t MonsterType,int WeaponType,int damage)
{
	int rnd=rand()%10;
	switch(WeaponType)
	{
		case 1: //нож
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Игрок пырнул %s ножом, нанеся %d ед. урона",FG_GREEN,MonsterName(MonsterType,L'G'),damage);break;
				case 2:PrintLog(L"Игрок нанес ножом серьезную рану %s, %d ед. урона",FG_GREEN,MonsterName(MonsterType,L'D'),damage);break;
				case 3:PrintLog(L"Игрок резанул ножом %s и нанес %d урона",FG_GREEN,MonsterName(MonsterType,L'G'),damage);break;
				default:PrintLog(L"Игрок всадил нож в бок %s, %d повреждений",FG_GREEN,MonsterName(MonsterType,L'D'),damage);break;
			}
			break;
		}
		case 2: //меч
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Игрок ткнул острием меча %s, %d ед. вреда",FG_GREEN,MonsterName(MonsterType,L'G'),damage);break;
				case 2:PrintLog(L"Игрок с размаху ударил мечом %s, %d ед. урона",FG_GREEN,MonsterName(MonsterType,L'G'),damage);break;
				case 3:PrintLog(L"Игрок всадил %s меч в бок и нанес %d урона",FG_GREEN,MonsterName(MonsterType,L'D'),damage);break;
				default:PrintLog(L"%s пришлось отведать острого меча игрока, %d вреда",FG_GREEN,MonsterName(MonsterType,L'D'),damage);break;
			}
			break;
		}
		case 3: // цеп
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Игрок рубанул цепом %s и нанес %d ед. урона",FG_GREEN,MonsterName(MonsterType,L'G'),damage);break;
				case 2:PrintLog(L"Цеп со свистом полетел в %s, %d ед. повреждений",FG_GREEN,MonsterName(MonsterType,L'G'),damage);break;
				case 3:PrintLog(L"%s познакомилась с цепом игрока, %d урона",FG_GREEN,MonsterName(MonsterType,L'N'),damage);break;
				default:PrintLog(L"Игрок ударил тяжелым цепом %s, %d вреда",FG_GREEN,MonsterName(MonsterType,L'G'),damage);break;
			}
			break;
		}
		default:PrintLog(L"[error]",FG_RED|FG_INT);
	}
}

void PrintPlayerKillMonster(wchar_t MonsterType,int WeaponType,int exp)
{
	int rnd=rand()%3;
	wchar_t s_die[32];
	swprintf(s_die,32,L"%s умирает (+%d опыт)",MonsterName(MonsterType),exp);
	switch(WeaponType)
	{
		case 1: //кинжал
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Игрок засадил нож %s в глаз. %s",FG_GREEN,MonsterName(MonsterType,L'D'),s_die);break;
				case 2:PrintLog(L"Игрок метнул нож в %s и попал. %s",FG_GREEN,MonsterName(MonsterType,L'G'),s_die);break;
				default:PrintLog(L"Игрок нашинковал %s ножом на дольки. %s",FG_GREEN,MonsterName(MonsterType,L'G'),s_die);break;
			}
			break;
		}
		case 2: //меч
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Игрок мечом разрубил %s пополам. %s",FG_GREEN,MonsterName(MonsterType,L'G'),s_die);break;
				case 2:PrintLog(L"Игрок мечом отсек голову %s. %s",FG_GREEN,MonsterName(MonsterType,L'D'),s_die);break;
				default:PrintLog(L"%s напоролась на острый меч игрока. %s",FG_GREEN,MonsterName(MonsterType,L'N'),s_die);break;
			}
			break;
		}
		case 3: //топор
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Игрок размазал %s по земле цепом. %s",FG_GREEN,MonsterName(MonsterType,L'G'),s_die);break;
				case 2:PrintLog(L"Игрок ударил %s своим цепом. %s",FG_GREEN,MonsterName(MonsterType,L'D'),s_die);break;
				default:PrintLog(L"Игрок разможил череп %s тяжелым цепом. %s",FG_GREEN,MonsterName(MonsterType,L'D'),s_die);break;
			}
			break;
		}
		default:PrintLog(L"[error]",FG_RED|FG_INT);
	}
}

void PrintMonsterDealDamage(wchar_t MonsterType,int damage)
{
	int rnd=rand()%5; // Генерируем случайное число от 1 до 5

	switch(MonsterType)
	{
		case L'П': // Паук
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Паук плюет на героя ядовитой слюной, %d ед. повреждений",FG_RED|FG_INT,damage);break;
				case 2:PrintLog(L"Паук кусает героя за ногу, %d ед. урона",FG_RED|FG_INT,damage);break;
				case 3:PrintLog(L"Паук пытается обмотать героя паутиной! %d ед. урона", FG_RED|FG_INT, damage); break;
				default:PrintLog(L"Паук набрасывается на героя и наносит %d ед. урона",FG_RED|FG_INT,damage);break;
			}
			break;
		}
		case L'К': // Крыса
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Крыса яростно кусает игрока. %d ед. повреждений",FG_RED|FG_INT,damage);break;
				case 2:PrintLog(L"Крыса прокусывает игроку ногу. %d ед. урона",FG_RED|FG_INT,damage);break;
				case 3:PrintLog(L"Крыса издает противный писк. %d ед. урона",FG_RED|FG_INT,damage);break;
				default:PrintLog(L"Крыса атакует игрока острыми зубами. %d ед. урона",FG_RED|FG_INT,damage);break;
			}
			break;
		}
		case L'Г': // Гоблин
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Гоблин тыкает в игрока кинжалом %d ед. повреждений",FG_RED|FG_INT,damage);break;
				case 2:PrintLog(L"Гоблин кидает в игрока камень, %d ед. урона",FG_RED|FG_INT,damage);break;
				case 3:PrintLog(L"Гоблин показывает вам фотографию своей тещи... %d ед. урона",FG_RED|FG_INT,damage);break;
				default:PrintLog(L"Гоблин корчит игроку страшную рожу, %d ед. урона",FG_RED|FG_INT,damage);break;
			}
			break;
		}
		case L'Н': // Наемник
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Наемник обидно пинает игрока, %d ед. повреждений",FG_RED|FG_INT,damage);break;
				case 2:PrintLog(L"Наемник наносит ответный удар! %d ед. урона",FG_RED|FG_INT,damage);break;
				case 3:PrintLog(L"Наемник хочет позаимствовать оружие героя, %d ед. урона",FG_RED|FG_INT,damage);break;
				case 4:PrintLog(L"Наемник атакует героя %d ед. урона",FG_RED|FG_INT,damage);break;
				case 5:PrintLog(L"Наемник притворился мертвым, а потом внезапно атаковал героя! %d ед. ур",FG_RED|FG_INT,damage);break;
				default:PrintLog(L"Наемник атакует героя ножом. %d ед. вреда",FG_RED|FG_INT,damage);break;
			}
			break;
		}
		case L'Т': // Тролль
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Тролль ударил игрока толстой дубинкой. %d ед. повреждений",FG_RED|FG_INT,damage);break;
				case 2:PrintLog(L"Тролль пнул игрока жирной ногой в портянках. %d ед. урона",FG_RED|FG_INT,damage);break;
				case 3:PrintLog(L"Тролль назвал героя придурком. %d ед. урона",FG_RED|FG_INT,damage); break;
				case 4:PrintLog(L"Тролль попытался стянуть у героя золото!. %d ед. урона",FG_RED|FG_INT,damage); break;
				case 5:PrintLog(L"Тролль назвал героя придурком. %d ед. урона",FG_RED|FG_INT,damage); break;
				default:PrintLog(L"Тролль вмазал игроку обглоданной костью по голове. %d ед. вреда",FG_RED|FG_INT,damage);break;
			}
			break;
		}
		case L'З': // Зомби
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Зомби атакует героя обоими руками. %d ед. повреждений",FG_RED|FG_INT,damage);break;
				case 2:PrintLog(L"Зомби бьет игрока головой. %d ед. урона",FG_RED|FG_INT,damage);break;
				case 3:PrintLog(L"Зомби бормочет что-то странное.", FG_RED | FG_INT,damage); break;
				case 4:PrintLog(L"Зомби пытается заговорить вам зубы.", FG_RED | FG_INT,damage); break;
				case 5:PrintLog(L"Зомби прикидывается торговцем.", FG_RED | FG_INT,damage); break;
				default:PrintLog(L"Зомби с ужасным стоном бьет игрока рукой. %d ед. вреда",FG_RED|FG_INT,damage);break;
			}
			break;
		}
		case L'Д': // Дракон
		{
			switch(rnd)
			{
				case 1:PrintLog(L"Дракон выдыхает на героя пламя, %d ед. урона",FG_RED|FG_INT,damage);break;
				case 2:PrintLog(L"Дракон бьет игрока своим могучим хвостом с шипами, %d ед. урона",FG_RED|FG_INT,damage);break;
				case 3:PrintLog(L"Дракон пытается утащить героя в свое логово!", FG_RED | FG_INT, damage); break;
				default:PrintLog(L"Дракон прыгает и придавливает героя своей жирной тушей, %d ед. урона",FG_RED|FG_INT,damage);break;
			}
			break;
		}
		default:PrintLog(L"странное",FG_RED|FG_INT);
	}
}