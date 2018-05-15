#ifndef MRLCD_HPP
#define MRLCD_HPP

#include <QWidget>

class MRLCD : public QWidget
{
	Q_OBJECT

public:
	explicit MRLCD(QWidget* parent = nullptr);
	~MRLCD();

	//Current value shown, or index of `items` (only is `setItems()` has been called)
	int value();
	void setValue(int value_);

	//Set limits for values, automatically set after `setItems()`
	double max();
	double min();
	void setMinMax(double min_, double max_);

	//Set LCD enabled
	bool LCDEnabled();
	void setLCDEnabled(bool lcdEnabled_);

	//Name shown above LCD
	QString name();
	void setName(QString name_);

	//Force text shown in box
	QString text();
	void setText(QString text_);

	//Unit shown on the right side of the box
	QString unit();
	void setUnit(QString unit_);

	//List of strings to be shown for specific values
	QStringList items();
	void setItems(QStringList items_);

protected:
	void paintEvent(QPaintEvent*);

private:
	int value_;
	double max_, min_;
	bool lcdEnabled_;
	QString name_, text_, unit_;
	QStringList items_;

signals:
	//Emit if value has been changed
	void valueChanged(int value);

	//Emit if text has been changed
	void textChanged(QString text);
};

#endif // MRLCD_HPP
