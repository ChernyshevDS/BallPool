#pragma once

#include <vector>

#include <QtGui/QPainter>
#include <QRectF>

#include "Vector2f.h"
#include "Ball.h"

// ����� PoolTableScene ��������� ����� ����������� �����, � �������� � ��������.
class PoolTableScene
{
public:
	explicit PoolTableScene(const QRectF& bounds);

	void update(float deltaSeconds);
	void redraw(QPainter& painter);

	QRectF bounds() const;
	void setBounds(const QRectF& bound);

private:

	std::vector<Ball> m_balls;
	QRectF m_bounds;
};
