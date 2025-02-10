#include "GolfGame.h"
#include "LineRenderer.h"
#include "TextStream.h"
#include "imgui.h"

#include <iostream>
#include <time.h>
#include <functional>

typedef bool(*fn)(PhysObject*, PhysObject*);

static fn collisionFunctionArray[] =
{
	GolfPhysScene::planeToPlane, GolfPhysScene::planeToCircle, GolfPhysScene::planeToGoalBox,
	GolfPhysScene::circleToPlane, GolfPhysScene::circleToCircle, GolfPhysScene::circleToGoalBox,
	GolfPhysScene::GoalBoxToPlane,GolfPhysScene::GoalBoxToCircle,GolfPhysScene::GoalBoxToGoalBox
};

GolfPhysScene::GolfPhysScene()
{
	appInfo.appName = "Windy Golf: EZ-BREEZY Edition";
	appInfo.grid.show = false;
	//appInfo.grid.mainAxesColour = Colour::GREEN.Lighten().Multiply(0.2f);
	//appInfo.grid.linesColour = Colour::GREEN.Lighten().Multiply(0.2f);
	//appInfo.grid.extent = 25;
	//appInfo.grid.showBasisLines = false;
	TimeStep = 0.01f;
	Gravity = 1.0f;
	appInfo.camera.disable = true;
	srand(time(NULL));
}

GolfPhysScene::~GolfPhysScene()
{
	for (auto PhysActor : PhysActors)
	{
		delete PhysActor;
	}
}

void GolfPhysScene::Initialise()
{
	// Creates GoalBox and zooms out grid
	CreateGoal();
	ScaleCameraHeight(5.1f);

	Plane* southWall;
	southWall = new Plane({ 0, 1 }, -25, WallBounce);
	addActor(southWall);

	Plane* northWall;
	northWall = new Plane({ 0, -1 }, -25, WallBounce);
	addActor(northWall);

	Plane* eastWall;
	eastWall = new Plane({ 1, 0 }, -25, WallBounce);
	addActor(eastWall);

	Plane* westWall;
	westWall = new Plane({ -1, 0 }, -25, WallBounce);
	addActor(westWall);
}

void GolfPhysScene::addActor(PhysObject* actor)
{
	PhysActors.push_back(actor);
}

void GolfPhysScene::removeActor(PhysObject* actor)
{
	int actorCount = PhysActors.size() - 1;
	for (int i = 0; i <= actorCount; i++)
	{
		if (PhysActors[i] == actor)
		{
			PhysActors.erase(PhysActors.begin() + i);
			delete actor;
			return;
		}
	}
}

void GolfPhysScene::Update(float delta)
{
	static float accumaltedTime = 0.0f;
	accumaltedTime += delta;

	while (accumaltedTime >= TimeStep)
	{
		for (auto physActor : PhysActors)
		{
			physActor->fixedUpdate(WindSpeed, Gravity, TimeStep); // Wind force, Gravity force, Timestep
		}
		accumaltedTime -= TimeStep;

		int actorCount = PhysActors.size();

		// Iterate through all physics actors, check and resolve any collisions.
		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysObject* actorA = PhysActors[outer];
				PhysObject* actorB = PhysActors[inner];
				int actorAID = actorA->getShapeType();
				int actorBID = actorB->getShapeType();

				int functionIdx = (actorAID * 3) + actorBID;
				fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
				if (collisionFunctionPtr != nullptr)
				{
					collisionFunctionPtr(actorA, actorB);
				}
				actorCount = PhysActors.size();
			}
		}
	}
	
	// Drawing
	draw();

	lines->DrawText("WIND:", { 30, 6 }, 2.0f, Colour::GREEN);
	lines->DrawText("+", { 31,-8 }, 10);
	lines->DrawLineWithArrow({ 36,0 }, ((WindSpeed / (MaxWindForce/10)) + Vec2{ 36, 0 }), Colour::GREEN, 1.0f);

	TextStream Title(lines, Vec2{ -45.0f, 18.0f }, 4.0f);
	Title << "Windy\nGolf";
	TextStream Controls(lines, Vec2{ -45.0f, 6.0f }, 1.0f);
	Controls << "Left/Right mouse:\nLaunch golf ball\nScroll wheel:\nSwing force: " << ClubForce ;
	TextStream Score(lines, Vec2{ -45.0f, -4.0f }, 1.0f);
	Score << "Balls shot: " << BallTotal << "\nBalls sunk: " << GoalCount <<"\nBalls in play: " << BallCount << "\n\nCombo: " << CurrentCombo << "\nBest Combo: " << TopCombo;


	// Aiming visual
	if (leftMouseDown || rightMouseDown)
	{
		lines->DrawLineSegment(cursorPos, { 0,0 }, Colour::GREY.Multiply(0.5f));
		lines->DrawLineWithArrow({0,0}, ( - cursorPos/5)*ClubForce);
	}
}

void GolfPhysScene::draw()
{
	for (auto PhysActor : PhysActors)
	{
		PhysActor->Draw(lines);
	}
}

// Shoot ball
void GolfPhysScene::OnLeftRelease()
{
	Circle* newBall;
	newBall = new Circle({ 0,0 }, (-cursorPos * ClubForce), 10, 0.5f, 1.0f, Colour::WHITE);
	addActor(newBall);
	BallCount++;
	BallTotal++;
	CurrentCombo = 0;
}

void GolfPhysScene::OnRightRelease()
{
	Circle* newBall;
	newBall = new Circle({ 0,0 }, (-cursorPos * ClubForce), 100, 0.9f, 0.5f, Colour::GREY);
	addActor(newBall);
	BallCount++;
	BallTotal++;
	CurrentCombo = 0;
}

void GolfPhysScene::OnMiddleClick() // Testing
{
	if (DebugState == false)
	{
		for (auto physActor : PhysActors)
		{
			physActor->debug = true;
		}
		DebugState = true;
	}
	else
	{
		for (auto physActor : PhysActors)
		{
			physActor->debug = false;
		}
		DebugState = false;
	}
}

void GolfPhysScene::OnMouseScroll(bool positive)
{
	if (positive)
	{
		ClubForce += 0.25f;
		if (ClubForce > 5.0f)
		{
			ClubForce = 5.0f;
		}
	}
	else
	{
		ClubForce -= 0.25f;
		if (ClubForce < 0.25f)
		{
			ClubForce = 0.25f;
		}
	}
}

// Shape Collisions

bool GolfPhysScene::circleToCircle(PhysObject* actorA, PhysObject* actorB)
{
	Circle* circleA = dynamic_cast<Circle*>(actorA);
	Circle* circleB = dynamic_cast<Circle*>(actorB);
	if (circleA != nullptr && circleB != nullptr)
	{
		Vec2 centreDisplacement = circleB->getPosition() - circleA->getPosition();
		float distance = centreDisplacement.GetMagnitude();
		float seperation = distance - circleA->getRadius() - circleB->getRadius();
		if (seperation < 0.0f)
		{
			circleA->resolveCollision(circleB, 0.5f * (circleA->getPosition() + circleB->getPosition()));
		}

		return true;
	}
	return false;
}

bool GolfPhysScene::circleToPlane(PhysObject* actorA, PhysObject* actorB)
{
	Circle* circle = dynamic_cast<Circle*>(actorA);
	Plane* plane = dynamic_cast<Plane*>(actorB);
	if (circle != nullptr && plane != nullptr)
	{
		float circleToPlane = Dot(circle->getPosition(), plane->getNormal()) - plane->getDistance();
		float intersection = circle->getRadius() - circleToPlane;
		float velocityOutOfPlane = Dot(circle->getVelocity(), plane->getNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			plane->resolveCollision(circle, (circle->getPosition()+(plane->getNormal()*-circle->getRadius())));
			return true;
		}

	}
	return false;
}

bool GolfPhysScene::planeToCircle(PhysObject* actorA, PhysObject* actorB)
{
	return circleToPlane(actorB, actorA);
}

bool GolfPhysScene::GoalBoxToCircle(PhysObject* actorA, PhysObject* actorB)
{
	return circleToGoalBox(actorB, actorA);
}

bool GolfPhysScene::circleToGoalBox(PhysObject* actorA, PhysObject* actorB)
{
	Circle* circle = dynamic_cast<Circle*>(actorA);
	GoalBox* box = dynamic_cast<GoalBox*>(actorB);
	if (circle != nullptr && box != nullptr)
	{
		Vec2 centreDisplacement = box->getPosition() - circle->getPosition();
		float distance = centreDisplacement.GetMagnitude();
		float seperation = distance - circle->getRadius() - 0.5f;
		if (seperation < 0.0f)
		{
			GolfPhysScene* scene = box->getScene();
			scene->ScoreGoal(box, circle);
			return true;
		}
	}
	return false;
}

void GolfPhysScene::CreateGoal()
{
	// Set a random wind speed
	float windX = (rand() % MaxWindForce) - MaxWindForce / 2.0f;
	float windY = (rand() % MaxWindForce) - MaxWindForce / 2.0f;
	WindSpeed = { windX, windY };

	// Set a new random goal position
	float goalX = (rand() % 49) - 24;
	float goalY = (rand() % 49) - 24;
	GoalPos = {goalX, goalY};
	
	// Create the new goal at the random position
	GoalBox* box;
	box = new GoalBox(GoalPos, this);
	addActor(box);
}

void GolfPhysScene::ScoreGoal(PhysObject* box, PhysObject* Ball)
{
	// remove and delete the current goal and the instigating ball. Then create a new goal and add 1 to players score.
	removeActor(box);
	removeActor(Ball);
	CreateGoal();
	GoalCount++;
	BallCount--;
	CurrentCombo++;
	if (CurrentCombo > TopCombo)
	{
		TopCombo = CurrentCombo;
	}
}

