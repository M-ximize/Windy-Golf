#pragma once

#include "TowerWeapon.h"

class MachineGun : public TowerWeapon
{
public:
	MachineGun();
	~MachineGun() {}

	virtual void FireWeapon(float weaponDamage) override;

private:
	
};