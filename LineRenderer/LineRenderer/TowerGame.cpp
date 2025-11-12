#include "TowerGame.h"
#include "LineRenderer.h"
#include "TextStream.h"
#include "imgui.h"

#include <time.h>
#include <iostream>

TowerSim::TowerSim()
{
	srand((unsigned int)time(NULL)); // Random seed

	int randomTitle = rand() % GameTitles.size();
	appInfo.appName = GameTitles[randomTitle];
	appInfo.grid.show = true;
	appInfo.grid.mainAxesColour = Colour::GREEN.Lighten().Multiply(0.2f);
	appInfo.grid.linesColour = Colour::GREEN.Lighten().Multiply(0.2f);
	appInfo.grid.extent = 5;
	appInfo.grid.unit = 5;
	appInfo.grid.showBasisLines = false;
	appInfo.camera.disable = true;
}

void TowerSim::Initialise()
{
	ScaleCameraHeight(5.1f);

	player = new PlayerTower(playerHealth, playerDamage);
}

void TowerSim::Update(float delta)
{
	// Track time for certain events
	gameTime += delta;
	spawnTimer += delta;

	// Update player and then enemies
	player->Update(delta, cursorPos);
	for (EnemyBase* enemy : ActiveEnemies)
	{
		enemy->Update(delta);
		if (enemy->IsDead())
		{
			//RemoveEnemy(enemy); // throws error
			continue;
		}
	}

	if (spawnTimer >= spawnDelay)
	{
		EnemyBase* newEnemy = new EnemyBase(gameTime);
		SpawnEnemy(newEnemy);
		spawnTimer = 0;
	}
	Draw();
}

void TowerSim::Draw()
{
	for (EnemyBase* enemy : ActiveEnemies)
	{
		enemy->Draw(lines);
	}
	player->Draw(lines, cursorPos);

	if (leftMouseDown)
	{
		lines->AddPointToLine({ cursorPos.x - 0.5f, cursorPos.y + 0.5f });
		lines->AddPointToLine({ cursorPos.x + 0.5f, cursorPos.y + 0.5f });
		lines->AddPointToLine({ cursorPos.x + 0.5f, cursorPos.y - 0.5f });
		lines->AddPointToLine({ cursorPos.x - 0.5f, cursorPos.y - 0.5f });
		lines->FinishLineLoop();
	}
}

void TowerSim::SpawnEnemy(EnemyBase* actor)
{
	EnemyBase* newEnemy;
	newEnemy = new EnemyBase(timeScale);
	ActiveEnemies.push_back(newEnemy);
}

void TowerSim::RemoveEnemy(EnemyBase* actor)
{
	int enemyCounter = 0;
	for (EnemyBase* Enemy : ActiveEnemies)
	{
		enemyCounter++;
		if (Enemy == actor)
		{
			ActiveEnemies.erase(ActiveEnemies.cbegin() + enemyCounter);
			delete Enemy;
		}
	}
}

void TowerSim::OnLeftClick()
{
	for (EnemyBase* enemy : ActiveEnemies)
	{
		
		if (enemy->GetPosition().x > cursorPos.x - 0.5f &&
			enemy->GetPosition().x < cursorPos.x + 0.5f &&
			enemy->GetPosition().y > cursorPos.y - 0.5f &&
			enemy->GetPosition().y < cursorPos.y + 0.5f)
		{
			enemy->TakeDamage(1);
			std::cout << "Hit" << std::endl;
		}
	}
}
