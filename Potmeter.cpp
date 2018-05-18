#include "Potmeter.hpp"
#include "ui_Potmeter.h"

Potmeter::Potmeter(MRKnob* knob, MRLCD* value, MRLCD* target, QWidget* parent) :
	QWidget(parent),
	ui(new Ui::Potmeter),
	knob(knob),
	value(value),
	target(target)
{
	ui->setupUi(this);

	ui->verticalLayout->replaceWidget(ui->knobHolder, knob);
	ui->horizontalLayout->replaceWidget(ui->targetLCDHolder, target);
	ui->horizontalLayout->replaceWidget(ui->valueLCDHolder, value);
}

Potmeter::~Potmeter()
{
	delete ui;
}

void Potmeter::doUpdate()
{
	if (knob->target() == -1)
	{
		target->setText("--");
	}
	else
	{
		target->setText("");
	}

	value->setValue(knob->value());
	target->setValue(knob->target());
}
