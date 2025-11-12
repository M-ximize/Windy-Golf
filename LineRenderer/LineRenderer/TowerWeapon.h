#pragma once

class LineRenderer;

class TowerWeapon
{
public:
	TowerWeapon();
	~TowerWeapon() {}

	virtual void FireWeapon(float weaponDamage);
	void Draw(LineRenderer* lines);

	float reloadWait;

private:
	float reloadTime;
	float damageMod;

	float bulletSpeed;
};