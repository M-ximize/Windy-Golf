#include "Box.h"

#include "glm/glm.hpp"

#include "LineRenderer.h"

Box::Box(Vec2 _position, float _size) : PhysObject(ShapeType::BOX)
{
	m_position = _position;
	m_boxSize = _size;
	m_extents = { _size,_size };
}

Box::~Box()
{
}

void Box::fixedUpdate(Vec2 _force, float _gravity, float _timeStep)
{
	float cs = cosf(0);
	float sn = sinf(0);
	m_localX = { glm::normalize(glm::vec2(cs, sn)).x, glm::normalize(glm::vec2(cs, sn)).y };
	m_localY = { glm::normalize(glm::vec2(-sn, cs)).x, glm::normalize(glm::vec2(-sn, cs)).y };
}

void Box::Draw(LineRenderer* lines) const
{
	lines->AddPointToLine({ m_position.x - m_boxSize, m_position.y + m_boxSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x + m_boxSize, m_position.y + m_boxSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x + m_boxSize, m_position.y - m_boxSize }, Colour::WHITE);
	lines->AddPointToLine({ m_position.x - m_boxSize, m_position.y - m_boxSize }, Colour::WHITE);
	lines->FinishLineLoop();
}

void Box::resetPosition()
{
}

void Box::resolveCollision(Rigidbody* _otherActor, Vec2 _contact, Vec2* _direction)
{
	Vec2 localContact = _contact - _otherActor->getPosition();
	Vec2 m_normal = (_direction ? *_direction : _otherActor->getPosition() - m_position).Normalise();
	Vec2 vRel = _otherActor->getVelocity() + _otherActor->getAngularVelocity() * Vec2(-localContact.y, localContact.x);

	float elasticity = (getElasticity() + _otherActor->getElasticity()) / 2.0f;
	float j = Dot(-(1 + elasticity) * (vRel), m_normal) / (1 / _otherActor->getMass());

	Vec2 force = m_normal * j;

	_otherActor->applyForce(force, _contact - _otherActor->getPosition());
}
