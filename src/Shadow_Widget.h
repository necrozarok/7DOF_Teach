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
	QPoint move_point; //�ƶ��ľ���
	bool mouse_press; //����������
};
