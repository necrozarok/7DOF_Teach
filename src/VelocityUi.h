#pragma once

#include <QWidget>
#include <QMessageBox>
#include "Dynamixel.h"
#include "EPOS.h"
#include <qmath.h>
#include <QTimer>
#include "qcustomplot.h"
#include <QString>
#include <QLayout>
namespace Ui { class VelocityUi; };

class VelocityUi : public QWidget
{
	Q_OBJECT

public:
	VelocityUi(QWidget *parent = Q_NULLPTR);
	~VelocityUi();
	void init(EPOS *e_object, Dynamixel *dy_object);//以便在当前对象调用其他对象的方法
	void setupRealtimeVelocityData(QCustomPlot *customPlot);
private:
	Ui::VelocityUi *ui;
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
signals:
	void closedSignal();
};
