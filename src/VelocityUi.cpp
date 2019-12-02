#include "VelocityUi.h"
#include "ui_VelocityUi.h"
#pragma execution_character_set("utf-8")
VelocityUi::VelocityUi(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::VelocityUi();
	ui->setupUi(this);
	QIcon icon("/ico/machine2.ico");
	setWindowIcon(icon);
	//setGeometry(500, 500, 600, 600);
	setupRealtimeVelocityData(ui->customPlot);
	setWindowTitle("速度监控: " + demoName);
	//statusBar->clearMessage();
	ui->customPlot->replot();
}

VelocityUi::~VelocityUi()
{
	emit closedSignal();
	delete ui;
}

void VelocityUi::init(EPOS * e_object, Dynamixel * dy_object)
{
	epos_object = e_object;
	dynamixel_object = dy_object;
}

void VelocityUi::setupRealtimeVelocityData(QCustomPlot * customPlot)
{
	demoName = "Real Velocity Data";
	customPlot->legend->setVisible(true);
	customPlot->legend->setFont(QFont("Helvetica", 9));
	// axis configurations:
	customPlot->xAxis->setLabel("Time:second");
	customPlot->yAxis->setLabel("速度/rpm");
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
	customPlot->yAxis->setRange(-100, 100);

	// make left and bottom axes transfer their ranges to right and top axes:
	connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
	connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

	// setup a timer that repeatedly calls CurrentUi::realtimeDataSlot:
	connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
	dataTimer.start(50); // Interval 0 means to refresh as fast as possible
}

void VelocityUi::realtimeDataSlot()
{
	static QTime time(QTime::currentTime());
	// calculate two new data points:
	double key = time.elapsed() / 1000.0; // time elapsed since start of demo, in seconds
	static double lastPointKey = 0;
	if (key - lastPointKey > 0.002) // at most add point every 5 ms
	{
		// add data to lines:
		ui->customPlot->graph(0)->addData(key, (double)20/(44*80)*epos_object->GetVelocityIs(1));
		ui->customPlot->graph(1)->addData(key, (double)epos_object->GetVelocityIs(2)/(2*80));
		ui->customPlot->graph(2)->addData(key, (double)epos_object->GetVelocityIs(3)/100);
		ui->customPlot->graph(3)->addData(key, (double)0.114*(dynamixel_object->getPresentVelocity(1)));
		ui->customPlot->graph(4)->addData(key, (double)0.114*(dynamixel_object->getPresentVelocity(2)));
		ui->customPlot->graph(5)->addData(key, (double)0.114*(dynamixel_object->getPresentVelocity(3)));
		ui->customPlot->graph(6)->addData(key, (double)0.114*(dynamixel_object->getPresentVelocity(4)));
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
