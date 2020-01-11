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
	//ö�ٰ�ť�ļ���״̬
	enum ButtonStatus { NORMAL, ENTER, PRESS, NOSTATUS };
	ButtonStatus status;
	bool mouse_press; //��ť����Ƿ���
public:
	QTimer *timer;
signals:
	void pressingSignal();
};
