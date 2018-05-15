#ifndef MRPRESETLIST_HPP
#define MRPRESETLIST_HPP

#include <QWidget>

class MRLCDButton;

class MRPresetList : public QWidget
{
	Q_OBJECT

public:
	explicit MRPresetList(QWidget* parent = nullptr);
	~MRPresetList();

	//Current page
	int page();
	void setPage(int page_);

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

	//Sort the item list, called automatically most times
	void showPresets();
	void sort();

private:
	void showEvent(QShowEvent*);
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);

	//Controls
	MRLCDButton* prevPage;
	MRLCDButton* nextPage;
	MRLCDButton* saveButton;
	MRLCDButton* loadButton;
	QList<MRLCDButton*> presets;

	int page_, selected_;
	QStringList items_;

	//c: Maximum visible items in available space
	//h: Height of one item
	//y: Vertical position
	int c, h, y;

signals:
	//Emit if save or load button is pressed
	void save();
	void load(QString name);
};

#endif // MRPRESETLIST_HPP
