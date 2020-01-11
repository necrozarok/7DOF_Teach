#include "PlotSetting.h"
#include "ui_PlotSetting.h"

PlotSetting::PlotSetting(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::PlotSetting();
	ui->setupUi(this);
}

PlotSetting::~PlotSetting()
{
	delete ui;
}
