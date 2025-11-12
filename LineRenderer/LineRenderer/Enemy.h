#pragma once

#include "Maths.h"

class LineRenderer;

class EnemyBase
{
public:
	EnemyBase(float timeScale);
	~EnemyBase() {}

	void Update(float deltaTime);
	void Draw(LineRenderer* lines);
	bool IsDead();

	void TakeDamage(float damage);
	void DealDamage();

	Vec2 GetPosition() { return position; }

private:
	float enemyHealth = 1;
	float enemyDamage = 1;

	float enemySize;
	Vec2 moveSpeed;

	Vec2 position;
};