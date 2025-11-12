#pragma once

#include "PhysActor.h"

class Circle : public Rigidbody
{
public:
	Circle(Vec2 _position, Vec2 _velocity, float _mass, float _radius, float _elasticity, Colour _colour);
	~Circle();

	virtual void Draw(LineRenderer* lines) const override;

	float getRadius() const { return m_radius; }
	Colour getColour() const { return m_colour; }

protected:
	float m_radius;
	Colour m_colour;
};