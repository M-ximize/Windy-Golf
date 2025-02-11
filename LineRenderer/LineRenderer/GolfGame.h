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
	float ClubForce = 1.0f;

	int CurrentCombo = 0; // All handled in runtime, keep at 0.
	int TopCombo = 0;
	int GoalCount = 0;
	int BallCount = 0;
	int BallTotal = 0;

	int selectBall = 0; // Currently selected ball, will reset to 0 if out of range.
	float selectMass = 10.0f;
	float selectRadius = 0.5f;
	float selectElastic = 1.0f;
	Colour selectColour = Colour::WHITE;

	// Game variables
	float WallBounce = 0.7f; // Elasticity of map bounds
	int MaxWindForce = 20; // Max possible acceleration of wind
	float windDivider = 100; // For wind updates
	bool DebugState = false; // Toggle accel display on balls
	Vec2 GoalPos;
	Vec2 WindSpeed;
	float Gravity;
	float TimeStep;
	std::vector<PhysObject*> PhysActors;

	std::vector<const char*> GameTitles = {
		"Windy Golf: EZ-BREEZY Edition", 
		"Windy Golf: Hurricane Season", 
		"Windy Golf: 1998 Championship Edition", 
		"Windy Golf: The ultimate game of golf with strong winds",
		"Windy Golf: Secret developer version"
	};

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