#ifndef POTMETER_HPP
#define POTMETER_HPP

#include <QProgressBar>
#include <QSlider>

class Potmeter
{
public:
	QProgressBar* bar;
	QSlider* slider;

	Potmeter(QProgressBar* bar, QSlider* slider) :
		bar(bar),
		slider(slider)
	{ }
};

#endif // POTMETER_HPP
