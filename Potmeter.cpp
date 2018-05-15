#include "Potmeter.hpp"
#include "ui_Potmeter.h"

Potmeter::Potmeter(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Potmeter)
{
	ui->setupUi(this);
}

Potmeter::~Potmeter()
{
	delete ui;
}
