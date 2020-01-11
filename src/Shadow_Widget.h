#pragma once

#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>

class Shadow_Widget : public QDialog
{
	Q_OBJECT
public:
	Shadow_Widget(QWidget *parent = 0);
	~Shadow_Widget();
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *event);
private:
	QPoint move_point; //移动的距离
	bool mouse_press; //按下鼠标左键
};
