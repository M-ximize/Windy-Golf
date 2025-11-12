#include "Goalbox.h"
#include "LineRenderer.h"

GoalBox::GoalBox(Vec2 _position, GolfPhysScene* _scene) : PhysObject(ShapeType::GOALBOX)
{
	m_position = _position;
	sceneRef = _scene;
}

GoalBox::~GoalBox()
{
}

void GoalBox::fixedUpdate(Vec2 _force, float _gravity, float _timeStep)
{
}

void GoalBox::Draw(LineRenderer* lines) const
{
	lines->AddPointToLine({ m_position.x - GoalBoxSize, m_position.y + GoalBoxSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x + GoalBoxSize, m_position.y + GoalBoxSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x + GoalBoxSize, m_position.y - GoalBoxSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x - GoalBoxSize, m_position.y - GoalBoxSize }, Colour::WHITE);
	lines->FinishLineLoop();
	lines->DrawCross(m_position, GoalBoxSize);
}

void GoalBox::resetPosition()
{
}
