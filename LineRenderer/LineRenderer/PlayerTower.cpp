#include "PlayerTower.h"

#include "LineRenderer.h"

PlayerTower::PlayerTower(float health, float damage)
{
	playerHealth = health;
	playerDamage = damage;

	position = { 0,0 };
}

void PlayerTower::Update(float delta, Vec2 cursorPos)
{
	for (TowerWeapon* weapon : Weapons)
	{
		weapon->reloadWait -= delta;
	}
}

void PlayerTower::Draw(LineRenderer* lines, Vec2 cursorPos)
{
	lines->DrawCircle({ 0,0 }, 2);
	lines->DrawCross(cursorPos, 0.25f);

	for (TowerWeapon* weapon : Weapons)
	{
		weapon->Draw(lines);
	}
}

void PlayerTower::AddWeapon(TowerWeapon* newWeapon)
{
	Weapons.push_back(newWeapon);
}

void PlayerTower::TakeDamage(float damage)
{
	playerHealth -= damage;
	if (playerHealth < 0)
	{
		playerHealth = 0;
		// kill player, end game
	}
}

void PlayerTower::FireWeapons()
{
	for (TowerWeapon* weapon : Weapons)
	{
		weapon->FireWeapon(playerDamage);
	}
}
