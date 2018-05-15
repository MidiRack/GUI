#ifndef MRPRESETLIST_HPP
#define MRPRESETLIST_HPP

#include <QWidget>

namespace Ui
{
	class MRPresetList;
}

class MRLCDButton;

class MRPresetList : public QWidget
{
	Q_OBJECT

public:
	explicit MRPresetList(QWidget* parent = nullptr);
	~MRPresetList();

	//Preset for which options are shown
	int options();
	void setOptions(int options_);

	//Current page
	int page();
	void setPage(int page_, bool reset = true);

	//Current selected item index
	int selectedIndex();
	void setSelectedIndex(int index_);

	//Current selected item string
	QString selected();
	void setSelected(QString selected_);

	//List of preset names
	QStringList items();
	void setItems(QStringList items_);
	void addItem(QString item_);

	void updateControls();
	void showPresets(bool redo = false);
	void showOptions();

	//Sort the item list, called automatically most times
	void sort();

private:
	Ui::MRPresetList* ui;

	void showEvent(QShowEvent*);
	void resizeEvent(QResizeEvent*);

	//Controls
	MRLCDButton* prevPage;
	MRLCDButton* nextPage;
	MRLCDButton* saveButton;
	MRLCDButton* loadButton;
	QList<MRLCDButton*> presets;

	int options_, page_, selected_;
	QStringList items_;

	//Maximum visible items in available space
	int count;
	QString newName;

signals:
	//Emit if save or load button is pressed
	void save();
	void load(QString name);
	void renamePreset(QString newName, QString oldName);
};

#endif // MRPRESETLIST_HPP
