#pragma once

#include "PhysActor.h"

class Box : public PhysObject
{
public:
	Box(Vec2 _position, float _size);
	~Box();

	virtual void fixedUpdate(Vec2 _force, float _gravity, float _timeStep) override;
	virtual void Draw(LineRenderer* lines) const override;
	virtual void resetPosition() override;

	void resolveCollision(Rigidbody* _otherActor, Vec2 _contact, Vec2* _direction);

	Vec2 getPosition() const { return m_position; }
	Vec2 getExtents() const { return m_extents; }

	Vec2 getlocalX() const { return m_localX; }
	Vec2 getlocalY() const { return m_localY; }

protected:
	float m_boxSize = 1.0f;
	Vec2 m_position;

	Vec2 m_extents;
	
	Vec2 m_localX;
	Vec2 m_localY;

};