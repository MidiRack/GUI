#ifndef MRKNOB_HPP
#define MRKNOB_HPP

#include <QWidget>

class MRKnob : public QWidget
{
	Q_OBJECT

public:
	explicit MRKnob(QWidget* parent = nullptr);
	~MRKnob();

	//Maximum value
	int maximum();
	void setMaximum(int maximum_);

	//Knob target
	int target();
	void setTarget(int target_, bool force = false);

	//Current value of knob
	int value();
	void setValue(int value_);

	//True if connected to MidiRack
	bool connected();
	void setConnected(bool connected_);

	//If mouse down
	bool isPressed();
	void setPressed(bool pressed_);

	//Name shown above knob
	QString name();
	void setName(QString name_);

private:
	void paintEvent(QPaintEvent*);
	void wheelEvent(QWheelEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);

	//Calculate mouse position to degrees based on center
	int pointToValue(QPoint);

	int maximum_, size_, target_, value_;
	bool connected_, pressed_;
	QString name_;

	//Required to calculate target
	QPoint mousePos;

signals:
	//Emit when value/target updated
	void valueChanged(int value);
	void targetChanged(int target);

	//Emit when mouse down/up
	void pressed();
	void released();
};

#endif // MRKNOB_HPP
