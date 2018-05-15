#ifndef MRLCDBUTTON_HPP
#define MRLCDBUTTON_HPP

#include <QWidget>

#include "MRLCD.hpp"

class MRLCDButton : public MRLCD
{
	Q_OBJECT

public:
	explicit MRLCDButton(QWidget* parent = nullptr);
	~MRLCDButton();

	bool clickable();
	void setClickable(bool clickable_);

	bool editable();
	void setEditable(bool editable_);

private:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);
	void keyPressEvent(QKeyEvent*);

	//Can button be clicked or text edited
	bool clickable_, editable_;

	//Position of the cursor while editing
	int cursorPos;

signals:
	void pressed(Qt::MouseButton);
	void doubleClicked();
	void finished();
};

#endif // MRLCDBUTTON_HPP
