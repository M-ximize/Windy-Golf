#include "PhysActor.h"
#include "LineRenderer.h"
#include "Goalbox.h"
#include "Plane.h"

#include "iostream"

Rigidbody::Rigidbody(ShapeType _shapeType, Vec2 _position, Vec2 _velocity, float _orientation, float _mass, float _elasticity)
{
	shapeType = _shapeType;
	m_position = _position;
	m_velocity = _velocity;
	m_orientation = _orientation;
	m_mass = _mass;
	debug = false;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(Vec2 _force, float _gravity, float _timeStep)
{
	m_velocity -= m_velocity * m_linearDrag * _timeStep;
	//m_angularVelocity -= m_angularVelocity * m_angularDrag * _timeStep;

	if (m_velocity.GetMagnitude() < 0.05f) {
		m_velocity = { 0,0 };
	}
	/*if (abs(m_angularVelocity) < 0.01f) {
		m_angularVelocity = 0;
	}*/

	m_position += m_velocity * _timeStep;
	applyForce(_force * _timeStep, { 0,0 });
	

	//m_orientation += m_angularVelocity * _timeStep;
}

void Rigidbody::applyForce(Vec2 _force, Vec2 _pos)
{
	m_velocity += _force / getMass();
	//m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / getMoment();
}


void Rigidbody::resolveCollision(Rigidbody* _otherActor, Vec2 _contact, Vec2* _collisionNormal)
{
	Vec2 normal = (_collisionNormal ? *_collisionNormal : _otherActor->m_position - m_position).Normalise();
	Vec2 perp(normal.y, -normal.x);

	float r1 = Dot(_contact - m_position, -perp);
	float r2 = Dot(_contact - _otherActor->m_position, perp);

	float v1 = Dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = Dot(_otherActor->m_velocity, normal) + r2 * _otherActor->m_angularVelocity;

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / _otherActor->m_mass + (r2 * r2) / _otherActor->m_moment);

		float elasticity = (getElasticity() + _otherActor->getElasticity()) / 2.0f;

		Vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		applyForce(-force, _contact - m_position);
		_otherActor->applyForce(force, _contact - _otherActor->m_position);
	}
}
