#pragma once

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
class Push_Button : public QPushButton
{
	Q_OBJECT

public:
	explicit Push_Button(QWidget *parent = 0);
	~Push_Button();
	void setPicName(QString pic_name);
protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *);
private:
	//ö�ٰ�ť�ļ���״̬
	enum ButtonStatus { NORMAL, ENTER, PRESS, NOSTATUS };
	ButtonStatus status;
	QString pic_name;

	int btn_width; //��ť���
	int btn_height; //��ť�߶�
	bool mouse_press; //��ť����Ƿ���
signals:
	void pressing();
};
