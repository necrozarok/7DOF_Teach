#include "DOF_Teach.h"
#pragma execution_character_set("utf-8")
DOF_Teach::DOF_Teach(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	isLoopRecurring = FALSE;
	isConnect = FALSE;
	table_row = 0;
	row_count=0;
	column_count=0;
	current_row = 0;
	//ȥ��������
	//this->setWindowFlags(Qt::FramelessWindowHint);
	ui.tabWidget->setCurrentIndex(0);
	//pos���ʼ��
	//ui.PosView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.PosView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui.PosView->setRowCount(0);
	//ui.PosView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");//ͨ����ʽ�����ñ�ͷ����ɫ
	//connect(ui.PosView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(getItem(QTreeWidgetItem*, int)));//��ȡѡ�еĵ�Ԫ������

	//��ť
	connect(ui.btnCurrent, &QPushButton::clicked, this, &DOF_Teach::btn_Current_clicked);
	connect(ui.btnTorgue, &QPushButton::clicked, this, &DOF_Teach::btn_Torgue_clicked);
	connect(ui.btnVelocity, &QPushButton::clicked, this, &DOF_Teach::btn_Velocity_clicked);
	connect(ui.btnGetPos, &QPushButton::clicked, this, &DOF_Teach::btn_GetPos_clicked);
	connect(ui.btnDelete_Pos, &QPushButton::clicked, this, &DOF_Teach::btn_DeletePos_clicked);
	connect(ui.btnRecurring, &QPushButton::clicked, this, &DOF_Teach::btn_Recurring_clicked);
	connect(ui.btnLoopRecurring, &QPushButton::clicked, this, &DOF_Teach::btn_LoopRecurring_clicked);
	connect(ui.btnRecurring_Halt, &QPushButton::clicked, this, &DOF_Teach::Recurring_Halt_clicked);
	connect(ui.btnRecurring_UrgencyHalt, &QPushButton::clicked, this, &DOF_Teach::Recurring_UrgencyHalt_clicked);
	connect(ui.cbActuate, &QCheckBox::stateChanged, this, &DOF_Teach::cb_Actuate_checked);
	connect(ui.cbRelate, &QCheckBox::stateChanged, this, &DOF_Teach::cb_Relate_checked);
	connect(ui.cbTorgue, &QCheckBox::stateChanged, this, &DOF_Teach::cb_Torgue_checked);
	connect(ui.cbCurrentThreShold, &QCheckBox::stateChanged, this, &DOF_Teach::cb_CurrentThreshold_checked);
	
	//�������ǶԻ������
	dlg_about = new about(this);

	ui.mdiArea->setViewMode(QMdiArea::TabbedView);
	ui.mdiArea->setTabsClosable(true);
}

DOF_Teach::~DOF_Teach()
{
	
	if (dlg_help != nullptr)
		delete dlg_help;	
}

void DOF_Teach::init(EPOS * e_object, Dynamixel * dy_object,ConfigUi * dlg_config,ConnectUi * dlg_connect)
{
	epos_object = e_object;
	dynamixel_object = dy_object;
	this->dlg_config = dlg_config;
	this->dlg_connect = dlg_connect;
}

bool DOF_Teach::saveDataAsStream(QString & aFileName)
{
	//�����ݱ���ΪQtԤ�������������ļ�
	QFile aFile(aFileName);
	if (!(aFile.open(QIODevice::WriteOnly | QIODevice::Truncate)))
		return false;
	QDataStream aStream(&aFile);
	aStream.setVersion(QDataStream::Qt_5_9);//���汾�ţ�д��Ͷ�ȡ�汾����
	qint16 rowCount = ui.PosView->rowCount();
	qint16 colCount = ui.PosView->columnCount();
	aStream << rowCount;
	aStream << colCount;
	//��ȡ��ͷ����
	//for (int i = 0; i < ui.PosView->columnCount(); i++)
	//{
	//	QString str = ui.PosView->horizontalHeaderItem(i)->text();
	//	aStream << str;//�ַ���д���ļ���
	//}
	//��ȡ������������
	for (int i = 0; i < ui.PosView->rowCount(); i++)
	{
		QTableWidgetItem* aItem = ui.PosView->item(i, 0);
		qreal node1pos = aItem->data(Qt::DisplayRole).toInt();
		aStream << node1pos;//д���ļ���qreal

		aItem = ui.PosView->item(i, 1);
		qreal node2pos = aItem->data(Qt::DisplayRole).toInt();
		aStream << node2pos;//д���ļ���qreal

		aItem = ui.PosView->item(i, 2);
		qreal node3pos = aItem->data(Qt::DisplayRole).toInt();
		aStream << node3pos;//д���ļ���qreal

		aItem = ui.PosView->item(i, 3);
		qreal id1 = aItem->data(Qt::DisplayRole).toInt();
		aStream << id1;//д���ļ���qreal

		aItem = ui.PosView->item(i, 4);
		qreal id2 = aItem->data(Qt::DisplayRole).toInt();
		aStream << id2;//д���ļ���qreal

		aItem = ui.PosView->item(i, 5);
		qreal id3 = aItem->data(Qt::DisplayRole).toInt();
		aStream << id3;//д���ļ���qreal

		aItem = ui.PosView->item(i, 6);
		qreal id4 = aItem->data(Qt::DisplayRole).toInt();
		aStream << id4;//д���ļ���qreal
	}
	aFile.close();
	return true;

	
}

bool DOF_Teach::saveBinaryFile(QString & aFileName)
{
	//�����ݱ���Ϊ��׼����������ļ�
	QFile aFile(aFileName);
	if (!(aFile.open(QIODevice::WriteOnly)))
		return false;
	QDataStream aStream(&aFile);
	aStream.setByteOrder(QDataStream::LittleEndian);//С���ֽ���
	qint16 rowCount = ui.PosView->rowCount();
	qint16 colCount = ui.PosView->columnCount();
	aStream.writeRawData((char *)&rowCount,sizeof(qint16));
	aStream.writeRawData((char *)&colCount, sizeof(qint16));
	//��ȡ��ͷ����
	//QByteArray btArray;
	//for (int i = 0; i < ui.PosView->columnCount(); i++)
	//{
	//	QString str = ui.PosView->horizontalHeaderItem(i)->text();
	//	btArray = str.toUtf8();//ת��Ϊ�ַ�����
	//	aStream.writeBytes(btArray, btArray.length());//д���ַ�������
	//}
	//��ȡ������������(д���ַ�����aStream.writeBytes����ֵ��writeRawData)
	for (int i = 0; i < ui.PosView->rowCount(); i++)
	{
		QTableWidgetItem* aItem = ui.PosView->item(i, 0);
		qint16 node1pos = aItem->data(Qt::DisplayRole).toInt();
		aStream.writeRawData((char *)&node1pos, sizeof(qint16));;//д���ļ���qreal

		aItem = ui.PosView->item(i, 1);
		qint16 node2pos = aItem->data(Qt::DisplayRole).toInt();
		aStream.writeRawData((char *)&node2pos, sizeof(qint16));;//д���ļ���qreal

		aItem = ui.PosView->item(i, 2);
		qint16 node3pos = aItem->data(Qt::DisplayRole).toInt();
		aStream.writeRawData((char *)&node3pos, sizeof(qint16));;//д���ļ���qreal

		aItem = ui.PosView->item(i, 3);
		qint16 id1 = aItem->data(Qt::DisplayRole).toInt();
		aStream.writeRawData((char *)&id1, sizeof(qint16));;//д���ļ���qreal

		aItem = ui.PosView->item(i, 4);
		qint16 id2 = aItem->data(Qt::DisplayRole).toInt();
		aStream.writeRawData((char *)&id2, sizeof(qint16));;//д���ļ���qreal

		aItem = ui.PosView->item(i, 5);
		qint16 id3 = aItem->data(Qt::DisplayRole).toInt();
		aStream.writeRawData((char *)&id3, sizeof(qint16));;//д���ļ���qreal

		aItem = ui.PosView->item(i, 6);
		qint16 id4 = aItem->data(Qt::DisplayRole).toInt();
		aStream.writeRawData((char *)&id4, sizeof(qint16));;//д���ļ���qreal
	}
	aFile.close();
	return true;
}

bool DOF_Teach::openDataAsStream(QString & aFileName)
{
	//�������
	row_count = 0;
	ui.PosView->setRowCount(row_count);
	//��stm�ļ���ȡ����
	QFile aFile(aFileName);
	if (!(aFile.open(QIODevice::ReadOnly)))
		return false;
	QDataStream aStream(&aFile);//�ļ������
	aStream.setVersion(QDataStream::Qt_5_9);//�������ļ��汾��
	qint16 rowCount, colCount;
	aStream >> rowCount;//��ȡ����
	aStream >> colCount;//��ȡ����
	//this->resetTable(rowCount);//���λ���趨����
	//��ȡ��ͷ����
	//QString str;
	//for (int i = 0; i < colCount; i++)
	//	aStream >> str;//��ȡ��ͷ�ַ���
	//��ȡ����������
	qreal node1pos, node2pos, node3pos, id1, id2, id3, id4;
	QTableWidgetItem *aItem;
	for (int i = 0; i < rowCount; i++)
	{
		ui.PosView->setRowCount(++table_row);
		//ui.PosView->setItem(table_row - 1, 0, new QTableWidgetItem(QString(QString::number(node1_pos))));
		aStream >> node1pos;
		aItem = new QTableWidgetItem(QString(QString::number(node1pos)));
		ui.PosView->setItem(i, 0, aItem);
		
		aStream >> node2pos;
		aItem = new QTableWidgetItem(QString(QString::number(node2pos)));
		ui.PosView->setItem(i, 1, aItem);

		aStream >> node3pos;
		aItem = new QTableWidgetItem(QString(QString::number(node3pos)));
		ui.PosView->setItem(i, 2, aItem);

		aStream >> id1;
		aItem = new QTableWidgetItem(QString(QString::number(id1)));
		ui.PosView->setItem(i, 3, aItem);

		aStream >> id2;
		aItem = new QTableWidgetItem(QString(QString::number(id2)));
		ui.PosView->setItem(i, 4, aItem);

		aStream >> id3;
		aItem = new QTableWidgetItem(QString(QString::number(id3)));
		ui.PosView->setItem(i, 5, aItem);

		aStream >> id4;
		aItem = new QTableWidgetItem(QString(QString::number(id4)));
		ui.PosView->setItem(i, 6, aItem);
	}
	aFile.close();
	return true;
}

bool DOF_Teach::openBinaryFile(QString & aFileName)
{
	
	//��stm�ļ���ȡ����
	QFile aFile(aFileName);
	if (!(aFile.open(QIODevice::ReadOnly)))
		return false;
	QDataStream aStream(&aFile);//�ļ������
	aStream.setByteOrder(QDataStream::LittleEndian);//С���ֽ���
	qint16 rowCount, colCount;
	aStream.readRawData((char *)&rowCount,sizeof(qint16));//��ȡ����
	aStream.readRawData((char *)&colCount, sizeof(qint16));//��ȡ����
	//�������
	row_count = 0;
	ui.PosView->setRowCount(row_count);
	//��ȡ��ͷ����
	//char *buf;
	//uint strlen;
	//for (int i = 0; i < colCount; i++)
	//{
	//	aStream.readBytes(buf, strlen);//ͬʱ��ȡ�ַ������Ⱥ�����
	//	QString str=QString::fromLocal8Bit(buf,strlen);
	//}
	//��ȡ����������
	qint16 node1pos, node2pos, node3pos, id1, id2, id3, id4;
	QTableWidgetItem *aItem;
	for (int i = 0; i < rowCount; i++)
	{
		ui.PosView->setRowCount(++table_row);
		//ui.PosView->setItem(table_row - 1, 0, new QTableWidgetItem(QString(QString::number(node1_pos))));
		aStream.readRawData((char *)&node1pos, sizeof(qint16));
		aItem = new QTableWidgetItem(QString(QString::number(node1pos)));
		ui.PosView->setItem(i, 0, aItem);

		aStream.readRawData((char *)&node2pos, sizeof(qint16));
		aItem = new QTableWidgetItem(QString(QString::number(node2pos)));
		ui.PosView->setItem(i, 1, aItem);

		aStream.readRawData((char *)&node3pos, sizeof(qint16));
		aItem = new QTableWidgetItem(QString(QString::number(node3pos)));
		ui.PosView->setItem(i, 2, aItem);

		aStream.readRawData((char *)&id1, sizeof(qint16));
		aItem = new QTableWidgetItem(QString(QString::number(id1)));
		ui.PosView->setItem(i, 3, aItem);

		aStream.readRawData((char *)&id2, sizeof(qint16));
		aItem = new QTableWidgetItem(QString(QString::number(id2)));
		ui.PosView->setItem(i, 4, aItem);

		aStream.readRawData((char *)&id3, sizeof(qint16));
		aItem = new QTableWidgetItem(QString(QString::number(id3)));
		ui.PosView->setItem(i, 5, aItem);

		aStream.readRawData((char *)&id4, sizeof(qint16));
		aItem = new QTableWidgetItem(QString(QString::number(id4)));
		ui.PosView->setItem(i, 6, aItem);
	}
	aFile.close();
	return true;
}

void DOF_Teach::on_actionConfig_triggered()
{
	if (dlg_config == nullptr)
	{
		QMessageBox::about(NULL, "warning", "No connection!");
		return;
	}		
	dlg_config->exec();
}

void DOF_Teach::on_actionConnect_triggered()
{
	/*if (dlg_connect == nullptr)
		dlg_connect = new ConnectUi(this);*/
	dlg_connect->exec();
}

void DOF_Teach::on_actionSave_triggered()
{
	if (ui.PosView->rowCount() < 1)
		return;
	//��QTԤ������뱣���ļ�
	//QString curPath = QDir::currentPath();
	////QMessageBox::about(this, "", curPath);
	//QString aFileName = QFileDialog::getSaveFileName(this, "ѡ�񱣴��ļ�", curPath, "QtԤ������������ļ�(*.stm)");
	//if (aFileName.isEmpty())
	//	return;
	////QMessageBox::about(this, "", aFileName);
	//if (saveDataAsStream(aFileName))
	//	QMessageBox::information(this, "��ʾ��Ϣ", "�ļ��Ѿ��ɹ����棡");

	//��.dat��ʽ����
	QString curPath = QDir::currentPath();
	//QMessageBox::about(this, "", curPath);
	QString aFileName = QFileDialog::getSaveFileName(this, "ѡ�񱣴��ļ�", curPath, "��׼���������ļ�(*.dat)");
	if (aFileName.isEmpty())
		return;
	//QMessageBox::about(this, "", aFileName);
	if (saveBinaryFile(aFileName))
		QMessageBox::information(this, "��ʾ��Ϣ", "�ļ��Ѿ��ɹ����棡");
}

void DOF_Teach::on_actionOpen_triggered()
{
	//��stm�ļ�
	/*QString curPath = QDir::currentPath();
	QString aFileName = QFileDialog::getOpenFileName(this, "��һ���ļ�", curPath, "QtԤ������������ļ�(*.stm)");
	if (aFileName.isEmpty())
		return;
	if (openDataAsStream(aFileName))
		QMessageBox::information(this, "��ʾ��Ϣ", "�ļ��Ѿ��򿪣�");*/

	//��dat�ļ�
	QString curPath = QDir::currentPath();
	QString aFileName = QFileDialog::getOpenFileName(this, "��һ���ļ�", curPath, "��׼���������ļ�(*.dat)");
	if (aFileName.isEmpty())
		return;
	if (openBinaryFile(aFileName))
		QMessageBox::information(this, "��ʾ��Ϣ", "�ļ��Ѿ��򿪣�");
}

void DOF_Teach::on_actionAbout_triggered()
{
	dlg_about->exec();//ģ̬
}

void DOF_Teach::on_actionDemo_triggered()
{
	//if(dlg_help==nullptr)
	dlg_help = new HelpUi(this);
	dlg_help->setAttribute(Qt::WA_DeleteOnClose);//�Ի���ر�ʱ�Զ�ɾ��
	dlg_help->setWindowFlag(Qt::Dialog, true);
	Qt::WindowFlags flags = dlg_help->windowFlags();//��ȡ����flags
	dlg_help->setWindowFlags(flags | Qt::WindowStaysOnBottomHint);
	dlg_help->setWindowOpacity(0.8);
	dlg_help->show();//��ģ̬
}

void DOF_Teach::on_sliderActuate_valueChanged(int value)
{
	QString str = "��ǰ���棺" + QString::asprintf("%dqc", value);
	ui.labActuate->setText(str);
	emit ActuateSliderSignal(value);
}

void DOF_Teach::btn_Current_clicked()
{
	if (epos_object == nullptr&&dynamixel_object == nullptr)
	{
		QMessageBox::about(NULL, "warning", "No connection!");
		return;
	}
	ui.btnCurrent->setEnabled(0);
	//if(dlg_current==nullptr)
	dlg_current = new CurrentUi(this);
	dlg_current->setAttribute(Qt::WA_DeleteOnClose);//�Ի���ر�ʱ�Զ�ɾ��
	Qt::WindowFlags flags = dlg_current->windowFlags();//��ȡ����flags
	dlg_current->setWindowFlags(flags | Qt::WindowStaysOnBottomHint);
	dlg_current->setWindowFlag(Qt::Dialog, true);
	dlg_current->init(epos_object, dynamixel_object);
	ui.mdiArea->addSubWindow(dlg_current);
	
	dlg_current->show();//��ģ̬
	connect(dlg_current, &CurrentUi::closedSignal, this, &DOF_Teach::btn_Current_closed);
}

void DOF_Teach::btn_Current_closed()
{
	//delete dlg_current;
	
	ui.btnCurrent->setEnabled(1);
}

void DOF_Teach::btn_Torgue_clicked()
{
	if (epos_object == nullptr&&dynamixel_object == nullptr)
	{
		QMessageBox::about(NULL, "warning", "No connection!");
		return;
	}
	ui.btnTorgue->setEnabled(0);
	//if (dlg_torgue == nullptr)
	dlg_torgue = new TorgueUi(this);
	dlg_torgue->setAttribute(Qt::WA_DeleteOnClose);//�Ի���ر�ʱ�Զ�ɾ��
	Qt::WindowFlags flags = dlg_torgue->windowFlags();//��ȡ����flags
	dlg_torgue->setWindowFlags(flags | Qt::WindowStaysOnBottomHint);
	//dlg_torgue->setWindowFlag(Qt::Dialog, true);
	dlg_torgue->init(epos_object, dynamixel_object);
	ui.mdiArea->addSubWindow(dlg_torgue);
	dlg_torgue->show();//��ģ̬
	connect(dlg_torgue, &TorgueUi::closedSignal, this, &DOF_Teach::btn_Torgue_closed);
}

void DOF_Teach::btn_Torgue_closed()
{
	ui.btnTorgue->setEnabled(1);
}

void DOF_Teach::btn_Velocity_clicked()
{
	if (epos_object == nullptr&&dynamixel_object == nullptr)
	{
		QMessageBox::about(NULL, "warning", "No connection!");
		return;
	}
	ui.btnVelocity->setEnabled(0);
	//if(dlg_velocity == nullptr)
	dlg_velocity = new VelocityUi(this);
	dlg_velocity->setAttribute(Qt::WA_DeleteOnClose);//�Ի���ر�ʱ�Զ�ɾ��
	Qt::WindowFlags flags = dlg_velocity->windowFlags();//��ȡ����flags
	dlg_velocity->setWindowFlags(flags | Qt::WindowStaysOnBottomHint);
	dlg_velocity->setWindowFlag(Qt::Dialog, true);
	dlg_velocity->init(epos_object, dynamixel_object);
	ui.mdiArea->addSubWindow(dlg_velocity);
	dlg_velocity->show();//��ģ̬
	connect(dlg_velocity, &VelocityUi::closedSignal, this, &DOF_Teach::btn_Velocity_closed);
}

void DOF_Teach::btn_Velocity_closed()
{
	ui.btnVelocity->setEnabled(1);
}

void DOF_Teach::Recurring_UrgencyHalt_clicked()
{
	emit UrgencyHaltSignal();

	current_row = row_count + 1;
	if (isLoopRecurring)
	{
		isLoopRecurring = FALSE;
	}

	ui.btnRecurring->setEnabled(1);
	ui.btnLoopRecurring->setEnabled(1);
	ui.btnGetPos->setEnabled(1);
	ui.btnDelete_Pos->setEnabled(1);
}

void DOF_Teach::cb_Actuate_checked()
{
	if (ui.cbActuate->isChecked())
	{
		ui.btnRecurring->setEnabled(0);
		ui.btnLoopRecurring->setEnabled(0);
		emit ActuateCheckedSignal();
	}
	else
	{
		ui.btnRecurring->setEnabled(1);
		ui.btnLoopRecurring->setEnabled(1);
		emit ActuateunCheckedSignal();
	}
}

void DOF_Teach::cb_Relate_checked()
{
}

void DOF_Teach::cb_Torgue_checked()
{
	if (ui.cbTorgue->isChecked())
	{
		emit TorquecheckedSignal();
	}
	else
	{
		emit TorqueuncheckedSignal();
	}
}

void DOF_Teach::cb_CurrentThreshold_checked()
{
	//QMessageBox::about(NULL, "warning", " cb_CurrentThreshold_checked!");
	if (ui.cbCurrentThreShold->isChecked())
	{
		//QMessageBox::about(NULL, "warning", " cb_CurrentThreshold_checked!");
		emit CurrentThresholdCheckedSignal();
	}
	else
	{
		//QMessageBox::about(NULL, "warning", " cb_CurrentThreshold_unchecked!");
		emit CurrentThresholdUncheckedSignal();
	}
}

void DOF_Teach::on_sbCurrentThreshold1_valueChanged(int i)
{
	emit CurrentThresholdValueChangedSignal1(i);
}

void DOF_Teach::on_sbCurrentThreshold2_valueChanged(int i)
{
	emit CurrentThresholdValueChangedSignal2(i);
}

void DOF_Teach::on_sbCurrentThreshold3_valueChanged(int i)
{
	emit CurrentThresholdValueChangedSignal3(i);
}

void DOF_Teach::on_Node1_tuningUp_pressingSignal()
{
	//static int i = 0;
	//i++;
	//if(i>10)
	//	//QMessageBox::about(NULL, "warning", QString::asprintf("step:%d velocity:%d", i, i));
	int step=ui.Node1_tuningStep->value();
	int velocity=ui.Node1_tuningVelocity->value();
	emit TuningUp_Singnal1(step, velocity);
	//QMessageBox::about(NULL, "warning",QString::asprintf("step:%d velocity:%d",step,velocity));
}

void DOF_Teach::on_Node1_tuningDown_pressingSignal()
{
	int step = ui.Node1_tuningStep->value();
	int velocity = ui.Node1_tuningVelocity->value();
	emit TuningDown_Singnal1(step, velocity);
	//QMessageBox::about(NULL, "warning", QString::asprintf("step:%d velocity:%d", step, velocity));
}

void DOF_Teach::on_Node2_tuningUp_pressingSignal()
{
	int step = ui.Node2_tuningStep->value();
	int velocity = ui.Node2_tuningVelocity->value();
	emit TuningUp_Singnal2(step, velocity);
	//QMessageBox::about(NULL, "warning", QString::asprintf("step:%d velocity:%d", step, velocity));
}

void DOF_Teach::on_Node2_tuningDown_pressingSignal()
{
	int step = ui.Node2_tuningStep->value();
	int velocity = ui.Node2_tuningVelocity->value();
	emit TuningDown_Singnal2(step, velocity);
	//QMessageBox::about(NULL, "warning", QString::asprintf("step:%d velocity:%d", step, velocity));
}

void DOF_Teach::on_Node3_tuningUp_pressingSignal()
{
	int step = ui.Node3_tuningStep->value();
	int velocity = ui.Node3_tuningVelocity->value();
	emit TuningUp_Singnal3(step, velocity);
	//QMessageBox::about(NULL, "warning", QString::asprintf("step:%d velocity:%d", step, velocity));
}

void DOF_Teach::on_Node3_tuningDown_pressingSignal()
{
	int step = ui.Node3_tuningStep->value();
	int velocity = ui.Node3_tuningVelocity->value();
	emit TuningDown_Singnal3(step, velocity);
	//QMessageBox::about(NULL, "warning", QString::asprintf("step:%d velocity:%d", step, velocity));
}

void DOF_Teach::on_dial_ID1_sliderMoved(int value)
{
	emit Tuning_Dial_Singnal(1,value);
}

void DOF_Teach::on_dial_ID2_sliderMoved(int value)
{
	emit Tuning_Dial_Singnal(2, value);
}

void DOF_Teach::on_dial_ID3_sliderMoved(int value)
{
	emit Tuning_Dial_Singnal(3, value);
}

void DOF_Teach::on_dial_ID4_sliderMoved(int value)
{
	emit Tuning_Dial_Singnal(4, value);
}

void DOF_Teach::on_tabWidget_currentChanged(int page)
{
	if (1 == page)
		emit Tuning_Dial_Init_Singnal();
}

void DOF_Teach::update_dial(uint16_t cw[4], uint16_t ccw[4], uint16_t current_value[4])
{
	//QMessageBox::about(NULL, "warning","init");
	ui.dial_ID1->setMinimum(cw[0]);
	ui.dial_ID1->setMaximum(ccw[0]);
	ui.dial_ID1->setValue(current_value[0]);

	ui.dial_ID2->setMinimum(cw[1]);
	ui.dial_ID2->setMaximum(ccw[1]);
	ui.dial_ID2->setValue(current_value[1]);

	ui.dial_ID3->setMinimum(cw[2]);
	ui.dial_ID3->setMaximum(ccw[2]);
	ui.dial_ID3->setValue(current_value[2]);

	ui.dial_ID4->setMinimum(cw[3]);
	ui.dial_ID4->setMaximum(ccw[3]);
	ui.dial_ID4->setValue(current_value[3]);
}

void DOF_Teach::on_actionDB_triggered()
{
	dlg_recordDB = new RecrodDB(this);
	Qt::WindowFlags flags = dlg_recordDB->windowFlags();
	dlg_recordDB->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);

	int action_count = ui.PosView->rowCount();
	QList<int> action_data;
	//���붯�������붯����
	if (action_count)
	{
		for (int i = 0; i < action_count; i++)
		{
			try
			{
				QTableWidgetItem* aItem = ui.PosView->item(i, 0);
				action_data.append(aItem->data(Qt::DisplayRole).toInt());

				aItem = ui.PosView->item(i, 1);
				action_data << aItem->data(Qt::DisplayRole).toInt();

				aItem = ui.PosView->item(i, 2);
				action_data << aItem->data(Qt::DisplayRole).toInt();

				aItem = ui.PosView->item(i, 3);
				action_data << aItem->data(Qt::DisplayRole).toInt();

				aItem = ui.PosView->item(i, 4);
				action_data << aItem->data(Qt::DisplayRole).toInt();

				aItem = ui.PosView->item(i, 5);
				action_data << aItem->data(Qt::DisplayRole).toInt();

				aItem = ui.PosView->item(i, 6);
				action_data << aItem->data(Qt::DisplayRole).toInt();
			}
			catch (...)
			{
				QMessageBox::about(NULL, "warning","���ݱ��ȡ�쳣");
			}
		}
		dlg_recordDB->ImportData(action_count, action_data);
	}
	dlg_recordDB->exec();//ģ̬

	if (dlg_recordDB->IsRead())
	{
		//��ȡ���ݲ���ʾ��tablewidget��
		action_data = dlg_recordDB->OutData();
		action_count = dlg_recordDB->OutCount();
		int node1pos, node2pos, node3pos, id1, id2, id3, id4;
		QTableWidgetItem *aItem;
		for (int i = 0; i < action_count; i++)
		{
			ui.PosView->setRowCount(++table_row);
			//ui.PosView->setItem(table_row - 1, 0, new QTableWidgetItem(QString(QString::number(node1_pos))));
			node1pos = action_data[0 + 7 * i];
			aItem = new QTableWidgetItem(QString(QString::number(node1pos)));
			ui.PosView->setItem(i, 0, aItem);

			node2pos = action_data[1 + 7 * i];;
			aItem = new QTableWidgetItem(QString(QString::number(node2pos)));
			ui.PosView->setItem(i, 1, aItem);

			node3pos = action_data[2 + 7 * i];;
			aItem = new QTableWidgetItem(QString(QString::number(node3pos)));
			ui.PosView->setItem(i, 2, aItem);

			id1 = action_data[3 + 7 * i];;
			aItem = new QTableWidgetItem(QString(QString::number(id1)));
			ui.PosView->setItem(i, 3, aItem);

			id2 = action_data[4 + 7 * i];;
			aItem = new QTableWidgetItem(QString(QString::number(id2)));
			ui.PosView->setItem(i, 4, aItem);

			id3 = action_data[5 + 7 * i];;
			aItem = new QTableWidgetItem(QString(QString::number(id3)));
			ui.PosView->setItem(i, 5, aItem);

			id4 = action_data[6 + 7 * i];;
			aItem = new QTableWidgetItem(QString(QString::number(id4)));
			ui.PosView->setItem(i, 6, aItem);
		}
	}
	

	delete  dlg_recordDB;
}

void DOF_Teach::btn_GetPos_clicked()
{
	ui.PosView->setRowCount(++table_row);
	emit TeachReadPosSignal();
}

void DOF_Teach::btn_GetPos_clicked_Receive(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos)
{
	ui.PosView->setItem(table_row - 1, 0, new QTableWidgetItem(QString(QString::number(node1_pos))));
	ui.PosView->setItem(table_row - 1, 1, new QTableWidgetItem(QString(QString::number(node2_pos))));
	ui.PosView->setItem(table_row - 1, 2, new QTableWidgetItem(QString(QString::number(node3_pos))));
	ui.PosView->setItem(table_row - 1, 3, new QTableWidgetItem(QString(QString::number(ID1_pos))));
	ui.PosView->setItem(table_row - 1, 4, new QTableWidgetItem(QString(QString::number(ID2_pos))));
	ui.PosView->setItem(table_row - 1, 5, new QTableWidgetItem(QString(QString::number(ID3_pos))));
	ui.PosView->setItem(table_row - 1, 6, new QTableWidgetItem(QString(QString::number(ID4_pos))));
}

void DOF_Teach::btn_DeletePos_clicked()
{
	if (ui.PosView->rowCount() == 0)
		return;
	ui.PosView->setRowCount(--table_row);
}

void DOF_Teach::btn_Recurring_clicked()
{
	if (ui.PosView->rowCount() == 0)
		return;

	isLoopRecurring = FALSE;

	ui.btnRecurring->setEnabled(0);
	ui.btnGetPos->setEnabled(0);
	ui.btnDelete_Pos->setEnabled(0);
	ui.btnLoopRecurring->setEnabled(0);	

	current_row = 0;
	row_count = ui.PosView->rowCount();
	//column_count = ui.PosView->columnCount();
	SingleOrLoopRecurring();		
}

void DOF_Teach::btn_LoopRecurring_clicked()
{
	if (ui.PosView->rowCount() == 0)
		return;

	isLoopRecurring = TRUE;

	ui.btnRecurring->setEnabled(0);
	ui.btnGetPos->setEnabled(0);
	ui.btnDelete_Pos->setEnabled(0);
	ui.btnLoopRecurring->setEnabled(0);

	current_row = 0;
	row_count = ui.PosView->rowCount();
	//column_count = ui.PosView->columnCount();
	SingleOrLoopRecurring();
}

void DOF_Teach::SingleOrLoopRecurring()
{
	if (isLoopRecurring)
	{
		static BOOL isinverse = false;//ѭ������flag
		if (!isinverse)
		{
			if (current_row < row_count)
			{
				emit TeachRecurringSignal(
					ui.PosView->item(current_row, 0)->text().toInt(),
					ui.PosView->item(current_row, 1)->text().toInt(),
					ui.PosView->item(current_row, 2)->text().toInt(),
					ui.PosView->item(current_row, 3)->text().toInt(),
					ui.PosView->item(current_row, 4)->text().toInt(),
					ui.PosView->item(current_row, 5)->text().toInt(),
					ui.PosView->item(current_row, 6)->text().toInt()
				);
				current_row++;
				return;
			}
			else
			{
				isinverse = true;
				current_row -= 2;
				emit TeachRecurringSignal(
					ui.PosView->item(current_row, 0)->text().toInt(),
					ui.PosView->item(current_row, 1)->text().toInt(),
					ui.PosView->item(current_row, 2)->text().toInt(),
					ui.PosView->item(current_row, 3)->text().toInt(),
					ui.PosView->item(current_row, 4)->text().toInt(),
					ui.PosView->item(current_row, 5)->text().toInt(),
					ui.PosView->item(current_row, 6)->text().toInt()
				);
				return;
			}
		}
		else
		{
			if (current_row >= 0)
			{
				emit TeachRecurringSignal(
					ui.PosView->item(current_row, 0)->text().toInt(),
					ui.PosView->item(current_row, 1)->text().toInt(),
					ui.PosView->item(current_row, 2)->text().toInt(),
					ui.PosView->item(current_row, 3)->text().toInt(),
					ui.PosView->item(current_row, 4)->text().toInt(),
					ui.PosView->item(current_row, 5)->text().toInt(),
					ui.PosView->item(current_row, 6)->text().toInt()
				);
				current_row--;
				return;
			}
			else
			{
				isinverse = false;
				current_row += 2;
				emit TeachRecurringSignal(
					ui.PosView->item(current_row, 0)->text().toInt(),
					ui.PosView->item(current_row, 1)->text().toInt(),
					ui.PosView->item(current_row, 2)->text().toInt(),
					ui.PosView->item(current_row, 3)->text().toInt(),
					ui.PosView->item(current_row, 4)->text().toInt(),
					ui.PosView->item(current_row, 5)->text().toInt(),
					ui.PosView->item(current_row, 6)->text().toInt()
				);
				return;
			}
		}
	}
	else
	{
		if (current_row < row_count)
		{
			emit TeachRecurringSignal(
				ui.PosView->item(current_row, 0)->text().toInt(),
				ui.PosView->item(current_row, 1)->text().toInt(),
				ui.PosView->item(current_row, 2)->text().toInt(),
				ui.PosView->item(current_row, 3)->text().toInt(),
				ui.PosView->item(current_row, 4)->text().toInt(),
				ui.PosView->item(current_row, 5)->text().toInt(),
				ui.PosView->item(current_row, 6)->text().toInt()
			);
			current_row++;
		}
		else
		{
			ui.btnRecurring->setEnabled(1);
			ui.btnGetPos->setEnabled(1);
			ui.btnDelete_Pos->setEnabled(1);
			ui.btnLoopRecurring->setEnabled(1);
			current_row = 0;
		}
	}
}

void DOF_Teach::Recurring_Halt_clicked()
{
	current_row = row_count + 1;
	if (isLoopRecurring)
	{
		isLoopRecurring = FALSE;
	}
	ui.btnRecurring->setEnabled(1);
	ui.btnLoopRecurring->setEnabled(1);
	ui.btnGetPos->setEnabled(1);
	ui.btnDelete_Pos->setEnabled(1);
}

void DOF_Teach::StatusBar_Show_Pos(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos)
{
	QString pos_message = "Node1_Pos:" + QString::number(node1_pos) + "\t\t\t\tNode2_Pos:" + QString::number(node2_pos) + "\t\t\t\tNode3_Pos:" + QString::number(node3_pos) + "\t\t\t\tID1_Pos:" + QString::number(ID1_pos) + "\t\t\t\tID2_Pos:" + QString::number(ID2_pos) + "\t\t\t\tID3_Pos:" + QString::number(ID3_pos) + "\t\t\t\tID4_Pos:" + QString::number(ID4_pos);
	statusBar()->showMessage(pos_message, 3000);//3s��ر�
}

void DOF_Teach::mousePressEvent(QMouseEvent * event)
{
	//��갴��
	if (event->button() == Qt::LeftButton)
	{
		m_moving = true;
		m_lastPos = event->globalPos() - pos();//��¼���������ڴ��ڵ�λ��
	}
	return QMainWindow::mousePressEvent(event);
}

void DOF_Teach::mouseMoveEvent(QMouseEvent * event)
{
	//��갴������ƶ�
	if (m_moving && (event->buttons() && Qt::LeftButton) && (event->globalPos() - m_lastPos).manhattanLength() > QApplication::startDragDistance())
	{
		move(event->globalPos() - m_lastPos);
		m_lastPos = event->globalPos() - pos();
	}
	return QMainWindow::mouseMoveEvent(event);
}

void DOF_Teach::mouseReleaseEvent(QMouseEvent * event)
{
	//����ɿ�
	m_moving = false;//�ƶ�
}

void DOF_Teach::closeEvent(QCloseEvent * event)
{	
	//�ر������ں�����ʹ�ܽ����ϵİ�ť
	disconnect(dlg_current, &CurrentUi::closedSignal, this, &DOF_Teach::btn_Current_closed);
	disconnect(dlg_torgue, &TorgueUi::closedSignal, this, &DOF_Teach::btn_Torgue_closed);
	disconnect(dlg_velocity, &VelocityUi::closedSignal, this, &DOF_Teach::btn_Velocity_closed);
	ui.mdiArea->closeActiveSubWindow();

	//��һ�ֹرշ���
	//if (dlg_current != nullptr)
	//	delete dlg_current;
	//	//dlg_current->close();
	//if (dlg_torgue != nullptr)
	//	delete dlg_current;
	//	//dlg_torgue->close();
	//if (dlg_velocity != nullptr)
	//	delete dlg_current;
	//	//dlg_velocity->close();
	SaveLastPosition();
}

bool DOF_Teach::ReadLastPosition()
{
	QString str;
	QString curPath = QDir::currentPath();
	QString aFileName = curPath + "/LastPosition.txt";
	//QMessageBox::about(this, "", aFileName);
	QFile aFile(aFileName);
	if (!aFile.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		QMessageBox::about(this, "", "read fail");
		return false;
	}
	QTextStream aStream(&aFile);
	aStream.setAutoDetectUnicode(true);
	while (!aStream.atEnd())
	{
		str = aStream.readLine();
		QMessageBox::about(this, "", str);
	}
	aFile.close();
	return true;
}

bool DOF_Teach::SaveLastPosition()
{
	//���жϹر�ǰ���������ٱ���
	if (!(dynamixel_object->isconnected() && epos_object->isconnected()))
		return false;
	ui.PosView->setRowCount(++table_row);
	emit TeachReadPosSignal();

	QString curPath = QDir::currentPath();
	QString aFileName = curPath + "/LastPosition.txt";
	//QMessageBox::about(this, "", aFileName);
	QFile aFile(aFileName);
	if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::about(this, "", "save fail");
		return false;
	}
		
	QTextStream aStream(&aFile);//�ı���д��
	aStream.setAutoDetectUnicode(true);//�Զ����Unicode
	int i = ui.PosView->rowCount() - 1;
	{
		QTableWidgetItem* aItem = ui.PosView->item(i, 0);
		qreal node1pos = aItem->data(Qt::DisplayRole).toInt();
		aStream << node1pos;
		aStream << "\n";

		aItem = ui.PosView->item(i, 1);
		qreal node2pos = aItem->data(Qt::DisplayRole).toInt();
		aStream << node2pos;
		aStream << "\n";

		aItem = ui.PosView->item(i, 2);
		qreal node3pos = aItem->data(Qt::DisplayRole).toInt();
		aStream << node3pos;
		aStream << "\n";
	}
	aFile.close();
	//QMessageBox::about(this, "", "save");
	return true;
}
