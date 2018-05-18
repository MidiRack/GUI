#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QShortcut>

#include "Potmeter.hpp"
#include "MRKnob.hpp"
#include "MRLCD.hpp"
#include "MRPresetList.hpp"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	presetList(nullptr),
	connected(true)
{
	ui->setupUi(this);

	QShortcut* undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
	QShortcut* redoShortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);

	connect(undoShortcut, &QShortcut::activated, [this]() { undo(); });
	connect(redoShortcut, &QShortcut::activated, [this]() { redo(); });

	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

	for (int i = 0; i < ports.length(); i++)
	{
		if (ports[i].description().contains("Arduino") || ports[i].description().contains("Genuino") || ports[i].description().contains("0043"))
		{
			qDebug() << ports[i].description();

			port.setPort(ports[i]);
			port.setBaudRate(QSerialPort::Baud9600);
			break;
		}
	}

	port.open(QIODevice::ReadWrite);

	connect(&port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));
}

#include <QShowEvent>
void MainWindow::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

	presetList = new MRPresetList(this);
	ui->gridLayout->replaceWidget(ui->presetListHolder, presetList);

	QStringList listItems;
	QDir().mkdir(QDir::currentPath() + "/presets");
	QDirIterator it(QDir::currentPath() + "/presets", QStringList("*.preset"), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		listItems.append(it.next().replace(QDir::currentPath() + "/presets/", "").replace(".preset", ""));
	}
	presetList->setItems(listItems);

	connect(presetList, &MRPresetList::save, [this]() { this->savePreset(); });
	connect(presetList, &MRPresetList::load, [this](QString name) { this->loadPreset(name); });
	connect(presetList, &MRPresetList::renamePreset, [this](QString oldName, QString newName) { this->renamePreset(oldName, newName); });

	loadPotmeters();

	/*QStringList items;
	items.append("OctaveBrass");
	items.append("Clarinet");
	items.append("Claribel");
	items.append("DistortionBell");
	items.append("Mawkish");
	items.append("Kookop");
	items.append("Boulud");
	items.append("Quagmire");
	items.append("Hibox");
	items.append("Wipeout");
	items.append("DucksBackpack");
	items.append("Quiqui");
	items.append("Heeyaw");
	items.append("Foonik");
	items.append("RubberNaught");
	items.append("Woppam");

	MRKnob* waveKnob = new MRKnob(this);
	waveKnob->setMaximum(15);
	waveKnob->setName("Waveform");
	MRLCD* waveValueLCD = new MRLCD(this);
	waveValueLCD->setName("Value");
	waveValueLCD->setItems(items);
	MRLCD* waveTargetLCD = new MRLCD(this);
	waveTargetLCD->setName("Target");
	waveTargetLCD->setItems(items);

	ui->gridLayout->replaceWidget(ui->waveKnobHolder, waveKnob);
	ui->horizontalLayout->replaceWidget(ui->waveTargetLCDHolder, waveTargetLCD);
	ui->horizontalLayout->addWidget(waveValueLCD);

	potmeters.append(new Potmeter(waveKnob, waveValueLCD, waveTargetLCD));
	connect(waveKnob, &MRKnob::released, [this]() { setData(0); });
	connect(waveKnob, &MRKnob::pressed, [this]() { pushUndo(0); });
	connect(waveKnob, &MRKnob::targetChanged, [this]() { potmeters.at(0)->update(); });
	connect(waveKnob, &MRKnob::valueChanged, [this]() { potmeters.at(0)->update(); });
	potmeters.at(0)->update();

	MRKnob* freqKnob = new MRKnob(this);
	freqKnob->setName("Frequency");
	MRLCD* freqValueLCD = new MRLCD(this);
	freqValueLCD->setName("Value");
	freqValueLCD->setUnit("Hz");
	freqValueLCD->setMinMax(8.18, 8372.0);
	MRLCD* freqTargetLCD = new MRLCD(this);
	freqTargetLCD->setName("Target");
	freqTargetLCD->setUnit("Hz");
	freqTargetLCD->setMinMax(8.18, 8372.0);

	ui->gridLayout->replaceWidget(ui->freqKnobHolder, freqKnob);
	ui->horizontalLayout_2->replaceWidget(ui->freqTargetLCDHolder, freqTargetLCD);
	ui->horizontalLayout_2->addWidget(freqValueLCD);

	potmeters.append(new Potmeter(freqKnob, freqValueLCD, freqTargetLCD));
	connect(freqKnob, &MRKnob::released, [this]() { setData(1); });
	connect(freqKnob, &MRKnob::pressed, [this]() { pushUndo(1); });
	connect(freqKnob, &MRKnob::targetChanged, [this]() { potmeters.at(1)->update(); });
	connect(freqKnob, &MRKnob::valueChanged, [this]() { potmeters.at(1)->update(); });
	potmeters.at(1)->update();*/

	connect(&port, SIGNAL(readyRead()), this, SLOT(dataAvailable()));

	event->accept();
}

MainWindow::~MainWindow()
{
	port.close();

	delete ui;
}

void MainWindow::dataAvailable()
{
	data.append(port.readAll());
	if (data.contains("\r\n"))
	{
		QByteArray lineData = data.split('\n')[0];
		QString line = QString(lineData);

		if (line.contains(":"))
		{
			int index = QString(line.split(":").first()).toInt();
			int value = QString(line.split(":").last()).toInt();

			if (index >= 0 && index < potmeters.length())
			{
				if (value >= 0)
				{
					potmeters.at(index)->knob->setValue(value);
				}
				else if (value < 0)
				{
					potmeters.at(index)->knob->setTarget(value);
				}
				potmeters[index]->doUpdate();
			}
			else
			{
				if (value == 0)
				{
					undo();
				}
				else if (value == 1)
				{
					redo();
				}
			}
		}

		data = data.mid(data.indexOf('\n') + 1);
	}
}

void MainWindow::setData(int index)
{
	QString line = QString::number(index) + ":" + QString::number(potmeters.at(index)->knob->target() * (255.0 / potmeters.at(index)->knob->maximum())) + "\r\n";
	port.write(line.toStdString().c_str());
	potmeters.at(index)->doUpdate();
}

void MainWindow::getData(QString line)
{
	if (line.contains(":"))
	{
		int index = QString(line.split(":").first()).toInt();
		int value = QString(line.split(":").last()).toInt();

		potmeters.at(index)->knob->setTarget(value, true);
		potmeters.at(index)->doUpdate();

		setData(index);
	}
}

void MainWindow::pushUndo(int index, bool clear)
{
	if (clear && redoItems.length() > 0)
	{
		redoItems.clear();
	}

	if (undoItems.length() == 0)
	{
		QString line = QString::number(index) + ":" + QString::number(potmeters.at(index)->knob->value());
		undoItems.push(line);
	}

	QString line = QString::number(index) + ":" + QString::number(potmeters.at(index)->knob->target());
	undoItems.push(line);
}

void MainWindow::pushRedo(int index)
{
	QString line = QString::number(index) + ":" + QString::number(potmeters.at(index)->knob->target());
	redoItems.push(line);
}

void MainWindow::undo()
{
	if (undoItems.length() > 0)
	{
		QString item = undoItems.pop();
		while (item.endsWith("-1"))
		{
			if (undoItems.length() == 0)
			{
				return;
			}

			item = undoItems.pop();
		}

		pushRedo(QString(item.split(':').at(0)).toInt());
		getData(item);
	}
}

void MainWindow::redo()
{
	if (redoItems.length() > 0)
	{
		QString item = redoItems.pop();
		while (item.endsWith("-1"))
		{
			if (redoItems.length() == 0)
			{
				return;
			}

			item = redoItems.pop();
		}

		pushUndo(QString(item.split(':').at(0)).toInt(), false);
		getData(item);
	}
}

#include <iostream>
#include <fstream>
void MainWindow::savePreset()
{
	QString oName = "New Preset";
	QString name = oName;
	int i = 1;
	while (QFile(QDir::currentPath() + "/presets/" + name + ".preset").exists())
	{
		name = oName + " " + QString::number(i);
		i++;
	}

	std::ofstream stream;
	stream.open((QDir::currentPath() + "/presets/" + name + ".preset").toStdString());
	for (int i = 0; i < potmeters.length(); i++)
	{
		stream << i << ":" << potmeters.at(i)->knob->value() << "\n";
	}

	presetList->addItem(name);
	presetList->setSelected(name);
	presetList->setOptions(presetList->selectedIndex());
}

void MainWindow::loadPreset(QString name)
{
	QFile file(QDir::currentPath() + "/presets/" + name + ".preset");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);

	while (!stream.atEnd())
	{
		QString line = stream.readLine();
		getData(line);
	}
}

void MainWindow::renamePreset(QString oldName, QString newName)
{
	if (QFile(QDir::currentPath() + "/presets/" + oldName + ".preset").exists())
	{
		QFile(QDir::currentPath() + "/presets/" + oldName + ".preset").rename(QDir::currentPath() + "/presets/" + newName + ".preset");
	}

	QStringList listItems;
	QDir().mkdir(QDir::currentPath() + "/presets");
	QDirIterator it(QDir::currentPath() + "/presets", QStringList("*.preset"), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		listItems.append(it.next().replace(QDir::currentPath() + "/presets/", "").replace(".preset", ""));
	}
	presetList->setItems(listItems);
}


void MainWindow::setHighlightColor(int r, int g, int b)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Highlight, QColor::fromRgb(r, g, b));
	QApplication::setPalette(pal);
}
#include <QDebug>
void MainWindow::loadPotmeters()
{
	QFile file(QDir::currentPath() + "/potmeters.cfg");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);

	int id = -1;
	bool inBlock = false;

	QString name, unit;
	double min, max;
	QStringList items;

	while (!stream.atEnd())
	{
		QString line = stream.readLine();
		if (line.trimmed() == "{" && !inBlock)
		{
			id += 1;
			inBlock = true;
		}
		else if (line.trimmed() == "}" && inBlock)
		{
			inBlock = false;

			MRKnob* knob = new MRKnob();
			knob->setName(name);
			if (!items.isEmpty())
			{
				knob->setMaximum(items.length() - 1);
			}

			MRLCD* value = new MRLCD();
			value->setName("Value");
			value->setUnit(unit);
			value->setItems(items);
			value->setMinMax(min, max);

			MRLCD* target = new MRLCD();
			target->setName("Target");
			target->setUnit(unit);
			target->setItems(items);
			target->setMinMax(min, max);

			Potmeter* potmeter = new Potmeter(knob, value, target, ui->knobHolder);
			potmeters.append(potmeter);
			ui->gridLayout_2->addWidget(potmeter, floor(id / 3), id % 3);

			connect(knob, &MRKnob::released, [this, id]() { setData(id); });
			connect(knob, &MRKnob::pressed, [this, id]() { pushUndo(id); });
			connect(knob, &MRKnob::targetChanged, [this, id]() { potmeters.at(id)->doUpdate(); });
			connect(knob, &MRKnob::valueChanged, [this, id]() { potmeters.at(id)->doUpdate(); });
			potmeters.at(id)->doUpdate();

			name = "";
			unit = "";
			min = -1;
			max = -1;
			items.clear();
		}

		if (inBlock)
		{
			line = line.trimmed();

			QString val = line.right(line.length() - line.indexOf("=") - 1);
			if (line.startsWith("name="))
			{
				name = val;
			}
			else if (line.startsWith("unit="))
			{
				unit = val;
			}
			else if (line.startsWith("limits="))
			{
				line = val.mid(1, val.length() - 2).trimmed();
				QStringList list = line.split(",");
				min = list.at(0).trimmed().toDouble();
				max = list.at(1).trimmed().toDouble();
			}
			else if (line.startsWith("items="))
			{
				line = val.mid(1, val.length() - 2).trimmed();
				QStringList list = line.split(",");
				for (int i = 0; i < list.length(); i++)
				{
					items.append(list.at(i).trimmed());
				}
			}
		}
	}
}
