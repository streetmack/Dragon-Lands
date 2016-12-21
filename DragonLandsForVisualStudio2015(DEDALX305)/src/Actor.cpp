#include "maps.h"

// класс Actor
Actor::Actor()
{
	map = GetMapInstance();
	type = L'E';
	x = 1;
	y = 1;
	level = 1; // Уровень
	exp = 0; // Опыт
	WeaponDmg = 10; // 10 по умолчанию
	Gold = 0; // Золото
	dead = true;
	HP = 1; // Здоровье
	MaxHP = 1; // Максимальное здоровье
}

bool Actor::TestCollision(int _x, int _y)
{
	if (x == _x&&y == _y)return true;
	else return false;
}

void Actor::Render()
{
	PrintChar(type, color, x + 1, y + 1);
}
void Actor::Die()
{
	dead = true;
	map->Loc().Blood(x, y);
}

//class Monster

void Monster::Update()
{
	if (dead)return;
	// прототип AI
	Actor& player = map->player;
	int mx = 0, my = 0;
	if (player.x != x)mx = abs(player.x - x) / (player.x - x);
	if (player.y != y)my = abs(player.y - y) / (player.y - y);

	if (map->Loc().TestCollision(x + mx, y + my))
	{
		if (mx == 0)Random() ? mx = -1 : mx = 1;
		if (my == 0)Random() ? my = -1 : my = 1;
		if (map->Loc().TestCollision(x + mx, y + my))Random() ? mx = 0 : my = 0;
	}

	if (map->Loc().CollisionWorld(x + mx, y + my))
	{
		Random() ? mx = 0 : my = 0;
		if (map->Loc().CollisionWorld(x + mx, y + my))Swap(mx, my);
	}

	if (abs(player.x - x) > 10 || abs(player.y - y) > 10) // Герой слишком далеко, топчемся на месте
	{
		if (Random()) Random() ? mx = 1 : mx = -1;
		else mx = 0;
		if (Random()) Random() ? my = 1 : my = -1;
		else my = 0;
	}
	Move(mx, my);
	Render();
}
void Monster::Move(int _x, int _y)
{
	if (!TestCoord(x + _x, y + _y))return;
	if (map->Loc().CollisionWorld(x + _x, y + _y))return;
	if (map->Loc().TestCollision(x + _x, y + _y))return;
	Player& player = map->player;

	if (player.TestCollision(x + _x, y + _y))
	{
		int damage = WeaponDmg;
		if (player.Armor > 0)damage -= (Rand(1, player.Armor));
		if (damage < 1)damage = 1;
		player.HP -= damage;
		if (player.HP < 1)
		{
			PrintLog(L"Игрок умирает", FG_RED | FG_INT);
			player.Die();
		}

		else
		{
			PrintMonsterDealDamage(type, damage);
		}
		player.RenderStat();
		return;
	}
	map->Loc().Repaint(x, y);
	x += _x;
	y += _y;
}

void Monster::Spawn(wchar_t _type, int _x, int _y)
{
	type = _type;
	color = FG_RED | FG_INT;
	x = _x;
	y = _y;

	switch (_type)
	{
	case L'П':Gold = Rand(0, 0);	MaxHP = Rand(1, 2);	  level = 2;	exp = 1;	WeaponDmg = 1;	break; // Паук
	case L'К':Gold = Rand(2, 5);	MaxHP = Rand(4, 8);	  level = 3;	exp = 2;	WeaponDmg = 2;	break; // Крыса	 
	case L'Г':Gold = Rand(1, 4);	MaxHP = Rand(6, 12);  level = 4;	exp = 3;	WeaponDmg = 3;	break; // Гоблин
	case L'Т':Gold = Rand(5, 10);   MaxHP = Rand(10, 15); level = 5;	exp = 5;	WeaponDmg = 5;	break; // Тролль
	case L'Н':Gold = Rand(25, 40);  MaxHP = Rand(20, 25); level = 7;	exp = 8;	WeaponDmg = 7;	break; // Наемник
	case L'З':Gold = Rand(5, 15);   MaxHP = Rand(5, 12);  level = 5;	exp = 4;	WeaponDmg = 4;	break; // Зомби
	case L'Д':Gold = 1000;          MaxHP = 300;          level = 10;	exp = 10;	WeaponDmg = 8;	break; // Дракон 	 
	}
	if (Random())Gold = 0;

	HP = MaxHP;
	dead = false;
	Render();
}

//class Player
Player::Player()
{
	dead = false;
	type = L'И';
	x = 5;
	y = 5;
	MaxHP = 1000;
	HP = MaxHP;
	WeaponDmg = 1;
	WeaponType = 1;
	Armor = 1;
	time_idle = 0;
	Wood = 0;
	exp = 0;
	Strength = 1;
	Potion = 3;
	Gold = 10;
	count_trolls = 0;
	count_rats = 0;
	NPC_color = blacksmith_color;
	bAxe = false; // Топор. true для отладки
	bDialogMode = false;
	bPick = false; // Кирка. true для отладки
	bRelic = false; // Реликвия. true для отладки
	bVictory = false;
}

void Player::RenderStat()
{
	time_idle = 3;
	int str = 1;
	PrintStat(L"Герой, уровень %d", str++, FG_WHITE | FG_INT, level);
	PrintStat(L"               ", str++, FG_WHITE | FG_INT);
	if (HP >= 10)
		PrintStat(L"Здоровье %d/%d", str++, FG_WHITE | FG_INT, HP, MaxHP); // Нормальное здоровье - белое
	else PrintStat(L"Здоровье %d/%d", str++, FG_RED | FG_INT, HP, MaxHP); // Плохое здоровье выводится красным

	PrintStat(L"Урон     %d-%d", str++, FG_WHITE | FG_INT, MinDamage(), MaxDamage());
	PrintStat(L"Опыт     %d/%d", str++, FG_WHITE | FG_INT, exp, ExpForLevelUp());
	PrintStat(L"--------------", str++, FG_WHITE | FG_INT);
	PrintStat(L"%s      +%d", str++, FG_WHITE | FG_INT, WeaponName(WeaponType), WeaponDmg);
	PrintStat(L"Доспехи  +%d", str++, FG_WHITE | FG_INT, Armor);
	PrintStat(L"Эликсир(H) x%d", str++, FG_CYAN | FG_INT, Potion);
	PrintStat(L"--------------", str++, FG_WHITE | FG_INT);
	PrintStat(L"Золото   %d", str++, FG_WHITE | FG_INT, Gold);

	if (Wood > 0)PrintStat(L"Дерево   %d", str++, FG_WHITE | FG_INT, Wood);
	else PrintStat(0, str++, FG_WHITE);

}

void Player::Update()
{
	time_idle--;
	if (time_idle <= 0)
	{
		time_idle = 3;
		RenderStat();
		PrintStat(0, 14, FG_WHITE);
		PrintStat(0, 15, FG_WHITE);
	}
}

void Player::Render()
{
	PrintChar(0x263B, FG_GREEN | FG_INT, x + 1, y + 1);
}

bool Player::CollisionMonster(int _x, int _y) // Проверка на взаимодействие с монстрами
{
	Actor *monster = map->Loc().TestCollision(_x, _y);
	if (monster)
	{
		int damage = Rand(MinDamage(), MaxDamage());
		if (damage < 1)damage = 1;

		monster->HP -= damage;
		if (monster->HP < 1)
		{
			PrintPlayerKillMonster(monster->type, WeaponType, monster->exp);
			exp += monster->exp;
			if (exp >= ExpForLevelUp())
			{
				exp -= ExpForLevelUp();
				level++;
				Strength++;
				MaxHP += 3;
				HP = MaxHP;
				PrintLog(L"Вы заработали новый уровень!", FG_YELLOW | FG_INT);
				PrintLog(L"Теперь у вас есть +1 к силе, +3 к здоровью", FG_YELLOW | FG_INT);
			}
			if (monster->Gold > 0)
			{
				Gold += monster->Gold;

				int rnd = Rand(1, 10);
				wchar_t add_str[255];
				swprintf(add_str, 255, L"!");

				switch (rnd)
				{
				case 1:
				{
					if (monster->level >= WeaponDmg)
					{
						UpgradeWeapon();
						swprintf(add_str, 255, L"и %s +%d", WeaponName(WeaponType), WeaponDmg);
					}
					break;
				}
				case 2:
				{
					if (monster->level >= Armor)
					{
						Armor++;
						swprintf(add_str, 255, L"и доспехи +%d", Armor);
					}
					break;
				}
				}
				PrintLog(L"Обыскав труп, вы нашли %d золотых %s!", FG_YELLOW | FG_INT, monster->Gold, add_str);
			}
			if (monster->type == L'Т')count_trolls++;
			if (monster->type == L'К')count_rats++;
			if (monster->type == L'Д')bVictory = true;
			monster->Die();
			Render();
		}
		else
		{
			PrintPlayerDealDamage(monster->type, WeaponType, damage);
		}

		RenderStat();
		PrintStat(MonsterName(monster->type), 14, FG_RED | FG_INT);
		PrintStat(L"Здоровье: %d/%d", 15, FG_WHITE | FG_INT, monster->HP, monster->MaxHP);
		return true;
	}
	return false;
}

// Взаимодействие игрока с обьектами (деревья, камни, золото, монстры...)
bool Player::CollisionObject(int x, int y)
{
	if (!TestCoord(x, y))return true;
	wchar_t chr = map->Loc().GetChar(x, y);
	if (chr == 0x2665)
	{
		map->Loc().SetChar(x, y, L' ');
		PrintLog(L"Полное излечение и +2 к здоровью!", FG_YELLOW | FG_INT);
		MaxHP += 2;
		HP = MaxHP;
		RenderStat();
		return true;
	}
	if (chr == 0x25BC)
	{
		map->Loc().SetChar(x, y, L' ');
		PrintLog(L"Вы нашли реликвию!", FG_YELLOW | FG_INT);
		bRelic = true;
		return true;
	}
	if (chr == 0x2660 || chr == 0x2663) // деревья
	{
		if (bAxe) // Если имеет топор
		{
			map->Loc().SetChar(x, y, 0x2219);
			PrintLog(L"Вы срубили дерево и заготовили 1 ед. древесины!", FG_GREEN);
			Wood++; // Увеличиваем количество дерева
			RenderStat(); // Обновляем статы
		}
		else PrintLog(L"Вы не смогли срубить дерево... вам нужен топор", FG_WHITE);
		return true;
	}
	if (chr == 0x25A0) // камень
	{
		if (bPick)
		{
			map->Loc().SetChar(x, y, 0x2591);
			map->Loc().SetColor(x, y, FG_WHITE);
			PrintLog(L"Вы разломали камень!", FG_GREEN);

		}
		else PrintLog(L"Вы не смогли разломать камень... вам нужна кирка", FG_WHITE);
		return true;
	}
	if (chr == 0x2592) // Скала
	{
		return true;
	}
	if (chr == L'$') // Деньги
	{
		int g = Rand(2, 5) + (level - 1) * 4;
		Gold += g;
		PrintLog(L"Вы нашли на земле %d золотых!", FG_YELLOW | FG_INT, g);
		map->Loc().SetChar(x, y, L' ');
		RenderStat();
		return false;
	}
	if (chr == 0x2666) // Лут
	{
		int g = Rand(5, 10) + (level - 1) * 5;
		int rnd = Rand(1, 15);
		switch (rnd)
		{
		case 1:UpgradeWeapon(); PrintLog(L"Вы нашли оружие, %s +%d!", FG_YELLOW | FG_INT, WeaponName(WeaponType), WeaponDmg); break;
		case 2:Armor++; PrintLog(L"Вы нашли доспехи +%d!", FG_YELLOW | FG_INT, Armor); break;
		case 3:Strength++; PrintLog(L"Вы нашли эликсир силы, +1 к Силе!", FG_CYAN | FG_INT); break;
		case 4:MaxHP += 2; HP = MaxHP; PrintLog(L"Вы нашли эликсир полного восстановления, +2 к здоровью!", FG_YELLOW | FG_INT); break;
		default:PrintLog(L"Вы нашли сундук, в котором лежало %d золотых!", FG_YELLOW | FG_INT, g); Gold += g;
		}
		map->Loc().SetChar(x, y, L' ');
		RenderStat();
		return false;
	}
	if (chr == 0x263B) // NPC
	{
		bDialogMode = true;
		NPC_color = map->Loc().GetColor(x, y);
		return true;
	}

	return false;
}

void Player::Move(int _x, int _y) // Перемещение игрока
{
	if (!TestCoord(x + _x, y + _y))
	{
		if (!map->Move(_x, _y)) return;
		//игрок вышел за пределы карты
		if (_x == 1)x = 0;
		if (_x == -1)x = map_width - 2;
		if (_y == 1)y = 0;
		if (_y == -1)y = map_height - 1;
		map->Loc().RepaintMap();
		Render();
		RenderStat();
		return;
	}
	if (CollisionObject(x + _x, y + _y))return;
	if (CollisionMonster(x + _x, y + _y))return;
	if (map->Loc().CollisionWorld(x + _x, y + _y))return;
	map->Loc().Repaint(x, y);
	x += _x;
	y += _y;
	Render();
}

void Player::UpgradeWeapon() // Улучшение оружия
{
	WeaponDmg++;
	if (WeaponDmg > 4 && WeaponDmg < 9)WeaponType = 2;
	if (WeaponDmg >= 9)WeaponType = 3;
}

int Player::ExpForLevelUp() // На сколько увеличивается опыт при повышении уровня героя
{
	return level * 10;
}

int Player::MinDamage() // Минимальный урон, наносимый героем
{
	return WeaponDmg / 2 + 1;
}

int Player::MaxDamage() // Максимальный урон, наносимый героем
{
	return WeaponDmg + Strength;
}

void Player::Heal() // Первая медицинская помощь
{
	if (Potion < 1)return;
	if (HP == MaxHP)return;
	Potion--;
	HP += MaxHP / 2 + 1;
	if (HP > MaxHP)HP = MaxHP;
	RenderStat();
	PrintLog(L"Вы используете эликсир для восстановления здоровья (+50%s)!", FG_CYAN, L"%%");
}