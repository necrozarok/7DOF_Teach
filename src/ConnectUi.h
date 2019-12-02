#pragma once

#include <QDialog>
#include <QSerialPortInfo>    //提供系统中存在的串口的信息
#include <QTimer>
#include <QMessageBox>
#include "Dynamixel.h"
#include "EPOS.h"
namespace Ui { class ConnectUi; };

class ConnectUi : public QDialog
{
	Q_OBJECT

public:
	ConnectUi(QWidget *parent = Q_NULLPTR);
	~ConnectUi();
	void add_Dy_ComPort();//显示Dynamixel串口列表
	void init(EPOS *e_object,Dynamixel *dy_object);//以便在当前对象调用其他对象的方法
	void get_EPOS_DeviceName();
public slots:
	void ComScanTimeout();//每隔1秒更新串口列表
	void Connect_clicked();
	void GetProtocolStackName(QString device);
	void GetInterfaceName(QString protocol_stack);
	void GetPortName(QString interfaceName);
signals:
	void Connected();
private:
	Ui::ConnectUi *ui;
	QStringList m_serialPortName;//保存串口列表
	QString  name;//需要操作的串口号
	qint32  baudRate;//波特率
	QTimer *com_scan_timer = nullptr;
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
};
