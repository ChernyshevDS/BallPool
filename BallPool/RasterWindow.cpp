#include "RasterWindow.h"

namespace 
{
	constexpr int WIN_WIDTH = 400;
	constexpr int WIN_HEIGHT = 400;

    constexpr int BALL_MIN_RADIUS = 10;
    constexpr int BALL_MAX_RADIUS = 50;

    constexpr int BALL_MIN_SPEED = 0;
    constexpr int BALL_MAX_SPEED = 500;

    constexpr int INIT_BALLS_CNT = 5;
}

// ����������� ������: ��������� ���� (������������) �������� ���� QWindow,
// � ���� ��������� ����� �������� ������������ ���� (��������, ��� ���������� �������)
RasterWindow::RasterWindow(QWindow *parent)
	: QWindow(parent)
	, m_backingStore(new QBackingStore(this))
	, m_scene(std::make_unique<PoolTableScene>(QRectF(0,0,WIN_WIDTH,WIN_HEIGHT)))
{
	setMinimumSize(QSize(WIN_WIDTH, WIN_HEIGHT));
	m_updateTimer.start();

    initRandomGenerator();

    int n_balls = 0;
    while(n_balls < INIT_BALLS_CNT){
        if(m_scene->tryAddBall(getRandomBall())){
            n_balls++;
        }
    }
}

// ���������� ��������� �������� - �������� ��� ���������
bool RasterWindow::isAnimating() const
{
	return m_isAnimating;
}

// �������� ��� ��������� ��������
void RasterWindow::setAnimating(bool isAnimating)
{
	m_isAnimating = isAnimating;
	if (isAnimating)
	{
		renderLater();
	}
}

// ����� event ���������� ����� ���������� ����� �������, ������� resizeEvent � exposeEvent
bool RasterWindow::event(QEvent *event)
{
	if (event->type() == QEvent::UpdateRequest) {
		renderNow();
		return true;
	}
	return QWindow::event(event);
}

// ����� exposeEvent ���������� ��� ������ ����
void RasterWindow::exposeEvent(QExposeEvent *)
{
	if (isExposed())
	{
		renderNow();
	}
}
	
// ����� resizeEvent ���������� ��� ��������� ������� ����
void RasterWindow::resizeEvent(QResizeEvent *resizeEvent)
{
	// �������� ������ ������ �����, ����� �� �������� � �������� ����
	QSize size = resizeEvent->size();
	m_backingStore->resize(size);
	m_scene->setBounds(QRectF(0, 0, size.width(), size.height()));
	if (isExposed())
	{
		renderNow();
    }
}

void RasterWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        Ball b = getRandomBall();
        b.setCenter(Vector2f(event->pos().x(), event->pos().y()));
        m_scene->tryAddBall(b);
    }
}

// ����� ��������� � ������� ������� Qt ������� ���������� ������ (UpdateRequest),
// ��� ����� �������� ����������� ����� � ��������� �������.
void RasterWindow::renderLater()
{
	requestUpdate();
}

// ���������� ���������� ���� ��������
void RasterWindow::updateScene()
{
	const float elapsedSeconds = float(m_updateTimer.elapsed()) / 1000.f;

	// ������� ���������� � ������, ���� ������ �� ����� ������ ��������� �����.
	if (elapsedSeconds > 0)
	{
		m_updateTimer.restart();
        float dt = elapsedSeconds / 10;
        for(int i=0; i<10; i++){
            m_scene->update(dt);
        }
	}
}

// ��������� �����
void RasterWindow::renderScene()
{
	QRect rect(0, 0, width(), height());
	m_backingStore->beginPaint(rect);

	QPaintDevice *device = m_backingStore->paintDevice();
	QPainter painter(device);

	painter.fillRect(0, 0, width(), height(), Qt::white);
	m_scene->redraw(painter);
	painter.end();

	m_backingStore->endPaint();
    m_backingStore->flush(rect);
}

void RasterWindow::initRandomGenerator()
{
    const unsigned seed = unsigned(std::time(nullptr));
    random_engine.seed(seed);
}

int RasterWindow::getRandomValue(int min, int max)
{
    std::uniform_int_distribution<size_t> distribution(min, max);
    return (int)distribution(random_engine);
}

QColor RasterWindow::getRandomColor()
{
    int r = getRandomValue(0,255);
    int g = getRandomValue(0,255);
    int b = getRandomValue(0,255);
    return QColor::fromRgb(r, g, b);
}

Ball RasterWindow::getRandomBall()
{
    float r = getRandomValue(BALL_MIN_RADIUS, BALL_MAX_RADIUS);
    float speed_x = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
    float speed_y = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
    float pos_x = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
    float pos_y = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
    return Ball(r, Vector2f(pos_x, pos_y), Vector2f(speed_x, speed_y), getRandomColor());
}

// ����� ��������� ���� �������� � ������ � ������� ���������
void RasterWindow::renderNow()
{
    if (!isExposed())
    {
        return;
    }

    updateScene();
    renderScene();

    if (m_isAnimating)
    {
        renderLater();
    }
}

