#include "PoolTableScene.h"
#include "fuzzy.h"

PoolTableScene::PoolTableScene(const QRectF &bounds)
	: m_bounds(bounds)
{
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
                Vector2f impulse_before = b1.impulse() + b2.impulse();
                float energy_before = b1.energy() + b2.energy();

                // возвращаем шары на исходную
                b1.move(-deltaSeconds);
                b2.move(-deltaSeconds);
                // вычисляем точное время до столкновения
                float tc = getCollisionTime(b1, b2);
                b1.move(tc);
                b2.move(tc);
                collide(b1, b2);
                // используем остаток времени после столкновения
                b1.move(deltaSeconds - tc);
                b2.move(deltaSeconds - tc);

                Vector2f impulse_after = b1.impulse() + b2.impulse();
                float energy_after = b1.energy() + b2.energy();

                Q_ASSERT(areFuzzyEqual(impulse_before.x, impulse_after.x));
                Q_ASSERT(areFuzzyEqual(impulse_before.y, impulse_after.y));
                Q_ASSERT(areFuzzyEqual(energy_before, energy_after));
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

bool PoolTableScene::tryAddBall(const Ball &ball)
{
    for(const auto& b: m_balls){
        if(b.bbox().intersects(ball.bbox()))
            return false;
    }
    m_balls.push_back(ball);
    return true;
}

void PoolTableScene::collide(Ball & b1, Ball & b2)
{
	Vector2f dist_vec = b1.center() - b2.center();
	Vector2f dv = b1.speed() - b2.speed();
	float dist_squared = std::pow(dist_vec.length(), 2);
    float factor = dot(dv, dist_vec) / dist_squared;

    float mass_f1 = 2 * b2.mass() / (b1.mass() + b2.mass());
    float mass_f2 = 2 * b1.mass() / (b1.mass() + b2.mass());

    Vector2f w1 = b1.speed() - (mass_f1 * factor * dist_vec);
    Vector2f w2 = b2.speed() - (mass_f2 * factor * -dist_vec);

	b1.setSpeed(w1);
	b2.setSpeed(w2);
}

float PoolTableScene::getCollisionTime(const Ball &b1, const Ball &b2)
{
    //     v1        v2
    //  ( )-->    ( )->
    //   ----------> cc
    Vector2f cc = b2.center() - b1.center();
    float v1 = b1.speed().projectOn(cc);
    float v2 = b2.speed().projectOn(cc);
    float appr_speed = v1 - v2;
    float dist_now = cc.length();
    float ds = dist_now - (b1.radius() + b2.radius());
    return ds / appr_speed;
}
