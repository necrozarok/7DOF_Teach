#include "TuningButton.h"

TuningButton::TuningButton(QWidget *parent)
	: QPushButton(parent)
{
	this->timer = new QTimer();
	connect(timer, &QTimer::timeout, this, &TuningButton::pressing);
}

TuningButton::~TuningButton()
{
}

bool TuningButton::isPressing()
{
	return mouse_press;
}

void TuningButton::pressing()
{
	emit pressingSignal();
	//QMessageBox::about(NULL, "warning", "No!");
}

void TuningButton::enterEvent(QEvent *)
{
	status = ENTER;
	//update();
}

void TuningButton::leaveEvent(QEvent *)
{
	status = NORMAL;
	//update();
}

void TuningButton::mousePressEvent(QMouseEvent * event)
{
	//Èôµã»÷Êó±ê×ó¼ü
	if (event->button() == Qt::LeftButton)
	{
		mouse_press = true;
		status = PRESS;
		timer->start(200);
		//QMessageBox::about(NULL, "warning", "No!");
		//update();
	}
}

void TuningButton::mouseReleaseEvent(QMouseEvent * event)
{
	//Èôµã»÷Êó±ê×ó¼ü
	if (mouse_press)
	{
		//QMessageBox::about(NULL, "warning", "No!");
		mouse_press = false;
		status = ENTER;
		timer->stop();
		//update();
		emit clicked();
	}
}
