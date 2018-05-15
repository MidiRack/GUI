#include "MRKnob.hpp"

#include <QPainter>
#include <QImage>
#include <QtMath>

MRKnob::MRKnob(QWidget* parent) :
	QWidget(parent),
	maximum_(255),
	size_(0),
	target_(-1),
	value_(0),
	connected_(true),
	pressed_(false),
	name_()
{
	//Limit height, otherwise buttons can be huge
	setMaximumHeight(160);
}

MRKnob::~MRKnob()
{ }

int MRKnob::maximum() { return maximum_; }
void MRKnob::setMaximum(int maximum_)
{
	//Since value can not be higher than 255, the target is also not allowed to
	if (maximum_ > 255)
	{
		maximum_ = 255;
	}

	this->maximum_ = maximum_;
}

int MRKnob::target() { return target_; }
void MRKnob::setTarget(int target_, bool force)
{
	//Limit target
	if (target_ > maximum())
	{
		target_ = maximum();
	}
	else if (target_ < -1)
	{
		target_ = -1; //-1 means no target
	}

	//Only set target if forced
	if (force || target_ < 0)
	{
		//Set and repaint
		this->target_ = target_;
		repaint();

		//Tell the MainWindow the target has been changed
		emit targetChanged(this->target_);
	}
}

int MRKnob::value() { return value_; }
void MRKnob::setValue(int value_)
{
	//Value will be between 0 and 255, so convert it to something within the accepted range
	if (maximum() < 255)
	{
		value_ = round((double)maximum() / 255.0 * (double)value_);
	}

	//Limit the value
	if (value_ > maximum())
	{
		value_ = maximum();
	}
	else if (value_ < 0)
	{
		value_ = 0;
	}

	//Set and repaint
	this->value_ = value_;
	repaint();

	//Tell the MainWindow the value has been changed
	emit valueChanged(this->value_);
}

bool MRKnob::connected() { return connected_; }
void MRKnob::setConnected(bool connected_) { this->connected_ = connected_; }

bool MRKnob::isPressed() { return pressed_; }
void MRKnob::setPressed(bool pressed_) { this->pressed_ = pressed_; }

QString MRKnob::name() { return name_; }
void MRKnob::setName(QString name_) { this->name_ = name_; }

void MRKnob::paintEvent(QPaintEvent*)
{
	//Calculate the size of the square
	int width_ = width();
	int height_ = height();

	if (width_ < height_)
	{
		size_ = width_;
	}
	else
	{
		size_ = height_;
	}

	//Setup the painter, with AA and smooth transforms for best visual quality
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

	//Draw name if set
	if (!name().isEmpty())
	{
		painter.setFont(QFont("Monospace", 12));
		painter.setPen(QPen(QColor::fromRgb(255, 255, 255), 3));

		QRect nameRect(4, 4, width(), height());
		painter.drawText(nameRect, Qt::AlignHCenter, name());
	}

	//Draw the target arc around the knob
	QRect arc((double)width() / 2.0 - ((double)size_ - 36.0) / 2.0, 40, size_ - 36, size_ - 36);
	//Border
	painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 5));
	painter.drawArc(arc, 315 * 16, 270 * 16);

	//Background
	painter.setPen(QPen(QColor::fromRgb(25, 25, 25), 3));
	painter.drawArc(arc, 315 * 16, 270 * 16);

	if (target() >= 0)
	{
		//Draw actual target
		double valueDeg = (270.0 / (double)maximum() * (double)(target() - value())) * 16.0;
		double targetDeg = (-135.0 - 270.0 / (double)maximum() * (double)target()) * 16.0;

		//Use highlight color, can be changed for all widgets with `MainWindow::setHighlightColor(r, g, b)`
		QColor col = palette().color(QPalette::Highlight);

		//Do it 3 times with different line widths and different transparencies to create a glow effect
		col.setAlphaF(0.25);
		painter.setPen(QPen(col, 7));
		painter.drawArc(arc, targetDeg, valueDeg);

		col.setAlphaF(0.5);
		painter.setPen(QPen(col, 5));
		painter.drawArc(arc, targetDeg, valueDeg);

		col.setAlphaF(0.75);
		painter.setPen(QPen(col, 3));
		painter.drawArc(arc, targetDeg, valueDeg);
	}

	//Create rectangle for the knob
	QRect rectangle = arc.marginsRemoved(QMargins(8, 8, 8, 8));

	//Draw lines around
	for (int deg = 135; deg <= 405; deg += 15)
	{
		double a = deg * M_PI / 180.0;

		double x1 = rectangle.center().x() + ((size_ - 12) / 2) * cos(a) + 2;
		double y1 = rectangle.center().y() + ((size_ - 12) / 2) * sin(a) + 2;
		//Make it longer if it is on a 45 deg
		double x2 = rectangle.center().x() + ((size_ - (deg % 45 == 0 ? 0 : 8)) / 2) * cos(a) + 2;
		double y2 = rectangle.center().y() + ((size_ - (deg % 45 == 0 ? 0 : 8)) / 2) * sin(a) + 2;

		painter.setPen(QPen(QColor::fromRgb(250, 250, 250).darker(), 3));
		painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

		painter.setPen(QPen(QColor::fromRgb(250, 250, 250).darker(50), 1));
		painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
	}

	//Draw actual knob
	QString path = ":/images/knob.png";
	QImage knob(path);
	//Rotate it to the value
	painter.translate(rectangle.center().x() + 1, rectangle.center().y() + 1);
	painter.rotate(45.0 + 270.0 / (double)maximum() * (double)value());
	painter.translate(-rectangle.center().x() - 1, -rectangle.center().y() - 1);
	painter.drawImage(rectangle, knob);
}

//Also allow mousewheel to change the target/value
#include <QWheelEvent>
void MRKnob::wheelEvent(QWheelEvent* event)
{
	if (connected())
	{
		setTarget(target() + event->pixelDelta().y(), true);
	}
	else
	{
		setValue(value() + event->pixelDelta().y());
	}

	event->accept();
}

#include <QMouseEvent>
void MRKnob::mousePressEvent(QMouseEvent* event)
{
	emit pressed();
	setPressed(true);

	//Set target/value on press
	mousePos = event->pos();
	if (connected())
	{
		setTarget(pointToValue(mousePos), true);
	}
	else
	{
		setValue(pointToValue(mousePos));
	}

	event->accept();
}


void MRKnob::mouseReleaseEvent(QMouseEvent* event)
{
	mousePos = QPoint(-1, -1);

	emit released();
	setPressed(false);

	event->accept();
}

void MRKnob::mouseMoveEvent(QMouseEvent* event)
{
	if (mousePos.x() > 0 && mousePos.y() > 0)
	{
		//Update target/value when mouse is moved
		if (connected())
		{
			setTarget(pointToValue(event->pos()), true);
		}
		else
		{
			setValue(pointToValue(event->pos()));
		}
	}

	event->accept();
}

void MRKnob::enterEvent(QEvent* event)
{
	setCursor(QCursor(Qt::PointingHandCursor));

	event->accept();
}

void MRKnob::leaveEvent(QEvent* event)
{
	setCursor(QCursor(Qt::ArrowCursor));

	event->accept();
}

int MRKnob::pointToValue(QPoint point)
{
	QRect rect((double)width() / 2.0 - ((double)size_ - 36.0) / 2.0, 48, size_ - 36, size_ - 36);

	//Calculate degrees based on position and rectangle using atan2
	double val = (atan2(rect.center().x() - point.x(), rect.center().y() - point.y()) * M_PI / 180.0) * 1024.0;
	if (val >= 42)
	{
		return 0;
	}
	else if (val <= -42)
	{
		return (connected()) ? maximum() : 255;
	}

	//Set degrees to 'start'
	val += 42.0;
	val = 84.0 - val;

	int res = val;
	if (connected())
	{
		res = ((double)maximum() / 84.0 * val);
	}
	else
	{
		res = (255.0 / 84.0 * val);
	}

	return res;
}
