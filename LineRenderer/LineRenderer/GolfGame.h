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
	float ClubForce = 1.0f;
	int GoalCount = 0;
	int BallCount = 0;
	int BallTotal = 0;
	int CurrentCombo = 0;
	int TopCombo = 0;

	int selectBall = 0;
	float selectMass = 10.0f;
	float selectRadius = 0.5f;
	float selectElastic = 1.0f;
	Colour selectColour = Colour::WHITE;

	// Game variables
	float Threshold = 0.5f;
	float WallBounce = 0.7f;
	int MaxWindForce = 20;
	bool DebugState = false;
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
	static bool planeToPlane(PhysObject*, PhysObject*) { return false; }
	static bool circleToPlane(PhysObject*, PhysObject*);
	static bool planeToCircle(PhysObject*, PhysObject*);
	static bool GoalBoxToGoalBox(PhysObject*, PhysObject*) { return false; }
	static bool GoalBoxToCircle(PhysObject*, PhysObject*);
	static bool circleToGoalBox(PhysObject*, PhysObject*);
	static bool GoalBoxToPlane(PhysObject*, PhysObject*) { return false; }
	static bool planeToGoalBox(PhysObject*, PhysObject*) { return false; }

	// Game Functions

	void CreateGoal();

	void ScoreGoal(PhysObject* GoalBox, PhysObject* Ball);

	// Controls

	void OnLeftRelease() override;

	void OnRightClick() override;

	void OnMiddleClick() override;

	void OnMouseScroll(bool positive) override;
};