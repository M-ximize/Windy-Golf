#include "Enemy.h"

#include "LineRenderer.h"

#include <random>

EnemyBase::EnemyBase(float timeScale)
{
	float startX = (rand() % 41) - 20;
	float startY = (rand() % 41) - 20;
	position = { startX,startY };
	enemySize = timeScale;
	moveSpeed = position * 0.9f;
}

void EnemyBase::Update(float deltaTime)
{
	if (!IsDead()) 
	{
		position -= moveSpeed * deltaTime;
		if (position.x < 2.5f && position.x > -2.5f &&
			position.y < 2.5f && position.y > -2.5f)
		{
			moveSpeed = { 0,0 };
		}
	}
}

void EnemyBase::Draw(LineRenderer* lines)
{
	lines->DrawCircle(position, enemySize);
}

bool EnemyBase::IsDead()
{
	if (enemyHealth <= 0)
	{
		return true;
	}
	return false;
}

void EnemyBase::TakeDamage(float damage)
{
	enemyHealth -= damage;
}
