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

// ����� RasterWindow ��������� ��� ���� � ������ ������ QWindow
class RasterWindow : public QWindow
{
	// ������ Q_OBJECT �������� ������ ��� Qt moc - ���������� ���� � ������� Qt SDK
	Q_OBJECT

public: // ������ ������ �������� ��������� ����� � �������
	explicit RasterWindow(QWindow *parent = 0);

	bool isAnimating() const;
	void setAnimating(bool isAnimating);

protected: // ������ ������ ����� � �������, ��������� ������ � ����������� ����� ������
	bool event(QEvent *event) override;
	void exposeEvent(QExposeEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private: // ������ ������ ����� � �������, ��������� ������ � ����������� ����� ������
	void renderNow();
	void renderLater();
	void updateScene(); // ��������� ��������� �����
	void renderScene(); // �������������� ���������� �����
    void initRandomGenerator();
    int getRandomValue(int min, int max);
    QColor getRandomColor();
    Ball getRandomBall();

	// ����� QBackingStore ������������� ���� ����� ��������� �����.
	QBackingStore *m_backingStore = nullptr;
	std::unique_ptr<PoolTableScene> m_scene; // ������ �����
	QElapsedTimer m_updateTimer; // ������ ���������� �����
	bool m_isAnimating = false;
    std::mt19937 random_engine;
};
