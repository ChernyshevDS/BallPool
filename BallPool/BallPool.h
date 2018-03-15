#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BallPool.h"

class BallPool : public QMainWindow
{
	Q_OBJECT

public:
	BallPool(QWidget *parent = Q_NULLPTR);

private:
	Ui::BallPoolClass ui;
};
