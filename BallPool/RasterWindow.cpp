#include "RasterWindow.h"

namespace 
{
	constexpr int WIN_WIDTH = 400;
	constexpr int WIN_HEIGHT = 400;

    constexpr int BALL_MIN_RADIUS = 10;
    constexpr int BALL_MAX_RADIUS = 50;

    constexpr int BALL_MIN_SPEED = 0;
    constexpr int BALL_MAX_SPEED = 500;

    constexpr int INIT_BALLS_CNT = 20;
}

// Конструктор класса: принимает один (опциональный) параметр типа QWindow,
// в этом параметре можно передать родительское окно (например, для модального диалога)
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
        float r = getRandomValue(BALL_MIN_RADIUS, BALL_MAX_RADIUS);
        float speed_x = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
        float speed_y = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
        float pos_x = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
        float pos_y = getRandomValue(BALL_MIN_SPEED, BALL_MAX_SPEED);
        if(m_scene->tryAddBall(Ball(r, Vector2f(pos_x, pos_y), Vector2f(speed_x, speed_y)))){
            n_balls++;
        }
    }

    /*m_scene->tryAddBall(Ball(20, Vector2f(50, 50), Vector2f(300, 0)));
    m_scene->tryAddBall(Ball(20, Vector2f(350, 52), Vector2f(-100, 0), Qt::red));
    m_scene->tryAddBall(Ball(20, Vector2f(100, 252), Vector2f(0, -200), Qt::blue));
    */
}

// Возвращает состояние анимации - включена или выключена
bool RasterWindow::isAnimating() const
{
	return m_isAnimating;
}

// Включает или выключает анимацию
void RasterWindow::setAnimating(bool isAnimating)
{
	m_isAnimating = isAnimating;
	if (isAnimating)
	{
		renderLater();
	}
}

// Метод event вызывается перед обработкой любых событий, включая resizeEvent и exposeEvent
bool RasterWindow::event(QEvent *event)
{
	if (event->type() == QEvent::UpdateRequest) {
		renderNow();
		return true;
	}
	return QWindow::event(event);
}

// Метод exposeEvent вызывается при показе окна
void RasterWindow::exposeEvent(QExposeEvent *)
{
	if (isExposed())
	{
		renderNow();
	}
}
	
// Метод resizeEvent вызывается при изменении размера окна
void RasterWindow::resizeEvent(QResizeEvent *resizeEvent)
{
	// Изменяем размер буфера кадра, чтобы он совпадал с размером окна
	QSize size = resizeEvent->size();
	m_backingStore->resize(size);
	m_scene->setBounds(QRectF(0, 0, size.width(), size.height()));
	if (isExposed())
	{
		renderNow();
	}
}

// Метод добавляет в очередь событий Qt событие обновления экрана (UpdateRequest),
// тем самым форсируя перерисовку кадра в ближайшем будущем.
void RasterWindow::renderLater()
{
	requestUpdate();
}

// Вычисление следующего шага анимации
void RasterWindow::updateScene()
{
	const float elapsedSeconds = float(m_updateTimer.elapsed()) / 1000.f;

	// Пропуск обновления в случае, если таймер не успел засечь прошедшее время.
	if (elapsedSeconds > 0)
	{
		m_updateTimer.restart();
        m_scene->update(elapsedSeconds);
	}
}

// Отрисовка сцены
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

// Метод обновляет кадр анимации и ставит в очередь следующий
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

