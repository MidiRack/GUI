#ifndef POTMETER_HPP
#define POTMETER_HPP

#include <QWidget>

#include "MRKnob.hpp"
#include "MRLCD.hpp"

namespace Ui {
	class Potmeter;
}

class Potmeter : public QWidget
{
	Q_OBJECT

public:
	explicit Potmeter(QWidget* parent = nullptr);
	~Potmeter();

private:
	Ui::Potmeter *ui;

	MRKnob* knob;
	MRLCD* value;
	MRLCD* target;
};

#endif // POTMETER_HPP
