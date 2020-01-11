#pragma once
#define VER1
#include <QWidget>
#include <QMessageBox>
#include "Dynamixel.h"
#include "EPOS.h"
#include <qmath.h>
#include <QTimer>
#include "qcustomplot.h"
#include <QString>
#include <QLayout>

namespace Ui { class CurrentUi; };

class CurrentUi : public QWidget
{
	Q_OBJECT

public:
	CurrentUi(QWidget *parent = Q_NULLPTR);
	~CurrentUi();
	void init(EPOS *e_object, Dynamixel *dy_object);//以便在当前对象调用其他对象的方法

	void setupDemo(int demoIndex);
	void setupQuadraticDemo(QCustomPlot *customPlot);
	void setupSimpleDemo(QCustomPlot *customPlot);
	void setupSincScatterDemo(QCustomPlot *customPlot);
	void setupScatterStyleDemo(QCustomPlot *customPlot);
	void setupLineStyleDemo(QCustomPlot *customPlot);
	void setupScatterPixmapDemo(QCustomPlot *customPlot);
	void setupDateDemo(QCustomPlot *customPlot);
	void setupTextureBrushDemo(QCustomPlot *customPlot);
	void setupMultiAxisDemo(QCustomPlot *customPlot);
	void setupLogarithmicDemo(QCustomPlot *customPlot);
	void setupRealtimeDataDemo(QCustomPlot *customPlot);
	void setupParametricCurveDemo(QCustomPlot *customPlot);
	void setupBarChartDemo(QCustomPlot *customPlot);
	void setupStatisticalDemo(QCustomPlot *customPlot);
	void setupSimpleItemDemo(QCustomPlot *customPlot);
	void setupItemDemo(QCustomPlot *customPlot);
	void setupStyledDemo(QCustomPlot *customPlot);
	void setupAdvancedAxesDemo(QCustomPlot *customPlot);
	void setupColorMapDemo(QCustomPlot *customPlot);
	void setupFinancialDemo(QCustomPlot *customPlot);

	void setupPlayground(QCustomPlot *customPlot);
private:
	Ui::CurrentUi *ui;
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
signals:
	void closedSignal();
private:
	void closeEvent(QCloseEvent *event);

};
