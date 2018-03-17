#pragma once
#include "Vector2f.h"
#include <QPainter>
#include <QColor>
#include <QRectF>

class Ball
{
public:
    Ball(float r, Vector2f center, Vector2f speed = Vector2f{}, QColor color = QColor(0xFA, 0xFE, 0x78));
	virtual ~Ball();

	void setRadius(float r) { m_radius = r; }
	float radius() const { return m_radius; }

	void setCenter(const Vector2f& pos) { m_center = pos; }
	Vector2f center() const { return m_center; }

	void setSpeed(const Vector2f& spd) { m_speed = spd; }
	Vector2f speed() const { return m_speed;  }
	
	void setColor(const QColor& color) { m_color = color; }
	QColor color() const { return m_color; }

    float mass() const;
    float energy() const;
    Vector2f impulse() const;

	void move(float dt);
	void draw(QPainter& painter);

	QRectF bbox() const;

private:
    float m_radius;
    Vector2f m_center;
	Vector2f m_speed;
	QColor m_color;
};

