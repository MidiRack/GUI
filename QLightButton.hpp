#ifndef QLIGHTBUTTON_HPP
#define QLIGHTBUTTON_HPP

#include <QWidget>
/*
class QLightButton : public QWidget
{
	Q_OBJECT

public:
	explicit QLightButton(QWidget* parent = nullptr);
	~QLightButton();

	bool pressed();
	void setPressed(bool pressed_);

	QString buttonText();
	void setButtonText(QString text_);

private:
	void paintEvent(QPaintEvent*);
	void wheelEvent(QWheelEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);

	int pointToValue(QPoint);

	int value_, target_, maximum_, size_;
	QString name_;
	QPoint mousePos;

signals:
	void valueChanged(int value);
	void targetChanged(int target);

	void pressed();
	void released();
};*/

#endif // QLIGHTBUTTON_HPP
