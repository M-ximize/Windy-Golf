#pragma once

#include "PhysActor.h"

class GoalBox : public PhysObject
{
public:
	GoalBox(Vec2 _position, GolfPhysScene* _scene);
	~GoalBox();

	virtual void fixedUpdate(Vec2 _force, float _gravity, float _timeStep) override;
	virtual void Draw(LineRenderer* lines) const override;
	virtual void resetPosition() override;

	Vec2 getPosition() { return m_position; }
	GolfPhysScene* getScene() { return sceneRef; }

protected:
	float GoalBoxSize = 0.65f;
	Vec2 m_position;
	GolfPhysScene* sceneRef;
};