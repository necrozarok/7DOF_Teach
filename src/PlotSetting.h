#pragma once

#include <QWidget>
namespace Ui { class PlotSetting; };

class PlotSetting : public QWidget
{
	Q_OBJECT

public:
	PlotSetting(QWidget *parent = Q_NULLPTR);
	~PlotSetting();

private:
	Ui::PlotSetting *ui;
};
