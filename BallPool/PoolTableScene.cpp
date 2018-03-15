#include "PoolTableScene.h"

/*namespace
{
	// Скорость, пикселей в секунду
	constexpr float BALL_SPEED_X = 250;
	constexpr float BALL_SPEED_Y = 360;

	constexpr float BALL_SIZE = 40;
}*/

PoolTableScene::PoolTableScene(const QRectF &bounds)
	: m_bounds(bounds)
{
	Ball b(20.f, Vector2f(50, 50));
	b.setSpeed(Vector2f(250, 350));
	m_balls.push_back(b);

	Ball b2(30, Vector2f(100, 200), Qt::red);
	b2.setSpeed(Vector2f(100, 400));
	m_balls.push_back(b2);
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
