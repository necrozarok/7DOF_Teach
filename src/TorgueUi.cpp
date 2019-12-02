#include "TorgueUi.h"
#include "ui_TorgueUi.h"
#pragma execution_character_set("utf-8")
TorgueUi::TorgueUi(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::TorgueUi();
	ui->setupUi(this);
	QIcon icon("/ico/machine2.ico");
	setWindowIcon(icon);
	//setGeometry(500, 500, 600, 600);

	setupRealtimeTorgueData(ui->customPlot);

	setWindowTitle("力矩监控: " + demoName);
	//statusBar->clearMessage();
	ui->customPlot->replot();
}

TorgueUi::~TorgueUi()
{
	emit closedSignal();
	delete ui;
}

void TorgueUi::init(EPOS * e_object, Dynamixel * dy_object)
{
	epos_object = e_object;
	dynamixel_object = dy_object;
}

void TorgueUi::realtimeDataSlot()
{
	static QTime time(QTime::currentTime());
	// calculate two new data points:
	double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.002) // at most add point every 5 ms
	{
		// add data to lines:
		ui->customPlot->graph(0)->addData(key, (double)(40 / 22 * 80 * 9.55f * 18 * epos_object->GetCurrentIsAveraged(1) / epos_object->GetVelocityIs(1)));
		ui->customPlot->graph(1)->addData(key, (double)(2 * 80 * 9.55f * 18 * epos_object->GetCurrentIsAveraged(2) / epos_object->GetVelocityIs(2)));
		ui->customPlot->graph(2)->addData(key, (double)(100 * 9.55f * 18 * epos_object->GetCurrentIsAveraged(3) / epos_object->GetVelocityIs(3)));
		ui->customPlot->graph(3)->addData(key, (double)(dynamixel_object->getMaxTorque(1)));
		ui->customPlot->graph(4)->addData(key, (double)(dynamixel_object->getMaxTorque(2)));
		ui->customPlot->graph(5)->addData(key, (double)(dynamixel_object->getMaxTorque(3)));
		ui->customPlot->graph(6)->addData(key, (double)(dynamixel_object->getMaxTorque(4)));
		// rescale value (vertical) axis to fit the current data:
		ui->customPlot->graph(0)->rescaleValueAxis(true);
		ui->customPlot->graph(1)->rescaleValueAxis(true);
		ui->customPlot->graph(2)->rescaleValueAxis(true);
		ui->customPlot->graph(3)->rescaleValueAxis(false);
		ui->customPlot->graph(4)->rescaleValueAxis(false);
		ui->customPlot->graph(5)->rescaleValueAxis(false);
		ui->customPlot->graph(6)->rescaleValueAxis(false);
		lastPointKey = key;
	}
	// make key axis range scroll with the data (at a constant range size of 8):
	ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
	ui->customPlot->replot();

	// calculate frames per second:
	static double lastFpsKey;
	static int frameCount;
	++frameCount;
	if (key - lastFpsKey > 2) // average fps over 2 seconds
	{
		ui->statusLabel->setText(
			QString("%1 FPS, Total Data points: %2")
			.arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
			.arg(ui->customPlot->graph(0)->data()->size() + ui->customPlot->graph(1)->data()->size())
		);
		lastFpsKey = key;
		frameCount = 0;
	}
}

void TorgueUi::bracketDataSlot()
{
	double secs = QCPAxisTickerDateTime::dateTimeToKey(QDateTime::currentDateTime());

	// update data to make phase move:
	int n = 500;
	double phase = secs * 5;
	double k = 3;
	QVector<double> x(n), y(n);
	for (int i = 0; i < n; ++i)
	{
		x[i] = i / (double)(n - 1) * 34 - 17;
		y[i] = qExp(-x[i] * x[i] / 20.0)*qSin(k*x[i] + phase);
	}
	ui->customPlot->graph()->setData(x, y);

	itemDemoPhaseTracer->setGraphKey((8 * M_PI + fmod(M_PI*1.5 - phase, 6 * M_PI)) / k);

	ui->customPlot->replot();

	// calculate frames per second:
	double key = secs;
	static double lastFpsKey;
	static int frameCount;
	++frameCount;
	if (key - lastFpsKey > 2) // average fps over 2 seconds
	{
		ui->statusLabel->setText(
			QString("%1 FPS, Total Data points: %2")
			.arg(frameCount / (key - lastFpsKey), 0, 'f', 0)
			.arg(ui->customPlot->graph(0)->data()->size())
		);
		lastFpsKey = key;
		frameCount = 0;
	}
}

void TorgueUi::screenShot()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#elif QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
	QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#else
	QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x() - 7, this->y() - 7, this->frameGeometry().width() + 14, this->frameGeometry().height() + 14);
#endif
	QString fileName = demoName.toLower() + ".png";
	fileName.replace(" ", "");
	pm.save("./screenshots/" + fileName);
	qApp->quit();
}

void TorgueUi::allScreenShots()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	QPixmap pm = QPixmap::grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#elif QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
	QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x() + 2, this->y() + 2, this->frameGeometry().width() - 4, this->frameGeometry().height() - 4);
#else
	QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x() - 7, this->y() - 7, this->frameGeometry().width() + 14, this->frameGeometry().height() + 14);
#endif
	QString fileName = demoName.toLower() + ".png";
	fileName.replace(" ", "");
	pm.save("./screenshots/" + fileName);

	if (currentDemoIndex < 19)
	{
		if (dataTimer.isActive())
			dataTimer.stop();
		dataTimer.disconnect();
		delete ui->customPlot;
		ui->customPlot = new QCustomPlot(ui->centralWidget);
		//verticalLayout->addWidget(ui->customPlot);
		//setupDemo(currentDemoIndex + 1);
		// setup delay for demos that need time to develop proper look:
		int delay = 250;
		if (currentDemoIndex == 10) // Next is Realtime data demo
			delay = 12000;
		else if (currentDemoIndex == 15) // Next is Item demo
			delay = 5000;
		QTimer::singleShot(delay, this, SLOT(allScreenShots()));
	}
	else
	{
		qApp->quit();
	}
}

void TorgueUi::setupRealtimeTorgueData(QCustomPlot * customPlot)
{
	demoName = "Real Torque Data";
	customPlot->legend->setVisible(true);
	customPlot->legend->setFont(QFont("Helvetica", 9));
	// axis configurations:
	customPlot->xAxis->setLabel("Time:second");
	customPlot->yAxis->setLabel("转矩/mNm");
	// include this section to fully disable antialiasing for higher performance:
	/*
	customPlot->setNotAntialiasedElements(QCP::aeAll);
	QFont font;
	font.setStyleStrategy(QFont::NoAntialias);
	customPlot->xAxis->setTickLabelFont(font);
	customPlot->yAxis->setTickLabelFont(font);
	customPlot->legend->setFont(font);
	*/
	customPlot->addGraph(); // blue line
	customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
	customPlot->graph(0)->setName("关节 1");
	customPlot->addGraph(); // red line
	customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
	customPlot->graph(1)->setName("关节 2");
	customPlot->addGraph(); //  line
	customPlot->graph(2)->setPen(QPen(QColor(200, 110, 100)));
	customPlot->graph(2)->setName("关节 3");
	customPlot->addGraph(); //  line
	customPlot->graph(3)->setPen(QPen(QColor(255, 0, 0)));
	customPlot->graph(3)->setName("关节 4");
	customPlot->addGraph(); //  line
	customPlot->graph(4)->setPen(QPen(QColor(0, 110, 40)));
	customPlot->graph(4)->setName("关节 5");
	customPlot->addGraph(); //  line
	customPlot->graph(5)->setPen(QPen(QColor(100, 110, 255)));
	customPlot->graph(5)->setName("关节 6");
	customPlot->addGraph(); //  line
	customPlot->graph(6)->setPen(QPen(QColor(100, 0, 40)));
	customPlot->graph(6)->setName("关节 7");
	for (int i = 0; i < 7; i++)
		customPlot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 2));
	QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	timeTicker->setTimeFormat("%h:%m:%s");
	customPlot->xAxis->setTicker(timeTicker);
	customPlot->axisRect()->setupFullAxesBox();
	customPlot->yAxis->setRange(-6000, 6000);

	// make left and bottom axes transfer their ranges to right and top axes:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

	// setup a timer that repeatedly calls CurrentUi::realtimeDataSlot:
	connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	dataTimer.start(50); // Interval 0 means to refresh as fast as possible
}

