#pragma once

#include <vector>

#include <QtGui/QPainter>
#include <QRectF>

#include "Vector2f.h"
#include "Ball.h"

//  ласс PoolTableScene реализует сцену биль€рдного стола, с шариками и стенками.
class PoolTableScene
{
public:
	explicit PoolTableScene(const QRectF& bounds);

	void update(float deltaSeconds);
	void redraw(QPainter& painter);

	QRectF bounds() const;
	void setBounds(const QRectF& bound);

    bool tryAddBall(const Ball& ball);
private:
    void collide(Ball& b1, Ball& b2);
    float getCollisionTime(const Ball& b1, const Ball& b2);
    void updateRemainingTime(float dt);
    void removeDeadBalls();

	std::vector<Ball> m_balls;
	QRectF m_bounds;
};
