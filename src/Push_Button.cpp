#include "Push_Button.h"

Push_Button::Push_Button(QWidget *parent)
	: QPushButton(parent)
{
	status = NORMAL;
	mouse_press = false;
}

Push_Button::~Push_Button()
{
}

void Push_Button::setPicName(QString pic_name)
{
	this->pic_name = pic_name;
	//将小部件的最小和最大大小都设置为size()，从而防止其增长或收缩
	setFixedSize(QPixmap(pic_name).size());
}

void Push_Button::enterEvent(QEvent *)
{
	status = ENTER;
	update();
}

void Push_Button::leaveEvent(QEvent *)
{
	status = NORMAL;
	update();
}

void Push_Button::mousePressEvent(QMouseEvent * event)
{
	//若点击鼠标左键
	if (event->button() == Qt::LeftButton)
	{
		mouse_press = true;
		status = PRESS;
		emit  pressing();
		update();
	}
}

void Push_Button::mouseReleaseEvent(QMouseEvent * event)
{
	//若点击鼠标左键
	if (mouse_press  && this->rect().contains(event->pos()))
	{
		mouse_press = false;
		status = ENTER;
		update();
		emit clicked();
	}
}

void Push_Button::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	QPixmap pixmap;
	switch (status)
	{
	case NORMAL:
	{
		pixmap.load(pic_name);
		break;
	}
	case ENTER:
	{
		pixmap.load(pic_name + QString("_hover"));
		break;
	}
	case PRESS:
	{
		pixmap.load(pic_name + QString("_pressed"));
		break;
	}
	case NOSTATUS:
	{
		pixmap.load(pic_name);
		break;
	}
	default:
		pixmap.load(pic_name);
	}

	painter.drawPixmap(rect(), pixmap);
}
