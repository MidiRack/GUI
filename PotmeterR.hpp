#ifndef POTMETER_HPP
#define POTMETER_HPP

#include "MRKnob.hpp"
#include "MRLCD.hpp"

class Potmeter
{
public:
	//Pointers to custom widgets
	MRKnob* knob;
	MRLCD* value;
	MRLCD* target;

	Potmeter(MRKnob* knob, MRLCD* value, MRLCD* target) :
		knob(knob),
		value(value),
		target(target)
	{ }

	//Update LCDs
	void update()
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
};

#endif // POTMETER_HPP
