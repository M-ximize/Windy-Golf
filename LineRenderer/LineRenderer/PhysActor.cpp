#include "PhysActor.h"
#include "LineRenderer.h"



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
	m_angularVelocity -= m_angularVelocity * m_angularDrag * _timeStep;

	if (m_velocity.GetMagnitude() < 0.05f) {
		m_velocity = { 0,0 };
	}
	if (abs(m_angularVelocity) < 0.01f) {
		m_angularVelocity = 0;
	}

	m_position += m_velocity * _timeStep;
	applyForce(_force * m_mass * _timeStep, { 0,0 });

	m_orientation += m_angularVelocity * _timeStep;
}

void Rigidbody::applyForce(Vec2 _force, Vec2 _pos)
{
	m_velocity += _force / getMass();
	m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / getMoment();
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


// Circle Functions

Circle::Circle(Vec2 _position, Vec2 _velocity, float _mass, float _radius, float _elasticity, Colour _colour) :
	Rigidbody(CIRCLE, _position, _velocity, 0, _mass, _elasticity)
{
	m_mass = _mass;
	m_radius = _radius;
	m_elasticity = _elasticity;
	m_moment = 0.5f * m_mass * m_radius * m_radius;
	m_colour = _colour;
}

Circle::~Circle()
{
}

void Circle::Draw(LineRenderer* lines) const
{
	lines->DrawCircle(m_position, m_radius, m_colour);

	if (debug)
	{
		lines->DrawLineSegment(m_position, m_position + m_velocity, m_colour);
	}

	if (m_velocity.x > 50.0f || m_velocity.x < -50.0f || m_velocity.y > 50.0f || m_velocity.y < -50.0f)
	{
		lines->DrawCircle(m_position-(m_velocity/75), m_radius/1.4, Colour::YELLOW);
		lines->DrawCircle(m_position-(m_velocity/150), m_radius / 1.1, Colour::ORANGE);
		lines->DrawCircle(m_position, m_radius + 0.1f, Colour::RED);
	}
}


// Plane Functions

Plane::Plane(Vec2 _normal, float _distance, float _elasticity) : PhysObject(ShapeType::PLANE, _elasticity)
{
	m_normal = _normal;
	m_distanceToOrigin = _distance;
	m_elasticity = _elasticity;
}

void Plane::fixedUpdate(Vec2 _force, float _gravity, float _timeStep)
{
}

void Plane::Draw(LineRenderer* lines) const
{
	float planeLength = 25;
	Vec2 centerPoint = m_normal * m_distanceToOrigin;
	Vec2 parallel(m_normal.y, -m_normal.x);
	lines->DrawLineSegment({ centerPoint + (parallel * planeLength) }, { centerPoint - (parallel * planeLength) }, Colour::GREEN.Lighten());
}

void Plane::resetPosition()
{
}

void Plane::resolveCollision(Rigidbody* _otherActor, Vec2 _contact)
{
	Vec2 localContact = _contact - _otherActor->getPosition();
	Vec2 vRel = _otherActor->getVelocity() + _otherActor->getAngularVelocity() * Vec2(-localContact.y, localContact.x);
	float velIntoPlane = Dot(vRel, m_normal);

	float elasticity = (getElasticity() + _otherActor->getElasticity()) / 2.0f;
	float j = Dot(-(1 + elasticity) * (vRel), m_normal) / (1 / _otherActor->getMass());

	Vec2 force = m_normal * j;

	_otherActor->applyForce(force, _contact - _otherActor->getPosition());
}

// Hole Functions

Hole::Hole(Vec2 _position, GolfPhysScene* _scene) : PhysObject(ShapeType::HOLE)
{
	m_position = _position;
	sceneRef = _scene;
}

void Hole::fixedUpdate(Vec2 _force, float _gravity, float _timeStep)
{
}

void Hole::Draw(LineRenderer* lines) const
{
	lines->AddPointToLine({ m_position.x - holeSize, m_position.y + holeSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x + holeSize, m_position.y + holeSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x + holeSize, m_position.y - holeSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x - holeSize, m_position.y - holeSize }, Colour::WHITE);
	lines->FinishLineLoop();
	lines->DrawCross(m_position, holeSize);
}

void Hole::resetPosition()
{
}
