#include "MRPresetListC.hpp"

#include <QLineEdit>
#include <QPainter>
#include <QtMath>

#include "MRLCDButton.hpp"

MRPresetList::MRPresetList(QWidget* parent) :
	QWidget(parent),
	page_(0),
	selected_(-1),
	c(0),
	h(0),
	y(0)
{ }

MRPresetList::~MRPresetList()
{ }

int MRPresetList::page() { return page_; }
void MRPresetList::setPage(int page_)
{
	//Do not allow negative pages
	if (page_ < 0)
	{
		page_ = 0;
	}

	//Limit page to amount of pages
	if (page_ > floor((double)items().length() / (double)c))
	{
		page_ = floor((double)items().length() / (double)c);
	}
}

int MRPresetList::selectedIndex() { return selected_; }
void MRPresetList::setSelectedIndex(int index_)
{
	if (index_ < 0 || index_ > items().length())
	{
		selected_ = -1; //-1 means no selected item
	}
	else
	{
		selected_ = index_;
	}
}

QString MRPresetList::selected()
{
	if (selectedIndex() < 0 || selectedIndex() > items().length())
	{
		//Return nothing if the selected index is out of bounds
		return "";
	}
	else
	{
		//Or return the one at selected index
		return items().at(selectedIndex());
	}
}
void MRPresetList::setSelected(QString selected_)
{
	//Search list for string and set index to found position or -1 if not found
	setSelectedIndex(items().indexOf(selected_));
}

QStringList MRPresetList::items() { return items_; }
void MRPresetList::setItems(QStringList items_)
{
	//Set items and sort them
	this->items_ = items_;
	sort();
}
void MRPresetList::addItem(QString item_)
{
	//Add item, sort the list and selected the added item
	this->items_.push_back(item_);
	sort();
	setSelected(item_);
}

void MRPresetList::showPresets()
{

}

void MRPresetList::sort()
{
	items_.sort(Qt::CaseInsensitive);
}

#include <QShowEvent>
void MRPresetList::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

	layout = new QVBoxLayout(this);
	setLayout(layout);

	QHBoxLayout* nav = new QHBoxLayout(this);
	prevPage = new MRLCDButton(this);
	prevPage->setMinimumWidth(64);
	prevPage->setText("<<");
	prevPage->setLCDEnabled(false);
	nextPage = new MRLCDButton(this);
	nextPage->setMinimumWidth(64);
	nextPage->setText(">>");
	nextPage->setLCDEnabled(false);
	nav->addWidget(prevPage, 0, Qt::AlignLeft);
	nav->addWidget(nextPage, 0, Qt::AlignRight);
	layout->addLayout(nav, 1);

	QHBoxLayout* sl = new QHBoxLayout(this);
	saveButton = new MRLCDButton(this);
	saveButton->setMinimumWidth(64);
	saveButton->setText("Save");
	loadButton = new MRLCDButton(this);
	loadButton->setMinimumWidth(64);
	loadButton->setText("Load");
	loadButton->setLCDEnabled(false);
	sl->addWidget(saveButton, 0, Qt::AlignLeft);
	sl->addWidget(loadButton, 0, Qt::AlignRight);
	layout->addLayout(sl);

	event->accept();
}

void MRPresetList::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	painter.setFont(QFont("Monospace", 12));
	painter.setPen(QPen(QColor::fromRgb(255, 255 ,255), 3));

	QRect nameRect(0, 4, width(), height());
	painter.drawText(nameRect, Qt::AlignHCenter, "Presets");

	QWidget::paintEvent(e);
}
	/*y = fm.height() + 16;
	h = fm.height() + 4;
	int c = (height() - 32) / (h + 8) - 1;

	if (offset > 0 && items().length() <= (c - 1))
	{
		offset = 0;
	}

	if (selectedIndex() > 0)
	{
		offset = selectedIndex() / (c - 1);
	}

	QRect prevBox(8, y, (width() - 32) / 2, h);
	painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 3));
	painter.drawRect(prevBox);
	painter.fillRect(prevBox, QColor::fromRgb(25, 25, 25));

	QRect nextBox((width() - 32) / 2 + 24, y, (width() - 32) / 2, h);
	painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 3));
	painter.drawRect(nextBox);
	painter.fillRect(nextBox, QColor::fromRgb(25, 25, 25));

	painter.setFont(QFont("DS-Digital", 16));

	if (offset > 0 && options == "")
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 3));
	}
	else
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(200), 3));
	}
	QRect prevBoxText = prevBox.marginsRemoved(QMargins(4, 0, 4, 0));
	painter.drawText(prevBoxText, Qt::AlignCenter, "<<");

	if (c * (offset + 1) <= items().length() && options == "")
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 3));
	}
	else
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(200), 3));
	}
	QRect nextBoxText = nextBox.marginsRemoved(QMargins(4, 0, 4, 0));
	painter.drawText(nextBoxText, Qt::AlignCenter, ">>");


	y = fm.height() + h + 24;

	if (options == "")
	{
		for (int i = offset * (c - 1); i < items().length(); i++)
		{
			if (i >= (offset + 1) * (c - 1))
			{
				break;
			}

			QRect textBox(8, y, width() - 16, h);
			painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 3));
			painter.drawRect(textBox);
			painter.fillRect(textBox, QColor::fromRgb(25, 25, 25));

			if (selectedIndex() == i)
			{
				painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 3));
			}
			else
			{
				painter.setPen(QPen(palette().color(QPalette::Highlight).darker(200), 3));
			}

			QRect textBoxText = textBox.marginsRemoved(QMargins(4, 0, 4, 0));
			painter.setFont(QFont("DS-Digital", 16));
			painter.drawText(textBoxText, items().at(i));

			y += h + 8;
		}
	}
	else
	{
		QRect textBox(8, y, width() - 16, h);
		painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 3));
		painter.drawRect(textBox);
		painter.fillRect(textBox, QColor::fromRgb(25, 25, 25));

		QRect textBoxText = textBox.marginsRemoved(QMargins(4, 0, 4, 0));
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 3));
		painter.setFont(QFont("DS-Digital", 16));

		if (temp == "")
		{
			painter.drawText(textBoxText, items().at(selectedIndex()));
		}
		else
		{
			painter.drawText(textBoxText, temp);
			if (temp != "")
			{
				QFontMetrics fm2 = painter.fontMetrics();
				painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 1));
				painter.drawLine(fm2.width(temp) + 16, y + 4, fm2.width(temp) + 16, y + h - 4);
			}
		}

		y += h + 8;
	}

	y = fm.height() + h + 24 + (c - 1) * (h + 8);

	QRect saveBox(8, y, (width() - 32) / 2, h);
	painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 3));
	painter.drawRect(saveBox);
	painter.fillRect(saveBox, QColor::fromRgb(25, 25, 25));

	QRect loadBox((width() - 32) / 2 + 24, y, (width() - 32) / 2, h);
	painter.setPen(QPen(QColor::fromRgb(5, 5, 5), 3));
	painter.drawRect(loadBox);
	painter.fillRect(loadBox, QColor::fromRgb(25, 25, 25));

	painter.setFont(QFont("DS-Digital", 16));

	painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 3));
	QRect saveBoxText = saveBox.marginsRemoved(QMargins(4, 0, 4, 0));
	painter.drawText(saveBoxText, "Save");

	if (selectedIndex() > -1 && options == "")
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(100), 3));
	}
	else
	{
		painter.setPen(QPen(palette().color(QPalette::Highlight).darker(200), 3));
	}
	QRect loadBoxText = loadBox.marginsRemoved(QMargins(4, 0, 4, 0));
	painter.drawText(loadBoxText, "Load");
}*/

#include <QMouseEvent>
void MRPresetList::mousePressEvent(QMouseEvent* event)
{
	/*int cur = (event->pos().y() - 32) / (h + 8);
	int c = (height() - 32) / (h + 8) - 1;

	if (cur == 0 && options == "")
	{
		if (event->pos().x() >= 8 && event->pos().x() <= 8 + (width() - 32) / 2)
		{
			if (offset > 0)
			{
				offset--;
			}
		}
		else if (event->pos().x() >= (width() - 32) / 2 + 24 && event->pos().x() <= (width() - 32) / 2 + 24 + (width() - 32) / 2)
		{
			if (c * (offset + 1) <= items().length())
			{
				offset++;
			}
		}

		setSelectedIndex(-1);
	}
	else if (cur == (y - 32) / (h + 8))
	{
		if (event->pos().x() >= 8 && event->pos().x() <= 8 + (width() - 32) / 2)
		{
			if (options == "")
			{
				emit save();
			}
		}
		else if (event->pos().x() >= (width() - 32) / 2 + 24 && event->pos().x() <= (width() - 32) / 2 + 24 + (width() - 32) / 2 && options == "")
		{
			emit load(selected());
		}
	}
	else if (cur > (y - 32) / (h + 8))
	{
		//
	}
	else
	{
		if (options == "")
		{
			setSelectedIndex((c - 1) * offset + cur - 1);
		}
		else
		{
			if (cur == 1)
			{
				temp = items().at(selectedIndex());
			}
		}
	}

	repaint();
	event->accept();*/
}

void MRPresetList::mouseDoubleClickEvent(QMouseEvent* event)
{
	/*int cur = (event->pos().y() - 32) / (h + 8);
	int c = (height() - 32) / (h + 8) - 1;

	if (cur == 0 || cur == (y - 32) / (h + 8) || cur > (y - 32) / (h + 8))
	{
		//
	}
	else
	{
		if (options == "")
		{
			setSelectedIndex((c - 1) * offset + cur - 1);
			if (event->button() == Qt::LeftButton)
			{
				emit load(selected());
			}
			else if (event->button() == Qt::RightButton)
			{
				options = selected();
			}
		}
	}

	repaint();
	event->accept();*/
}
