#pragma once

#include <vector>

#include "Application.h"
#include "Maths.h"
#include "Grid.h"
#include "PhysActor.h"

class GolfPhysScene : public Application
{
private:
	// Player variables
	Colour CurrentColour;

	// Game variables
	bool InPlay = false;
	bool ShotReady = true;
	float Threshold = 0.5f;
	float WallBounce = 0.3f;
	int MaxWindForce = 20;
	int GoalCount = 0;
	int BallCount = 0;
	Vec2 GoalPos;
	Vec2 WindSpeed;
	float Gravity;
	float TimeStep;
	std::vector<PhysObject*> PhysActors;

	Grid GameMap;

public:
	GolfPhysScene();

	~GolfPhysScene();

	void Initialise() override;

	void addActor(PhysObject* actor);

	void removeActor(PhysObject* actor);

	void Update(float delta) override;

	void draw();

	void setGravity(float _gravity) { Gravity = _gravity; }
	float getGravity() const { return Gravity; }

	void setTimeStep(float _timeStep) { TimeStep = _timeStep; }
	float getTimeStep() const { return TimeStep; }

	static bool circleToCircle(PhysObject*, PhysObject*);
	static bool planeToPlane(PhysObject*, PhysObject*);
	static bool circleToPlane(PhysObject*, PhysObject*);
	static bool planeToCircle(PhysObject*, PhysObject*);
	static bool holeToHole(PhysObject*, PhysObject*);
	static bool holeToCircle(PhysObject*, PhysObject*);
	static bool circleToHole(PhysObject*, PhysObject*);
	static bool holeToPlane(PhysObject*, PhysObject*);
	static bool planeToHole(PhysObject*, PhysObject*);

	// Game Functions

	void CreateGoal();

	bool ScoreGoal(PhysObject* Hole, PhysObject* Ball);

	// Controls

	void OnLeftRelease() override;

	void OnRightRelease() override;

	void OnMiddleClick() override;
};