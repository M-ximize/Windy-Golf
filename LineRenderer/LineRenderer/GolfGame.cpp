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
	GolfPhysScene::planeToPlane, GolfPhysScene::planeToCircle, GolfPhysScene::planeToGoalBox, GolfPhysScene::planeToBox,
	GolfPhysScene::circleToPlane, GolfPhysScene::circleToCircle, GolfPhysScene::circleToGoalBox, GolfPhysScene::circleToBox,
	GolfPhysScene::GoalBoxToPlane,GolfPhysScene::GoalBoxToCircle,GolfPhysScene::GoalBoxToGoalBox, GolfPhysScene::goalBoxToBox,
	GolfPhysScene::boxToPlane, GolfPhysScene::boxToCircle, GolfPhysScene::boxToGoalBox, GolfPhysScene::boxToBox
};


GolfPhysScene::GolfPhysScene()
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
	TimeStep = 0.01f;
	Gravity = 0.0f;
	appInfo.camera.disable = true;
	
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
	int actorCount = (int)PhysActors.size() - 1;
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

		int actorCount = (int)PhysActors.size();

		// Iterate through all physics actors, check and resolve any collisions.
		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysObject* actorA = PhysActors[outer];
				PhysObject* actorB = PhysActors[inner];
				int actorAID = actorA->getShapeType();
				int actorBID = actorB->getShapeType();

				int functionIdx = (actorAID * 4) + actorBID;
				fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
				if (collisionFunctionPtr != nullptr)
				{
					collisionFunctionPtr(actorA, actorB);
				}
				actorCount = (int)PhysActors.size();
			}
		}
		float windX = (rand() % (MaxWindForce + 1)) - (MaxWindForce / 2.0f);
		if (WindSpeed.x + windX/ windDivider > MaxWindForce / 2 || WindSpeed.x + windX/ windDivider < -MaxWindForce / 2)
		{
			windX = 0;
		}
		float windY = (rand() % (MaxWindForce + 1)) - (MaxWindForce / 2.0f);
		if (WindSpeed.y + windY/ windDivider > MaxWindForce / 2 || WindSpeed.y + windY/ windDivider < -MaxWindForce / 2)
		{
			windY = 0;
		}
		WindSpeed = { WindSpeed.x + (windX / windDivider), WindSpeed.y + (windY / windDivider) };
	}
	
	// Drawing
	draw();

	lines->DrawText("WIND:", { 30, 16 }, 2.0f, Colour::GREEN);
	lines->DrawText("+", { 31,2 }, 10);
	lines->DrawLineWithArrow({ 36,10 }, ((WindSpeed / ((float)MaxWindForce/10.0f)) + Vec2{ 36, 10 }), Colour::GREEN, 1.0f);

	TextStream Title(lines, Vec2{ -45.0f, 18.0f }, 4.0f);
	Title << "Windy\nGolf";
	TextStream Controls(lines, Vec2{ -45.0f, 6.0f }, 1.0f);
	Controls << "Left mouse:\nLaunch golf ball\nRight mouse:\nChange ball\nScroll wheel:\nSwing force: " << ClubForce ;
	TextStream Score(lines, Vec2{ -45.0f, -10.0f }, 1.0f);
	Score << "Balls shot: " << BallTotal << "\nBalls sunk: " << GoalCount <<"\nBalls in play: " << BallCount << "\n\nCombo: " << CurrentCombo << "\nBest Combo: " << TopCombo;
	lines->DrawCircle({ 35, -3 }, selectRadius, selectColour);
	TextStream BallSelect(lines, { 28, 0 }, 1.0f);
	BallSelect << "Selected Ball:\n\n\nSize: " << selectRadius << "\nMass: " << selectMass << "\nBounce: " << selectElastic;


	// Aiming visual
	if (leftMouseDown)
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

#pragma region Controls

// Shoot ball
void GolfPhysScene::OnLeftRelease()
{
	Circle* newBall;
	newBall = new Circle({ 0,0 }, (-cursorPos * ClubForce), selectMass, selectRadius, selectElastic, selectColour);
	addActor(newBall);
	BallCount++;
	BallTotal++;
	CurrentCombo = 0;
}

void GolfPhysScene::OnRightClick() // Cycle Ball types
{
	selectBall++;
	switch (selectBall)
	{
		case 1: // Cannon Ball
		{
			selectMass = 100.0f;
			selectRadius = 1.0f;
			selectElastic = 0.5f;
			selectColour = Colour::GREY;
			break;
		}
		case 2: // Bouncy Ball
		{
			selectMass = 1.0f;
			selectRadius = 0.35f;
			selectElastic = 1.2f;
			selectColour = Colour::RED.Lighten();
			break;
		}
		case 3: // Super Bouncy Ball
		{
			selectMass = 1.0f;
			selectRadius = 0.25f;
			selectElastic = 1.5f;
			selectColour = Colour::BLUE.Lighten();
			break;
		}
		case 4: // Unbouncy Ball
		{
			selectMass = 1.0f;
			selectRadius = 0.3f;
			selectElastic = -0.7f;
			selectColour = Colour::MAGENTA;
			break;
		}
		case 5: // Random Ball
		{
			selectMass = float(rand() % 100);
			selectRadius = 0.05f + (rand() % 100) / static_cast<float>(50);
			selectElastic = (rand() % 100) / static_cast<float>(100);
			selectColour = Colour::YELLOW;
			break;
		}
		default: // Golf Ball
		{
			selectMass = 10.0f;
			selectRadius = 0.5f;
			selectElastic = 1.0f;
			selectColour = Colour::WHITE;
			selectBall = 0;
			break;
		}
	}
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

#pragma endregion

#pragma region ShapeCollision

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

bool GolfPhysScene::boxToCircle(PhysObject* actorA, PhysObject* actorB)
{
	Box* box = dynamic_cast<Box*>(actorA);
	Circle* circle = dynamic_cast<Circle*>(actorB);

	if (box != nullptr && circle != nullptr)
	{
		// convert circle into the box space instead of world
		Vec2 circlePosWorld = circle->getPosition() - box->getPosition(); // Difference between the circle and the box in the world.
		Vec2 circlePosBox = Vec2(Dot(circlePosWorld, box->getlocalX()), 
			Dot(circlePosWorld, box->getlocalY()));

		// closest point of circle centre on the box
		Vec2 closestPointOnBox = circlePosBox;
		Vec2 extents = box->getExtents();
		if (closestPointOnBox.x < -extents.x) closestPointOnBox.x = -extents.x;
		if (closestPointOnBox.x > extents.x) closestPointOnBox.x = extents.x;
		if (closestPointOnBox.y < -extents.y) closestPointOnBox.y = -extents.y;
		if (closestPointOnBox.y > extents.y) closestPointOnBox.y = extents.y;

		// convert back to world
		Vec2 closestPointOnBoxWorld = box->getPosition() + 
			closestPointOnBox.x * box->getlocalX() + 
			closestPointOnBox.y * box->getlocalY();
		Vec2 circleToBox = circle->getPosition() - closestPointOnBoxWorld;

		if (glm::length(glm::vec2{ circleToBox.x, circleToBox.y }) < circle->getRadius())
		{
			glm::vec2 GLMdirection = glm::normalize(glm::vec2{ circleToBox.x, circleToBox.y });
			Vec2 direction = { GLMdirection.x, GLMdirection.y };
			Vec2 contact = closestPointOnBoxWorld;
			box->resolveCollision(circle, contact, &direction);
			return true;
		}
	}
	return false;
}

bool GolfPhysScene::circleToBox(PhysObject* actorA, PhysObject* actorB)
{
	return boxToCircle(actorB, actorA);
}

bool GolfPhysScene::boxToGoalBox(PhysObject* actorA, PhysObject* actorB)
{
	Box* box = dynamic_cast<Box*>(actorA);
	GoalBox* goal = dynamic_cast<GoalBox*>(actorB);

	if (box != nullptr && goal != nullptr)
	{
		// convert circle into the box space instead of world
		Vec2 circlePosWorld = goal->getPosition() - box->getPosition(); // Difference between the circle and the box in the world.
		Vec2 circlePosBox = Vec2(Dot(circlePosWorld, box->getlocalX()),
			Dot(circlePosWorld, box->getlocalY()));

		// closest point of circle centre on the box
		Vec2 closestPointOnBox = circlePosBox;
		Vec2 extents = box->getExtents();
		if (closestPointOnBox.x < -extents.x) closestPointOnBox.x = -extents.x;
		if (closestPointOnBox.x > extents.x) closestPointOnBox.x = extents.x;
		if (closestPointOnBox.y < -extents.y) closestPointOnBox.y = -extents.y;
		if (closestPointOnBox.y > extents.y) closestPointOnBox.y = extents.y;

		// convert back to world
		Vec2 closestPointOnBoxWorld = box->getPosition() +
			closestPointOnBox.x * box->getlocalX() +
			closestPointOnBox.y * box->getlocalY();
		Vec2 circleToBox = goal->getPosition() - closestPointOnBoxWorld;

		if (glm::length(glm::vec2{ circleToBox.x, circleToBox.y }) < 4.0f)
		{
			glm::vec2 GLMdirection = glm::normalize(glm::vec2{ circleToBox.x, circleToBox.y });
			Vec2 direction = { GLMdirection.x, GLMdirection.y };
			Vec2 contact = closestPointOnBoxWorld;
			GolfPhysScene* scene = goal->getScene();
			scene->removeActor(box);
			return true;
		}
	}
	return false;
}

bool GolfPhysScene::goalBoxToBox(PhysObject* actorA, PhysObject* actorB)
{
	return false;
}

#pragma endregion

void GolfPhysScene::CreateGoal()
{
	// Set a random wind speed
	float windX = (rand() % MaxWindForce) - MaxWindForce / 2.0f;
	float windY = (rand() % MaxWindForce) - MaxWindForce / 2.0f;
	WindSpeed = { windX, windY };

	// Set a new random goal position
	float goalX = ((rand() % 490) - 240) / static_cast<float>(10);
	float goalY = ((rand() % 490) - 240) / static_cast<float>(10);
	GoalPos = {goalX, goalY};
	
	// Create the new goal at the random position
	GoalBox* box;
	box = new GoalBox(GoalPos, this);
	addActor(box);
	
}

void GolfPhysScene::CreateBox()
{
	Box* StaticBox;

	float boxX = ((rand() % 490) - 240) / static_cast<float>(10);
	float boxY = ((rand() % 490) - 240) / static_cast<float>(10);

	StaticBox = new Box({ boxX, boxY }, 1);
	addActor(StaticBox);
}

void GolfPhysScene::ScoreGoal(PhysObject* box, PhysObject* Ball)
{
	// remove and delete the current goal and the instigating ball. Then create a new goal and add 1 to players score.
	removeActor(box);
	removeActor(Ball);

	// Set a new random goal position
	
	CreateBox();

	CreateGoal();
	GoalCount++;
	BallCount--;
	CurrentCombo++;
	if (CurrentCombo > TopCombo)
	{
		TopCombo = CurrentCombo;
	}
}

