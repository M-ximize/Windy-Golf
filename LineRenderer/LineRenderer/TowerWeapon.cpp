#include "TowerWeapon.h"

TowerWeapon::TowerWeapon()
{
}

void TowerWeapon::FireWeapon(float weaponDamage)
{
	if (reloadWait <= 0.0f)
	{
		int damage = weaponDamage * damageMod;
		reloadWait = reloadTime;
	}
	else
	{

	}
}

void TowerWeapon::Draw(LineRenderer* lines)
{
}
