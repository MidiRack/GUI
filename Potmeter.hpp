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
	explicit Potmeter(MRKnob* knob, MRLCD* value, MRLCD* target, QWidget* parent = nullptr);
	~Potmeter();

	void doUpdate();

	MRKnob* knob;
	MRLCD* value;
	MRLCD* target;

private:
	Ui::Potmeter *ui;
};

#endif // POTMETER_HPP
