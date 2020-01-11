#include "RecrodDB.h"
#include "ui_RecrodDB.h"
#pragma execution_character_set("utf-8")
RecrodDB::RecrodDB(QWidget *parent)
	: QDialog(parent)
{
	is_read = false;
	ui = new Ui::RecrodDB();
	ui->setupUi(this);
	ui->btnCloseDB->setEnabled(false);
	ui->btnInsert->setEnabled(false);
	ui->btnRead->setEnabled(false);
	ui->btnDelete->setEnabled(false);
	ui->actRecFirst->setEnabled(false);
	ui->actRecPrevious->setEnabled(false);
	ui->actRecNext->setEnabled(false);
	ui->actRecLast->setEnabled(false);
}

RecrodDB::~RecrodDB()
{
	delete ui;
}

void RecrodDB::ImportData(int action_count, QList<int> action_data)
{
	this->action_count = action_count;
	this->action_data = action_data;
}

QList<int> RecrodDB::OutData()
{
	return action_data;
}

int RecrodDB::OutCount()
{
	return action_count;
}

bool RecrodDB::IsRead()
{
	return is_read;
}

void RecrodDB::openTable()
{
	qryModel = new QSqlQueryModel(this);
	qryModel->setQuery("SELECT RecNo,Date,ActionCount,Discription FROM _7DOF ORDER BY RecNo");//�������ݱ�
	if ((qryModel->lastError().isValid()))//��ѯ
	{
		QMessageBox::critical(this, "����", "���ݱ��ѯ���󣬴�����Ϣ\n"
			+ qryModel->lastError().text(),
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	qryModel->setHeaderData(0, Qt::Horizontal, "��¼��");
	qryModel->setHeaderData(1, Qt::Horizontal, "д������");
	qryModel->setHeaderData(2, Qt::Horizontal, "������");
	qryModel->setHeaderData(3, Qt::Horizontal, "����");

	theSelection = new QItemSelectionModel(qryModel);//ѡ��ģ��
	connect(theSelection, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
		this, SLOT(on_currentRowChanged(QModelIndex, QModelIndex)));
	ui->tableView->setModel(qryModel);//��������ģ��
	ui->tableView->setSelectionModel(theSelection);//����ѡ��ģ��

	//�����������������ģ�͵��ֶ�֮���ӳ��
	dataMapper = new QDataWidgetMapper();
	dataMapper->setModel(qryModel);//��������ģ��
	dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
	//���������qryModel�ľ����ֶμ���ϵ
	dataMapper->addMapping(ui->RecNo, 0);
	dataMapper->addMapping(ui->dateEdit, 1);
	dataMapper->addMapping(ui->ActionCount, 2);
	dataMapper->addMapping(ui->RecDiscrption, 3);
	dataMapper->toFirst();//�ƶ����׼�¼

	ui->btnOpenDB->setEnabled(false);
	ui->btnCloseDB->setEnabled(true);
	ui->btnInsert->setEnabled(true);
	ui->btnRead->setEnabled(true);
	ui->btnDelete->setEnabled(true);

	ui->actRecFirst->setEnabled(true);
	ui->actRecPrevious->setEnabled(true);
	ui->actRecNext->setEnabled(true);
	ui->actRecLast->setEnabled(true);
}

void RecrodDB::refreshTableView()
{
	//ˢ��tableview��ǰѡ����
	int index = dataMapper->currentIndex();
	QModelIndex curIndex = qryModel->index(index, 1);
	theSelection->clearSelection();//���ѡ����
	theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
}

void RecrodDB::updateRecord(int recNo)
{
}

void RecrodDB::on_btnOpenDB_clicked()
{
	QString aFile = QFileDialog::getOpenFileName(this, "DB file select", "", "SQLite���ݿ�(*.db *.db3)");
	if (aFile.isEmpty())
		return;
	DB = QSqlDatabase::addDatabase("QSQLITE");//��� SQLITE ���ݿ�����
	DB.setDatabaseName(aFile);//�������ݿ�����
	if (!DB.open())
	{
		QMessageBox::warning(this, "����", "�����ݿ�ʧ��", QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	openTable();
}

void RecrodDB::on_btnCloseDB_clicked()
{
	DB.close();
	qryModel->clear();

	ui->btnOpenDB->setEnabled(true);
	ui->btnCloseDB->setEnabled(false);
	ui->btnInsert->setEnabled(false);
	ui->btnRead->setEnabled(false);
	ui->btnDelete->setEnabled(false);
	ui->actRecFirst->setEnabled(false);
	ui->actRecPrevious->setEnabled(false);
	ui->actRecNext->setEnabled(false);
	ui->actRecLast->setEnabled(false);
}

void RecrodDB::on_btnInsert_clicked()
{
	QSqlQuery query;
	//query.exec("select * from _7DOF where RecNo=-1");//ֻ��ѯ�ֶ���Ϣ
	//QSqlRecord curRec = query.record();//��ȡ��ǰ�Ŀռ�¼
	//curRec.setValue("RecNo", qryModel->rowCount() + 1000);

	query.prepare("INSERT INTO _7DOF (RecNo,Date,ActionCount,Discription,RecData) VALUES (:ID,:Date,:ActionCount,:Discription,:RecData)");
	QDateTime current_date_time = QDateTime::currentDateTime();
	query.bindValue(":Date", current_date_time);
	query.bindValue(":ActionCount", action_count);
	query.bindValue(":Discription", ui->RecDiscrption->toPlainText());

	//��������
	QByteArray action_data_byte;
	int ac_data;
	QDataStream stream(&action_data_byte, QIODevice::ReadWrite);//WriteOnly
	stream.setVersion(QDataStream::Qt_5_9); 
	stream << action_count;
	foreach(ac_data, action_data)
		stream << ac_data;
	query.bindValue(":RecData", action_data_byte);
	int a, b, c, d;
	stream >> a >> b >> c >> d;
	QMessageBox::critical(this, "����", "����\n" + QString::asprintf("%d %d %d %d",a, b,  c,  d ), QMessageBox::Ok, QMessageBox::NoButton);


	query.bindValue(":ID", qryModel->rowCount() + 2000);
	if (!query.exec())
		QMessageBox::critical(this, "����", "�����¼����\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
	else//����ִ�в�ѯ���
	{
		qryModel->setQuery("SELECT RecNo,Date,ActionCount,Discription FROM _7DOF ORDER BY RecNo");
		dataMapper->toLast();
		refreshTableView();
	}	
}

void RecrodDB::on_btnRead_clicked()
{
	int curRecNo = theSelection->currentIndex().row();
	QSqlRecord curRec = qryModel->record(curRecNo);//��ȡ��ǰ��¼
	int RecNo = curRec.value("RecNo").toInt();

	QSqlQuery query;
	query.prepare("select RecData from _7DOF where RecNo = :ID");//��ѯRecNoΪID�Ķ�������
	query.bindValue(":ID", RecNo);
	if (!query.exec())
		QMessageBox::critical(this, "����", "��ȡ��¼����\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
	query.first();

	QVariant va = query.value("RecData");
	if (!va.isValid())//У���ֶ������Ƿ�Ϊ��
	{
		QMessageBox::critical(this, "����", "������\n" , QMessageBox::Ok, QMessageBox::NoButton);
		//���Դ���
		/*action_count = 1;
		action_data << 1 << 2 << 3 << 4 << 5 << 6 << 7;
		emit accept();
		is_read = true;*/
		return;
	}
	else {
		QByteArray action_data_byte=va.toByteArray();
		QDataStream stream(&action_data_byte, QIODevice::ReadOnly);
		stream.setVersion(QDataStream::Qt_5_9);
		action_data.clear();
		stream >> action_count;
		for (int i = 0; i < action_count * 7; i++)
			stream >> action_data;
		emit accept();
	}
}

void RecrodDB::on_btnDelete_clicked()
{
	int curRecNo = theSelection->currentIndex().row();
	QSqlRecord curRec = qryModel->record(curRecNo);
	if (curRec.isEmpty())
		return;
	int RecNo = curRec.value("RecNo").toInt();//��ȡ�������
	QSqlQuery query;
	query.prepare("delete from _7DOF where RecNo=:ID");
	query.bindValue(":ID", RecNo);
	if (!query.exec())
		QMessageBox::critical(this, "����", "ɾ����¼����\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
	else//����ִ�в�ѯ���,������ʾ
	{
		qryModel->setQuery("SELECT RecNo,Date,ActionCount,Discription FROM _7DOF ORDER BY RecNo");
		dataMapper->toLast();
		refreshTableView();
		//qryModel->query().exec();//�˷����޷�������ʾ
	}
}

void RecrodDB::on_currentRowChanged(const QModelIndex & current, const QModelIndex & previous)
{
	Q_UNUSED(previous);
	
	if (!current.isValid())
	{
		return;
	}
	
	dataMapper->setCurrentModelIndex(current);//��������ӳ���к�
	bool first = (current.row() == 0);//�Ƿ��׼�¼
	bool last = (current.row() == qryModel->rowCount() - 1);

	ui->actRecFirst->setEnabled(!first);
	ui->actRecPrevious->setEnabled(!first);
	ui->actRecNext->setEnabled(!last);
	ui->actRecLast->setEnabled(!last);

	int curRecNo = theSelection->currentIndex().row();
	QSqlRecord curRec = qryModel->record(curRecNo);//��ȡ��ǰ��¼
	int RecNo = curRec.value("RecNo").toInt();

	QSqlQuery query;
	query.prepare("select Discription from _7DOF where RecNo = :ID");//��ѯRecNoΪID�Ķ�������
	query.bindValue(":ID", RecNo);
	if(!query.exec())
		QMessageBox::critical(this, "����", "ɾ����¼����\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
	query.first();
	
	QVariant va = query.value("Discription");
	if (!va.isValid())
		ui->RecDiscrption->clear();
	else {
		QString data = va.toString();
		ui->RecDiscrption->setPlainText(data);
	}
}

void RecrodDB::on_actRecFirst_clicked()
{
	dataMapper->toFirst();
	refreshTableView();
}

void RecrodDB::on_actRecPrevious_clicked()
{
	dataMapper->toPrevious();
	refreshTableView();
}

void RecrodDB::on_actRecNext_clicked()
{
	dataMapper->toNext();
	refreshTableView();
}

void RecrodDB::on_actRecLast_clicked()
{
	dataMapper->toLast();
	refreshTableView();
}

void RecrodDB::closeEvent(QCloseEvent * event)
{
	if(!DB.isOpen())
		DB.close();
}
