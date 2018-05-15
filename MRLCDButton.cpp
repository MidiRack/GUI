#include "MRLCDButton.hpp"

#include <QPainter>

MRLCDButton::MRLCDButton(QWidget* parent) :
	MRLCD(parent),
	clickable_(true),
	editable_(false),
	cursorPos(-1)
{
	setFocusPolicy(Qt::StrongFocus);
}

MRLCDButton::~MRLCDButton()
{ }

bool MRLCDButton::clickable() { return clickable_; }
void MRLCDButton::setClickable(bool clickable_) { this->clickable_ = clickable_; }

bool MRLCDButton::editable() { return editable_; }
void MRLCDButton::setEditable(bool editable_) { this->editable_ = editable_; }

#include <QPaintEvent>
void MRLCDButton::paintEvent(QPaintEvent* event)
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

	if (!editable() || cursorPos < 0)
	{
		//Draw text
		painter.drawText(textBoxText, text());
	}
	else
	{
		QString front = text().left(cursorPos);
		QString back = text().right(text().length() - cursorPos);

		painter.drawText(textBoxText, front);
		painter.drawText(textBoxText, Qt::AlignRight, back);

		int x, y, w, h;
		textBoxText.getRect(&x, &y, &w, &h);
		painter.drawLine(painter.fontMetrics().width(front) + 16, y + 4, painter.fontMetrics().width(front) + 16, y + h - 4);
	}

	event->accept();
}

#include <QMouseEvent>
void MRLCDButton::mousePressEvent(QMouseEvent* event)
{
	MRLCD::mousePressEvent(event);

	if (clickable())
	{
		emit pressed(event->button());
	}

	if (editable())
	{
		cursorPos = text().length();
	}

	event->accept();
}

void MRLCDButton::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (clickable() && event->button() == Qt::LeftButton)
	{
		emit doubleClicked();
	}

	event->accept();
}

void MRLCDButton::enterEvent(QEvent* event)
{
	if (clickable())
	{
		setCursor(QCursor(Qt::PointingHandCursor));
	}
	if (editable())
	{
		setCursor(QCursor(Qt::IBeamCursor));
	}

	event->accept();
}

void MRLCDButton::leaveEvent(QEvent* event)
{
	setCursor(QCursor(Qt::ArrowCursor));

	event->accept();
}

#include <QKeyEvent>
void MRLCDButton::keyPressEvent(QKeyEvent* event)
{
	if (editable() && cursorPos >= 0)
	{
		QRegExp re("[^a-zA-Z0-9\ \-\_]");
		if (re.indexIn(event->text()) < 0 && !event->text().isEmpty())
		{
			QString front = text().left(cursorPos);
			QString back = text().right(text().length() - cursorPos);

			front += event->text().toLower();
			cursorPos += 1;

			setText(front + back);
		}
		else
		{
			if (event->key() == Qt::Key_Left)
			{
				cursorPos -= 1;
				if (cursorPos < 0)
				{
					cursorPos = 0;
				}
			}
			else if (event->key() == Qt::Key_Right)
			{
				cursorPos += 1;
				if (cursorPos > text().length())
				{
					cursorPos = text().length();
				}
			}
			else if (event->key() == Qt::Key_Backspace)
			{
				QString front = text().left(cursorPos);
				QString back = text().right(text().length() - cursorPos);

				cursorPos -= 1;
				if (cursorPos < 0)
				{
					cursorPos = 0;
				}

				setText(front.left(front.length() - 1) + back);
			}
			else if (event->key() == Qt::Key_Delete)
			{
				if (cursorPos < text().length())
				{
					QString front = text().left(cursorPos);
					QString back = text().right(text().length() - cursorPos);

					setText(front + back.right(back.length() - 1));
				}
			}
			else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
			{
				cursorPos = -1;
				emit(finished());
				return;
			}

			repaint();
		}
	}

	event->accept();
}
