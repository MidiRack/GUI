#include "MRPresetList.hpp"
#include "ui_MRPresetList.h"

#include <QPainter>
#include <QtMath>

#include "MRLCDButton.hpp"

MRPresetList::MRPresetList(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::MRPresetList),
	options_(-1),
	page_(0),
	selected_(-1),
	count(0),
	newName("")
{
	ui->setupUi(this);

	ui->verticalLayout_2->setAlignment(Qt::AlignTop);

	//Add controls
	prevPage = new MRLCDButton(this);
	prevPage->setMinimumWidth(64);
	prevPage->setText("<<");
	prevPage->setLCDEnabled(false);
	ui->horizontalLayout->replaceWidget(ui->prevButtonHolder, prevPage);

	nextPage = new MRLCDButton(this);
	nextPage->setMinimumWidth(64);
	nextPage->setText(">>");
	nextPage->setLCDEnabled(false);
	ui->horizontalLayout->replaceWidget(ui->nextButtonHolder, nextPage);

	saveButton = new MRLCDButton(this);
	saveButton->setMinimumWidth(64);
	saveButton->setText("Save");
	ui->horizontalLayout_2->replaceWidget(ui->saveButtonHolder, saveButton);

	loadButton = new MRLCDButton(this);
	loadButton->setMinimumWidth(64);
	loadButton->setText("Load");
	loadButton->setLCDEnabled(false);
	ui->horizontalLayout_2->replaceWidget(ui->loadButtonHolder, loadButton);

	connect(prevPage, &MRLCDButton::pressed, [this]()
	{
		if (options() < 0)
		{
			setPage(page() - 1);
		}
		else
		{
			setOptions(options() - 1);
		}
	});

	connect(nextPage, &MRLCDButton::pressed, [this]()
	{
		if (options() < 0)
		{
			setPage(page() + 1);
		}
		else
		{
			setOptions(options() + 1);
		}
	});

	connect(saveButton, &MRLCDButton::pressed, [this]()
	{
		if (options() < 0)
		{
			emit(save());
		}
		else
		{
			loadButton->setText("Load");
			QString oldName = items().at(options());
			items().clear();
			emit(renamePreset(oldName, newName));
			setOptions(-1);
			showPresets(true);
			setSelected(newName);
			newName = "";
			showPresets(true);
			updateControls();
		}
	});

	connect(loadButton, &MRLCDButton::pressed, [this]()
	{
		if (options() < 0)
		{
			emit(load(selected()));
		}
		else
		{
			loadButton->setText("Load");
			QString name = items().at(options());
			newName = "";
			setOptions(-1);
			showPresets(true);
			setSelected(name);
			showPresets(true); //Required to show the right page
			updateControls();
		}
	});
}

MRPresetList::~MRPresetList()
{
	delete ui;
}

int MRPresetList::options() { return options_; }
void MRPresetList::setOptions(int options_)
{
	//-1 means no options shown
	if (options_ < 0)
	{
		options_ = -1;
	}

	//Limit to amount of items
	if (options_ >= items().length())
	{
		options_ = items().length() - 1;
	}

	this->options_ = options_;

	if (this->options() >= 0)
	{
		showOptions();
	}
}

int MRPresetList::page() { return page_; }
void MRPresetList::setPage(int page_, bool reset)
{
	if (reset)
	{
		//Reset selection
		setSelectedIndex(-1);
	}

	//Do not allow negative pages
	if (page_ < 0)
	{
		page_ = 0;
	}

	//Limit page to amount of pages
	if (page_ > floor((double)items().length() / (double)count))
	{
		page_ = floor((double)items().length() / (double)count);
	}

	this->page_ = page_;

	if (reset)
	{
		showPresets(true);
	}
}

int MRPresetList::selectedIndex() { return selected_; }
void MRPresetList::setSelectedIndex(int index_)
{
	//Disable previously selected preset
	if (selected_ >= 0)
	{
		presets.at(selected_ - (page() * count))->setLCDEnabled(false);
	}

	if (index_ < 0 || index_ > items().length())
	{
		selected_ = -1; //-1 means no selected item
	}
	else
	{
		selected_ = index_;
		setPage(floor((double)selected_ / (double)count), false);
		presets.at(selected_ - (page() * count))->setLCDEnabled(true);
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
	showPresets();
}
void MRPresetList::addItem(QString item_)
{
	//Add item, sort the list and selected the added item
	this->items_.push_back(item_);
	sort();
	setSelected(item_);
	showPresets();
}

void MRPresetList::updateControls()
{
	if (options() < 0)
	{
		//Check for next page
		nextPage->setClickable(items().length() > count * (page() + 1));
		nextPage->setLCDEnabled(items().length() > count * (page() + 1));

		//Check for previous page
		prevPage->setClickable(page() > 0);
		prevPage->setLCDEnabled(page() > 0);

		//Enable if a preset is selected
		loadButton->setClickable(selectedIndex() >= 0);
		loadButton->setLCDEnabled(selectedIndex() >= 0);

		saveButton->setClickable(true);
		saveButton->setLCDEnabled(true);

		if (page() > 0 && page() * count >= items().length())
		{
			setPage(page() - 1, false);
		}
	}
	else
	{
		//Check for next item
		nextPage->setClickable(options() < items().length());
		nextPage->setLCDEnabled(options() < items().length());

		//Check for previous item
		prevPage->setClickable(options() > 0);
		prevPage->setLCDEnabled(options() > 0);

		//Always enable exit
		loadButton->setText("Exit");
		loadButton->setClickable(true);
		loadButton->setLCDEnabled(true);

		//Enable if name is different and not ""
		saveButton->setClickable(newName != items().at(options()) && !newName.isEmpty());
		saveButton->setLCDEnabled(newName != items().at(options()) && !newName.isEmpty());
	}

	repaint();
}

void MRPresetList::showPresets(bool redo)
{
	updateControls();

	if (redo || presets.length() == 0)
	{
		//Remove all presets
		for (int i = 0; i < presets.length(); i++)
		{
			ui->verticalLayout_2->removeWidget(presets.at(i));
			delete presets.at(i);
		}
		presets.clear();

		//Add presets again
		for (int i = page() * count; i < items().length(); i++)
		{
			if (i >= (page() + 1) * count)
			{
				break;
			}

			MRLCDButton* preset = new MRLCDButton(ui->presetHolder);
			preset->setText(items().at(i));
			preset->setLCDEnabled(i == selectedIndex());
			ui->verticalLayout_2->addWidget(preset);
			presets.push_back(preset);

			connect(preset, &MRLCDButton::pressed, [this, i](Qt::MouseButton button)
			{
				if (button == Qt::RightButton)
				{
					setOptions(i);
				}
				else
				{
					setSelectedIndex(i);
					updateControls();
				}
			});
			connect(preset, &MRLCDButton::doubleClicked, [this]() { emit load(selected()); });
		}
	}
	else
	{
		if (count > presets.length())
		{
			//Add `nItems` presets
			int nItems = count - presets.length();

			for (int i = 0; i < nItems; i++)
			{
				if (presets.length() == items().length())
				{
					break;
				}

				MRLCDButton* preset = new MRLCDButton(ui->presetHolder);
				preset->setText(items().at(presets.length()));
				preset->setLCDEnabled(false);
				ui->verticalLayout_2->addWidget(preset);
				presets.push_back(preset);
			}
		}
		else if (count < presets.length())
		{
			//Remove `nItems` presets
			int nItems = presets.length() - count;

			for (int i = 0; i < nItems; i++)
			{
				ui->verticalLayout_2->removeWidget(presets.last());
				delete presets.last();
				presets.pop_back();
			}
		}
	}

	updateControls();
}

void MRPresetList::showOptions()
{
	updateControls();

	//Remove all presets
	for (int i = 0; i < presets.length(); i++)
	{
		ui->verticalLayout_2->removeWidget(presets.at(i));
		presets.at(i)->deleteLater();
	}
	presets.clear();

	MRLCDButton* temp = new MRLCDButton(ui->presetHolder);
	newName = items().at(options());
	temp->setName("Rename");
	temp->setText(items().at(options()));
	temp->setEditable(true);
	ui->verticalLayout_2->addWidget(temp);
	presets.append(temp);

	connect(temp, &MRLCDButton::textChanged, [this](QString text) { this->newName = text; updateControls(); });
	connect(temp, &MRLCDButton::finished, [this]()
	{
		if (newName != items().at(options()) && !newName.isEmpty())
		{
			this->loadButton->setText("Load");
			QString oldName = items().at(options());
			items().clear();
			emit(renamePreset(oldName, newName));
			setOptions(-1);
			showPresets(true);
			setSelected(newName);
			newName = "";
			showPresets(true);
			updateControls();
		}
	});

	presets.at(0)->setClickable(false);
	updateControls();
}

void MRPresetList::sort()
{
	items_.sort(Qt::CaseInsensitive);
}

#include <QShowEvent>
void MRPresetList::showEvent(QShowEvent* event)
{
	showPresets();

	event->accept();
}

#include <QResizeEvent>
void MRPresetList::resizeEvent(QResizeEvent* event)
{
	double nCount = ((double)event->size().height() - ui->horizontalLayout->geometry().height() * 2 - ui->presetLabel->geometry().height() - 6) / ((double)prevPage->height() + 6);

	if (nCount == (double)floor(nCount))
	{
		if (event->size().height() < event->oldSize().height())
		{
			count--;
		}
		else
		{
			count++;
		}
		showPresets(page() > 0);
	}
	else
	{
		if (floor(nCount) != count)
		{
			count = floor(nCount);
			showPresets(page() > 0);
		}
	}

	if (selectedIndex() >= 0)
	{
		setPage(floor((double)selectedIndex() / (double)count), false);
	}

	event->accept();
}
