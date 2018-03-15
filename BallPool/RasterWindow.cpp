#include "RasterWindow.h"

namespace 
{
	constexpr int WIN_WIDTH = 400;
	constexpr int WIN_HEIGHT = 400;
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
		m_scene->update(elapsedSeconds);
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

