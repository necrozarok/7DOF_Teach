#include "DOF_Teach.h"
#pragma execution_character_set("utf-8")
DOF_Teach::DOF_Teach(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//ȥ��������
	this->setWindowFlags(Qt::FramelessWindowHint);
	//pos���ʼ��
	//ui.PosView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.PosView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//ui.PosView->setRowCount(table_row);
	//ui.PosView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");//ͨ����ʽ�����ñ�ͷ����ɫ
	//connect(ui.PosView, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(getItem(QTreeWidgetItem*, int)));//��ȡѡ�еĵ�Ԫ������
	//action��������ʵ���԰󶨣�
	//connect(ui.actionConfig, &QAction::triggered, this, &DOF_Teach::on_actionConfig_triggered);
	//connect(ui.actionConnect, &QAction::triggered, this, &DOF_Teach::on_actionConnect_triggered);
	//connect(ui.actionSave, &QAction::triggered, this, &DOF_Teach::on_actionSave_triggered);
	//connect(ui.actionOpen, &QAction::triggered, this, &DOF_Teach::on_actionOpen_triggered);
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

bool DOF_Teach::openDataAsStream(QString & aFileName)
{
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
	//��QTԤ������뱣���ļ�
	QString curPath = QDir::currentPath();
	QString aFileName = QFileDialog::getSaveFileName(this, "ѡ�񱣴��ļ�", curPath, "QtԤ������������ļ�(*.stm)");
	if (aFileName.isEmpty())
		return;
	if (saveDataAsStream(aFileName))
		QMessageBox::information(this, "��ʾ��Ϣ", "�ļ��Ѿ��ɹ����棡");
}

void DOF_Teach::on_actionOpen_triggered()
{
	//��stm�ļ�
	QString curPath = QDir::currentPath();
	QString aFileName = QFileDialog::getOpenFileName(this, "��һ���ļ�", curPath, "QtԤ������������ļ�(*.stm)");
	if (aFileName.isEmpty())
		return;
	if (openDataAsStream(aFileName))
		QMessageBox::information(this, "��ʾ��Ϣ", "�ļ��Ѿ��򿪣�");
}

void DOF_Teach::btn_Current_clicked()
{
	if (epos_object == nullptr&&dynamixel_object == nullptr)
	{
		QMessageBox::about(NULL, "warning", "No connection!");
		return;
	}
	ui.btnCurrent->setEnabled(0);
	dlg_current = new CurrentUi();
	dlg_current->setAttribute(Qt::WA_DeleteOnClose);//�Ի���ر�ʱ�Զ�ɾ��
	Qt::WindowFlags flags = dlg_current->windowFlags();//��ȡ����flags
	dlg_current->setWindowFlags(flags | Qt::WindowStaysOnBottomHint);
	dlg_current->init(epos_object, dynamixel_object);
	dlg_current->show();//��ģ̬
	connect(dlg_current, &CurrentUi::closedSignal, this, &DOF_Teach::btn_Current_closed);
}

void DOF_Teach::btn_Current_closed()
{
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
	dlg_torgue = new TorgueUi();
	dlg_torgue->setAttribute(Qt::WA_DeleteOnClose);//�Ի���ر�ʱ�Զ�ɾ��
	Qt::WindowFlags flags = dlg_torgue->windowFlags();//��ȡ����flags
	dlg_torgue->setWindowFlags(flags | Qt::WindowStaysOnBottomHint);
	dlg_torgue->init(epos_object, dynamixel_object);
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
	dlg_velocity = new VelocityUi();
	dlg_velocity->setAttribute(Qt::WA_DeleteOnClose);//�Ի���ر�ʱ�Զ�ɾ��
	Qt::WindowFlags flags = dlg_velocity->windowFlags();//��ȡ����flags
	dlg_velocity->setWindowFlags(flags | Qt::WindowStaysOnBottomHint);
	dlg_velocity->init(epos_object, dynamixel_object);
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
