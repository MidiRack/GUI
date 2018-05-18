#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtSerialPort>

namespace Ui
{
	class MainWindow;
}

class Potmeter;
class MRPresetList;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	//Override show event to initialize custom widgets
	//widget geometry is only available after window shown
	void showEvent(QShowEvent* event);

private:
	Ui::MainWindow* ui;

	QByteArray data;            //Data read from serial line
	QList<Potmeter*> potmeters; //List of potmeters on MidiRack
	QSerialPort port;           //Port where MidiRack is connected on
	QStack<QString> undoItems;  //A stack of actions which can be undone
	QStack<QString> redoItems;  //A stack of undone actions, which can be redone

	MRPresetList* presetList; //Pointer to custom preset list

	bool connected; //Connected to MidiRack

public slots:
	void dataAvailable();
	void setData(int index);
	void getData(QString line);

	void pushUndo(int index, bool clear = true);
	void pushRedo(int index);
	void undo();
	void redo();

	void savePreset();
	void loadPreset(QString name);
	void renamePreset(QString oldName, QString newName);

	void setHighlightColor(int r, int g, int b);

	void loadPotmeters();
};

#endif // MAINWINDOW_HPP
