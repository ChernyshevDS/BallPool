#include "BallPool.h"
#include <QtWidgets/QApplication>
#include "RasterWindow.h"

int main(int argc, char *argv[])
{
	// ќбъ€вл€ем переменную app типа QGuiApplication.
	// ќбъ€вить еЄ надо как можно раньше, так как при инициализации Q*Application
	//  не€вно инициализируютс€ различные подсистемы Qt
	QGuiApplication app(argc, argv);

	// ќбъ€вл€ем переменную window типа QWindow.
	// Ёта переменна€ представл€ет окно операционной системы.
	RasterWindow window;

	window.setAnimating(true);

	// »нициируем показ окна: показ случитс€ позже, когда начнЄтс€ цикл событий
	window.show();

	// «апускаем цикл событий: метод exec выполн€етс€ вплоть до завершени€
	//  работы приложени€, и возвращает в main код возврата (ненулевой в случае ошибки).
	return app.exec();
}
