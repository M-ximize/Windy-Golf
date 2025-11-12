#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "Application.h"
#include "Maths.h"

#include "PlayerTower.h"
#include "Enemy.h"

class TowerSim : public Application
{
private:
	// Player variables
	float playerHealth = 10;
	float playerDamage = 1;

	PlayerTower* player = nullptr;

	// Game variables
	float gameTime = 0; // total time of game

	float spawnTimer = 0; // no need to change
	float spawnDelay = 2; // time between enemy spawn
	int timeScale = 1;

	std::vector<EnemyBase*> ActiveEnemies;

	std::vector<const char*> GameTitles = {
		"Defend the Tower"
	};

public:
	TowerSim();

	~TowerSim() {}

	void Initialise() override;

	void Update(float delta) override;

	void Draw();

	// Game Functions

	void SpawnEnemy(EnemyBase* actor);

	void RemoveEnemy(EnemyBase* actor);

	// Controls
	void OnLeftClick() override;

	/*void OnRightClick() override;

	void OnMiddleClick() override;

	void OnMouseScroll(bool positive) override;*/
};