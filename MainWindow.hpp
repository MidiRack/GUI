#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtSerialPort>

namespace Ui {
	class MainWindow;
}

class Potmeter;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	QSerialPort port;
	QByteArray data;

	Potmeter* potmeters[2];

public slots:
	void dataAvailable();
	void setData(int index);
};

#endif // MAINWINDOW_HPP
