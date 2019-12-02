#pragma once

#include <QDialog>
#include <QSerialPortInfo>    //�ṩϵͳ�д��ڵĴ��ڵ���Ϣ
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
	void add_Dy_ComPort();//��ʾDynamixel�����б�
	void init(EPOS *e_object,Dynamixel *dy_object);//�Ա��ڵ�ǰ���������������ķ���
	void get_EPOS_DeviceName();
public slots:
	void ComScanTimeout();//ÿ��1����´����б�
	void Connect_clicked();
	void GetProtocolStackName(QString device);
	void GetInterfaceName(QString protocol_stack);
	void GetPortName(QString interfaceName);
signals:
	void Connected();
private:
	Ui::ConnectUi *ui;
	QStringList m_serialPortName;//���洮���б�
	QString  name;//��Ҫ�����Ĵ��ں�
	qint32  baudRate;//������
	QTimer *com_scan_timer = nullptr;
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
};
