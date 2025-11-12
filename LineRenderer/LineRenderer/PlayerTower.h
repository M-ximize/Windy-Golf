#pragma once

#include "TowerWeapon.h"

#include "Maths.h"
#include <vector>

class LineRenderer;

class PlayerTower
{
public:
	PlayerTower(float health, float damage);
	~PlayerTower() {}

	void Update(float delta, Vec2 cursorPos);
	void Draw(LineRenderer* lines, Vec2 cursorPos);

	void AddWeapon(TowerWeapon* newWeapon);
	void TakeDamage(float damage);

	void FireWeapons();

private:
	float playerHealth;
	float playerDamage;

	Vec2 position;
	float rotation;
	
	std::vector<TowerWeapon*> Weapons;
};