#include "Ball.h"


Ball::Ball(float r, Vector2f center, QColor color)
	: m_radius(r), m_center(center), m_color(color)
{
}

Ball::~Ball()
{
}

void Ball::move(float dt)
{
	m_center.x += m_speed.x * dt;
	m_center.y += m_speed.y * dt;
}

void Ball::draw(QPainter & painter)
{
	//painter.setBrush(Qt::black);
	//painter.drawRect(bbox());
	painter.setBrush(QBrush(m_color));
	painter.drawEllipse(QPointF(m_center.x, m_center.y), m_radius, m_radius);
	
}

QRectF Ball::bbox() const
{
	return QRectF(m_center.x - m_radius, m_center.y - m_radius, m_radius * 2, m_radius * 2);
}


