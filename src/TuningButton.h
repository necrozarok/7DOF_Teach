#pragma once

#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>
class TuningButton : public QPushButton
{
	Q_OBJECT

public:
	TuningButton(QWidget *parent=nullptr);
	~TuningButton();
	bool isPressing();
	void pressing();
protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	//枚举按钮的几种状态
	enum ButtonStatus { NORMAL, ENTER, PRESS, NOSTATUS };
	ButtonStatus status;
	bool mouse_press; //按钮左键是否按下
public:
	QTimer *timer;
signals:
	void pressingSignal();
};
