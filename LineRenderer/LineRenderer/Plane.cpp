#include "Plane.h"

#include "LineRenderer.h"

Plane::Plane(Vec2 _normal, float _distance, float _elasticity) : PhysObject(ShapeType::PLANE, _elasticity)
{
	m_normal = _normal;
	m_distanceToOrigin = _distance;
	m_elasticity = _elasticity;
}

Plane::~Plane()
{
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