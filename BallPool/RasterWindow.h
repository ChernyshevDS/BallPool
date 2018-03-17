#pragma once

#include <memory>
#include <ctime>
#include <random>

#include <QtGui/QWindow>
#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>
#include <QtGui/QExposeEvent>
#include <QtGui/QBackingStore>
#include <QtCore/QElapsedTimer>

#include "PoolTableScene.h"

// Класс RasterWindow наследует все поля и методы класса QWindow
class RasterWindow : public QWindow
{
	// Макрос Q_OBJECT является меткой для Qt moc - генератора кода в составе Qt SDK
	Q_OBJECT

public: // Начало секции публично доступных полей и методов
	explicit RasterWindow(QWindow *parent = 0);

	bool isAnimating() const;
	void setAnimating(bool isAnimating);

protected: // Начало секции полей и методов, доступных только в наследниках этого класса
	bool event(QEvent *event) override;
	void exposeEvent(QExposeEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private: // Начало секции полей и методов, доступных только в наследниках этого класса
	void renderNow();
	void renderLater();
	void updateScene(); // обновляет состояние сцены
	void renderScene(); // перерисовывает содержимое сцены
    void initRandomGenerator();
    int getRandomValue(int min, int max);
    QColor getRandomColor();
    Ball getRandomBall();

	// Класс QBackingStore предоставляет окну буфер рисования кадра.
	QBackingStore *m_backingStore = nullptr;
	std::unique_ptr<PoolTableScene> m_scene; // объект сцены
	QElapsedTimer m_updateTimer; // таймер обновления сцены
	bool m_isAnimating = false;
    std::mt19937 random_engine;
};
