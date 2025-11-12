#pragma once

#include "PhysActor.h"

class Plane : public PhysObject
{
public:
	Plane(Vec2 _normal, float _distance, float _elasticity);
	~Plane();

	virtual void fixedUpdate(Vec2 _force, float _gravity, float _timeStep) override;
	virtual void Draw(LineRenderer* lines) const override;
	virtual void resetPosition() override;
	void resolveCollision(Rigidbody* _otherActor, Vec2 _contact);

	Vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

protected:
	Vec2 m_normal;
	float m_distanceToOrigin;
};