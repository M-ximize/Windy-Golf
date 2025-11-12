#include "Sphere.h"
#include "LineRenderer.h"

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
		lines->DrawCircle(m_position - (m_velocity / 75), m_radius / 1.4f, Colour::YELLOW);
		lines->DrawCircle(m_position - (m_velocity / 150), m_radius / 1.1f, Colour::ORANGE);
		lines->DrawCircle(m_position, m_radius + 0.1f, Colour::RED);
	}
}
