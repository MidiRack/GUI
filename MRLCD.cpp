#include "MRLCD.hpp"

#include <QPainter>

MRLCD::MRLCD(QWidget* parent) :
	QWidget(parent),
	value_(0),
	max_(0),
	min_(0),
	lcdEnabled_(true),
	name_(""),
	text_(""),
	unit_("")
{
	setMinimumWidth(160);
	setFixedHeight(32);
}

MRLCD::~MRLCD()
{ }

int MRLCD::value() { return value_; }
void MRLCD::setValue(int value_)
{
	//Do not allow negative values
	if (value_ < 0)
	{
		value_ = 0;
	}

	//Set and repaint
	this->value_ = value_;
	repaint();

	//Tell MainWindow the value has been updated
	emit valueChanged(this->value_);
}

double MRLCD::max() { return max_; }
double MRLCD::min() { return min_; }
void MRLCD::setMinMax(double min_, double max_) { this->min_ = min_; this->max_ = max_; }

bool MRLCD::LCDEnabled() { return lcdEnabled_; }
void MRLCD::setLCDEnabled(bool lcdEnabled_)
{
	//Set and repaint
	this->lcdEnabled_ = lcdEnabled_;
	repaint();
}

QString MRLCD::name() { return name_; }
void MRLCD::setName(QString name_)
{
	this->name_ = name_;

	//Increase height
	setFixedHeight(64);
}

QString MRLCD::text() { return text_; }
void MRLCD::setText(QString text_) { this->text_ = text_; emit(textChanged(text())); repaint(); }

QString MRLCD::unit() { return unit_; }
void MRLCD::setUnit(QString unit_) { this->unit_ = unit_; repaint(); }

QStringList MRLCD::items() { return items_; }
void MRLCD::setItems(QStringList items_)
{
	this->items_ = items_;

	//Set limits to list size
	setMinMax(0, items_.length());
}

void MRLCD::paintEvent(QPaintEvent*)
{
	//Create antialiased painter
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetrics fm = painter.fontMetrics();

	//Create box based on font height
	QRect textBox(8, (name().isEmpty() ? 4 : fm.height() + 16), width() - 16, fm.height() + 4);
	painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 3));
	painter.drawRect(textBox); //Border
	painter.fillRect(textBox, QColor::fromRgb(25, 25, 25)); //Background

	//Draw name if set
	if (!name().isEmpty())
	{
		painter.setFont(QFont("Monospace", 12));
		painter.setPen(QPen(QColor::fromRgb(255, 255 ,255), 3));

		QRect nameRect(0, 4, width(), height());
		painter.drawText(nameRect, Qt::AlignHCenter, name());
	}

	painter.setFont(QFont("DS-Digital", 16));
	if (LCDEnabled())
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 3));
	}
	else
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(200), 3));
	}

	QRect textBoxText = textBox.marginsRemoved(QMargins(4, 0, 4, 0));

	//Draw unit on the right if set
	if (!unit().isEmpty())
	{
		painter.drawText(textBoxText, Qt::AlignRight, unit());
	}

	//Draw value or text
	if (text().isEmpty())
	{
		if (items().isEmpty())
		{
			//Calculate value to be between min and max
			double val = value();
			if (min() != 0 && max() != 0)
			{
				if (min() < 0)
				{
					val = (max() - min()) / 255.0 * (double)value() + min();
				}
				else
				{
					val = max() / 255.0 * (double)value() + min();
				}
			}

			painter.drawText(textBoxText, QString::number(val));
		}
		else
		{
			painter.drawText(textBoxText, items().at(value()));
		}
	}
	else
	{
		painter.drawText(textBoxText, text());
	}
}
