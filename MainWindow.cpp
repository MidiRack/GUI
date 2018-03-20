#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QDebug>

#include "Potmeter.hpp"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

	for (int i = 0; i < ports.length(); i++)
	{
		if (ports[i].description().contains("Arduino") || ports[i].description().contains("Genuino") || ports[i].description().contains("0043"))
		{
			qDebug() << ports[i].description();

			port.setPort(ports[i]);
			port.setBaudRate(QSerialPort::Baud9600);
			break;
		}
	}

	port.open(QIODevice::ReadWrite);

	connect(&port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));

	potmeters[0] = new Potmeter(ui->progressBar, ui->horizontalSlider);
	connect(ui->horizontalSlider, &QSlider::sliderReleased, [this]() { setData(0); });

	potmeters[1] = new Potmeter(ui->progressBar_2, ui->horizontalSlider_2);
	connect(ui->horizontalSlider_2, &QSlider::sliderReleased, [this]() { setData(1); });
}

MainWindow::~MainWindow()
{
	port.close();

	delete ui;
}

void MainWindow::dataAvailable()
{
	data.append(port.readAll());
	if (data.contains("\r\n"))
	{
		QByteArray lineData = data.split('\n')[0];
		QString line = QString(lineData);
		qDebug() << line;

		int index = QString(line.split(":").first()).toInt();
		int value = QString(line.split(":").last()).toInt();
		potmeters[index]->bar->setValue(value);

		data = data.mid(data.indexOf('\n') + 1);
	}
}

void MainWindow::setData(int index)
{
	QString data = QString::number(index) + ":" + QString::number(potmeters[index]->slider->value()) + "\r\n";
	port.write(data.toStdString().c_str());
}
