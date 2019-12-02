#pragma once
#include <QDialog>
#include "Dynamixel.h"
#include "EPOS.h"
#include <qmath.h>
#include <QTimer>
#include "qcustomplot.h"
namespace Ui { class TorgueUi; };

class TorgueUi : public QDialog
{
	Q_OBJECT

public:
	TorgueUi(QWidget *parent = Q_NULLPTR);
	~TorgueUi();
	void init(EPOS *e_object, Dynamixel *dy_object);//�Ա��ڵ�ǰ���������������ķ���
private:
	Ui::TorgueUi *ui;
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
private:
	QString demoName;
	QTimer dataTimer;
	QCPItemTracer *itemDemoPhaseTracer;
	int currentDemoIndex;
	QLayout *verticalLayout;
private slots:
	void realtimeDataSlot();
	void bracketDataSlot();
	void screenShot();
	void allScreenShots();

	void setupRealtimeTorgueData(QCustomPlot *customPlot);
signals:
	void closedSignal();
};
