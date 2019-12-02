#include "ConnectUi.h"
#include "ui_ConnectUi.h"
#pragma execution_character_set("utf-8")
ConnectUi::ConnectUi(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ConnectUi();
	ui->setupUi(this);

	QIcon icon("/ico/machine2.ico");
	setWindowIcon(icon);
	//Dynamixel端口扫描定时器
	com_scan_timer = new QTimer(this);
	com_scan_timer->start(1000);
	connect(com_scan_timer, SIGNAL(timeout()), this, SLOT(ComScanTimeout()));
	ui->DyBaudrate->setCurrentIndex(1);
	
	//EPOS端口信息自动获取
	get_EPOS_DeviceName();
	connect(ui->DeviceNameSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(GetProtocolStackName(QString)));
	connect(ui->ProtocolStackNameSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(GetInterfaceName(QString)));
	connect(ui->InterfaceNameSelection, SIGNAL(currentIndexChanged(QString)), this, SLOT(GetPortName(QString)));

	//按下连接button
	connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(Connect_clicked()));
}

ConnectUi::~ConnectUi()
{
	if (com_scan_timer != nullptr)
	{
		delete com_scan_timer;
		com_scan_timer = nullptr;
	}
	delete ui;
}
void ConnectUi::add_Dy_ComPort()
{
	ui->DyComSelection->clear();
	m_serialPortName.clear();
	QList<QSerialPortInfo> m_list = QSerialPortInfo::availablePorts();
	for (int i = 0; i < m_list.count(); i++)
	{
		m_serialPortName << m_list.at(i).portName();
		ui->DyComSelection->addItem(m_list.at(i).portName());
	}
	return;
}

void ConnectUi::init(EPOS *e_object, Dynamixel *dy_object)
{
	epos_object = e_object;
	dynamixel_object = dy_object;
}

void ConnectUi::get_EPOS_DeviceName()
{
	const WORD maxStrSize = 100;
	char deviceNameSel[maxStrSize];
	BOOL end_of_selection = FALSE;
	DWORD errorCode = 0;
	//first device name
	if (VCS_GetDeviceNameSelection(TRUE, deviceNameSel, maxStrSize, &end_of_selection, &errorCode))
	{
		QString dev_name_str(deviceNameSel);
		ui->DeviceNameSelection->addItem(dev_name_str);
		//get next devicename
		while (!end_of_selection)
		{
			VCS_GetDeviceNameSelection(FALSE, deviceNameSel, maxStrSize, &end_of_selection, &errorCode);
			QString dev_name_str(deviceNameSel);
			ui->DeviceNameSelection->addItem(dev_name_str);
		}
	}
}

void ConnectUi::Connect_clicked()
{
	//EPOS初始化
	QByteArray ba_DeviceName;
	ba_DeviceName = ui->DeviceNameSelection->currentText().toLatin1();
	char* DeviceName= ba_DeviceName.data();

	QByteArray ba_ProtocolStackName;
	ba_ProtocolStackName = ui->ProtocolStackNameSelection->currentText().toLatin1();
	char* ProtocolStackName= ba_ProtocolStackName.data();

	QByteArray ba_InterfaceName;
	ba_InterfaceName = ui->InterfaceNameSelection->currentText().toLatin1();
	char* InterfaceName= ba_InterfaceName.data();

	QByteArray ba_PortName;
	ba_PortName = ui->PortNameSelection->currentText().toLatin1();
	char* PortName= ba_PortName.data();

	if(epos_object->OpenDeviceInConnectUi(DeviceName, ProtocolStackName, InterfaceName, PortName))
		QMessageBox::about(NULL, "TIP", "EPOS Connect Success!");
	else
	{
		QMessageBox::about(NULL, "TIP", "EPOS Connect Fault!");
		return;
	}

	//dynamixel初始化
	//QString转char*
	QByteArray ba_dyCom;
	ba_dyCom = ui->DyComSelection->currentText() .toLatin1();
	char * dy_com_char = ba_dyCom.data();
	//QString转int
	int dy_baudrate_int = ui->DyBaudrate->currentText().toInt();
	if (dynamixel_object->Connect(dy_baudrate_int, dy_com_char))
		QMessageBox::about(NULL, "TIP", "Dynamixel Connect Success!");
	else
	{
		QMessageBox::about(NULL, "TIP", "Dynamixel Connect Fault!");
		return;
	}
	emit Connected();
}

void ConnectUi::GetInterfaceName(QString protocol_stack)
{
	ui->InterfaceNameSelection->clear();
	if (protocol_stack == "MAXON SERIAL V2")//因为未知原因，即使连接了USB端口也不会GET到接口类型，所以自行添加USB
		ui->InterfaceNameSelection->addItem("USB");
	const WORD maxStrSize = 100;
	//QString to char*
	QByteArray ba = ui->DeviceNameSelection->currentText().toLatin1();
	char* deviceName = ba.data();	
	ba = protocol_stack.toLatin1();
	char* protocolstackName = ba.data();
	char interfaceNameSel[maxStrSize];
	BOOL end_of_selection = FALSE;
	DWORD errorCode = 0;
	//first interface name
	if (VCS_GetInterfaceNameSelection(deviceName, protocolstackName, TRUE, interfaceNameSel, maxStrSize, &end_of_selection, &errorCode))
	{
		//QMessageBox::about(nullptr, "", ba);
		QString if_name_str(interfaceNameSel);
		ui->InterfaceNameSelection->addItem(if_name_str);
		//get next interface name
		while (!end_of_selection)
		{
			VCS_GetInterfaceNameSelection(deviceName, protocolstackName, TRUE, interfaceNameSel, maxStrSize, &end_of_selection, &errorCode);
			QString if_name_str(interfaceNameSel);
			ui->InterfaceNameSelection->addItem(if_name_str);
		}
	}
}

void ConnectUi::GetPortName(QString interfaceName)
{
	ui->PortNameSelection->clear();
	if (interfaceName == "USB")
		ui->PortNameSelection->addItem("USB0");
	const WORD maxStrSize = 100;
	//QString to char*
	QByteArray ba = ui->DeviceNameSelection->currentText().toLatin1();
	char* deviceName = ba.data();
	ba = ui->ProtocolStackNameSelection->currentText().toLatin1();
	char* protocolstackName = ba.data();
	ba= interfaceName.toLatin1();
	char* interfaceName_char= ba.data();
	char portSel[maxStrSize];
	BOOL end_of_selection = FALSE;
	DWORD errorCode = 0;
	//first port name
	if (VCS_GetPortNameSelection(deviceName, protocolstackName, interfaceName_char, TRUE, portSel, maxStrSize, &end_of_selection, &errorCode))
	{
		QString port_name_str(portSel);
		ui->PortNameSelection->addItem(port_name_str);
		//get next interface name
		while (!end_of_selection)
		{
			VCS_GetPortNameSelection(deviceName, protocolstackName, interfaceName_char, TRUE, portSel, maxStrSize, &end_of_selection, &errorCode);
			QString port_name_str(portSel);
			ui->PortNameSelection->addItem(port_name_str);
		}
	}
}

void ConnectUi::GetProtocolStackName(QString device)
{
	ui->ProtocolStackNameSelection->clear();
	const WORD maxStrSize = 100;
	//QString to char*
	QByteArray ba = device.toLatin1();
	char* deviceName=ba.data();
	char protocolStackNameSel[maxStrSize];
	BOOL end_of_selection = FALSE;
	DWORD errorCode = 0;
	//first protocol stack name
	if (VCS_GetProtocolStackNameSelection(deviceName, TRUE, protocolStackNameSel, maxStrSize, &end_of_selection,&errorCode))
	{
		QString ps_name_str(protocolStackNameSel);
		ui->ProtocolStackNameSelection->addItem(ps_name_str);
		//get next protocol stack name
		while (!end_of_selection)
		{
			VCS_GetProtocolStackNameSelection(deviceName, FALSE, protocolStackNameSel, maxStrSize, &end_of_selection, &errorCode);
			QString ps_name_str(protocolStackNameSel);
			ui->ProtocolStackNameSelection->addItem(ps_name_str);
		}
	}
}

void ConnectUi::ComScanTimeout()
{
	//检测串口是否变化，刷新串口列表
	QStringList m_serialPortName2;
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		m_serialPortName2 << info.portName();
	}
	if (m_serialPortName2 != m_serialPortName)
	{
		add_Dy_ComPort();
	}
	return;
}
