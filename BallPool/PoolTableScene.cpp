#include "PoolTableScene.h"

PoolTableScene::PoolTableScene(const QRectF &bounds)
	: m_bounds(bounds)
{
	Ball b(20, Vector2f(50, 50));
	b.setSpeed(Vector2f(300, 0));
	m_balls.push_back(b);

	Ball b2(20, Vector2f(350, 52), Qt::red);
	b2.setSpeed(Vector2f(-100, 0));
	m_balls.push_back(b2);

	Ball b3(20, Vector2f(100, 252), Qt::blue);
	b3.setSpeed(Vector2f(0, -200));
	m_balls.push_back(b3);
}


void PoolTableScene::update(float deltaSeconds)
{
	for (auto &ball : m_balls) {
		ball.move(deltaSeconds);
		if (ball.bbox().right() > m_bounds.right())
		{
			ball.setCenter(Vector2f(m_bounds.right() - ball.radius(), ball.center().y));
			ball.setSpeed(Vector2f(-ball.speed().x, ball.speed().y));
		} else if (ball.bbox().left() < m_bounds.left())
		{
			ball.setCenter(Vector2f(m_bounds.left() + ball.radius(), ball.center().y));
			ball.setSpeed(Vector2f(-ball.speed().x, ball.speed().y));
		}

		if (ball.bbox().top() < m_bounds.top())
		{
			ball.setCenter(Vector2f(ball.center().x, m_bounds.top() + ball.radius()));
			ball.setSpeed(Vector2f(ball.speed().x, -ball.speed().y));
		}
		else
		if (ball.bbox().bottom() > m_bounds.bottom())
		{
			ball.setCenter(Vector2f(ball.center().x, m_bounds.bottom() - ball.radius()));
			ball.setSpeed(Vector2f(ball.speed().x, -ball.speed().y));
		}
	}

	for (size_t fi = 0; fi < m_balls.size(); ++fi)
	{
		for (size_t si = fi + 1; si < m_balls.size(); ++si)
		{
			Ball& b1 = m_balls[fi];
			Ball& b2 = m_balls[si];
			// проверяем столкновение fi, si
			if (dist(b1.center(), b2.center()) <= b1.radius() + b2.radius())
			{
				collide(b1, b2);
			}
		}
	}

}

void PoolTableScene::redraw(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing);
	for (auto ball : m_balls) {
		ball.draw(painter);
	}
}

QRectF PoolTableScene::bounds() const 
{ 
	return m_bounds; 
}

void PoolTableScene::setBounds(const QRectF & bound)
{
	m_bounds = bound;
}

void PoolTableScene::collide(Ball & b1, Ball & b2)
{
	Vector2f dist_vec = b1.center() - b2.center();
	Vector2f dv = b1.speed() - b2.speed();
	float dist_squared = std::pow(dist_vec.length(), 2);

	Vector2f w1 = b1.speed() - ((dot(dv, dist_vec) / dist_squared) * dist_vec);
	Vector2f w2 = b2.speed() - ((dot(dv, dist_vec) / dist_squared) * -dist_vec);

	b1.setSpeed(w1);
	b2.setSpeed(w2);
}
