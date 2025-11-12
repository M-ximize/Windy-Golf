#pragma once

class LineRenderer;
class GolfPhysScene;

#include "Colour.h"
#include "Maths.h"

enum ShapeType {
	PLANE = 0,
	CIRCLE,
	GOALBOX,
	BOX
};

// Base PhysObject class

class PhysObject
{
protected:
	PhysObject() : m_elasticity(1.0f) {}
	PhysObject(ShapeType _shapeType) : shapeType(_shapeType), m_elasticity(1.0f) {}
	PhysObject(ShapeType _shapeType, float _elasticity) : shapeType(_shapeType), m_elasticity(_elasticity) {}

public:
	virtual void fixedUpdate(Vec2 _force, float _gravity, float _timeStep) = 0;
	virtual void Draw(LineRenderer* lines) const = 0;
	virtual void resetPosition() {};
	virtual ShapeType getShapeType() { return shapeType; }

	float getElasticity() { return m_elasticity; }
	bool getDebug() { return debug; }

	bool debug;

protected:
	ShapeType shapeType;
	float m_elasticity;
};

// Derived PhysObject classes

class Rigidbody : public PhysObject 
{
public:
	Rigidbody(ShapeType _shapeType, Vec2 _position, Vec2 _velocity, float _orientation, float _mass, float _elasticity);
	~Rigidbody();

	virtual void fixedUpdate(Vec2 _force, float _gravity, float _timeStep);
	void applyForce(Vec2 _force, Vec2 _pos); // Rotational force
	virtual void resolveCollision(Rigidbody* _otherActor, Vec2 _contact, Vec2* collisionNormal = nullptr);

	Vec2 getPosition() const { return m_position; }
	float getOrientation() const { return m_orientation; }
	Vec2 getVelocity() const { return m_velocity; }
	float getAngularVelocity() const { return m_angularVelocity; }
	float getMass() const { return m_mass; }
	float getMoment() const { return m_moment; }

protected:
	Vec2 m_position;
	Vec2 m_velocity;
	float m_mass;
	float m_orientation = 0;
	float m_angularVelocity = 0;
	float m_moment;
	float m_linearDrag = 0.5f;
	float m_angularDrag = 0.5f;
};

