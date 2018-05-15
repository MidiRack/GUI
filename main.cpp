#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//Load stylesheet for window styling
	QFile styleFile(":/default.qss");
	styleFile.open(QFile::ReadOnly);
	QString style(styleFile.readAll());
	a.setStyleSheet(style);

	MainWindow w;
	w.show();

	return a.exec();
}
