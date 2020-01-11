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
	qryModel->setQuery("SELECT RecNo,Date,ActionCount,Discription FROM _7DOF ORDER BY RecNo");//设置数据表
	if ((qryModel->lastError().isValid()))//查询
	{
		QMessageBox::critical(this, "错误", "数据表查询错误，错误信息\n"
			+ qryModel->lastError().text(),
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	qryModel->setHeaderData(0, Qt::Horizontal, "记录号");
	qryModel->setHeaderData(1, Qt::Horizontal, "写入日期");
	qryModel->setHeaderData(2, Qt::Horizontal, "动作数");
	qryModel->setHeaderData(3, Qt::Horizontal, "描述");

	theSelection = new QItemSelectionModel(qryModel);//选择模型
	connect(theSelection, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
		this, SLOT(on_currentRowChanged(QModelIndex, QModelIndex)));
	ui->tableView->setModel(qryModel);//设置数据模型
	ui->tableView->setSelectionModel(theSelection);//设置选择模型

	//创建界面组件与数据模型的字段之间的映射
	dataMapper = new QDataWidgetMapper();
	dataMapper->setModel(qryModel);//设置数据模型
	dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
	//界面组件与qryModel的具体字段间联系
	dataMapper->addMapping(ui->RecNo, 0);
	dataMapper->addMapping(ui->dateEdit, 1);
	dataMapper->addMapping(ui->ActionCount, 2);
	dataMapper->addMapping(ui->RecDiscrption, 3);
	dataMapper->toFirst();//移动到首记录

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
	//刷新tableview当前选择行
	int index = dataMapper->currentIndex();
	QModelIndex curIndex = qryModel->index(index, 1);
	theSelection->clearSelection();//清空选择项
	theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
}

void RecrodDB::updateRecord(int recNo)
{
}

void RecrodDB::on_btnOpenDB_clicked()
{
	QString aFile = QFileDialog::getOpenFileName(this, "DB file select", "", "SQLite数据库(*.db *.db3)");
	if (aFile.isEmpty())
		return;
	DB = QSqlDatabase::addDatabase("QSQLITE");//添加 SQLITE 数据库驱动
	DB.setDatabaseName(aFile);//设置数据库名称
	if (!DB.open())
	{
		QMessageBox::warning(this, "错误", "打开数据库失败", QMessageBox::Ok, QMessageBox::NoButton);
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
	//query.exec("select * from _7DOF where RecNo=-1");//只查询字段信息
	//QSqlRecord curRec = query.record();//获取当前的空记录
	//curRec.setValue("RecNo", qryModel->rowCount() + 1000);

	query.prepare("INSERT INTO _7DOF (RecNo,Date,ActionCount,Discription,RecData) VALUES (:ID,:Date,:ActionCount,:Discription,:RecData)");
	QDateTime current_date_time = QDateTime::currentDateTime();
	query.bindValue(":Date", current_date_time);
	query.bindValue(":ActionCount", action_count);
	query.bindValue(":Discription", ui->RecDiscrption->toPlainText());

	//动作数据
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
	QMessageBox::critical(this, "错误", "数据\n" + QString::asprintf("%d %d %d %d",a, b,  c,  d ), QMessageBox::Ok, QMessageBox::NoButton);


	query.bindValue(":ID", qryModel->rowCount() + 2000);
	if (!query.exec())
		QMessageBox::critical(this, "错误", "插入记录错误\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
	else//重新执行查询语句
	{
		qryModel->setQuery("SELECT RecNo,Date,ActionCount,Discription FROM _7DOF ORDER BY RecNo");
		dataMapper->toLast();
		refreshTableView();
	}	
}

void RecrodDB::on_btnRead_clicked()
{
	int curRecNo = theSelection->currentIndex().row();
	QSqlRecord curRec = qryModel->record(curRecNo);//获取当前记录
	int RecNo = curRec.value("RecNo").toInt();

	QSqlQuery query;
	query.prepare("select RecData from _7DOF where RecNo = :ID");//查询RecNo为ID的动作描述
	query.bindValue(":ID", RecNo);
	if (!query.exec())
		QMessageBox::critical(this, "错误", "读取记录错误\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
	query.first();

	QVariant va = query.value("RecData");
	if (!va.isValid())//校验字段内容是否为空
	{
		QMessageBox::critical(this, "错误", "无数据\n" , QMessageBox::Ok, QMessageBox::NoButton);
		//测试代码
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
	int RecNo = curRec.value("RecNo").toInt();//获取动作编号
	QSqlQuery query;
	query.prepare("delete from _7DOF where RecNo=:ID");
	query.bindValue(":ID", RecNo);
	if (!query.exec())
		QMessageBox::critical(this, "错误", "删除记录错误\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
	else//重新执行查询语句,更新显示
	{
		qryModel->setQuery("SELECT RecNo,Date,ActionCount,Discription FROM _7DOF ORDER BY RecNo");
		dataMapper->toLast();
		refreshTableView();
		//qryModel->query().exec();//此方法无法更新显示
	}
}

void RecrodDB::on_currentRowChanged(const QModelIndex & current, const QModelIndex & previous)
{
	Q_UNUSED(previous);
	
	if (!current.isValid())
	{
		return;
	}
	
	dataMapper->setCurrentModelIndex(current);//更新数据映射行号
	bool first = (current.row() == 0);//是否首记录
	bool last = (current.row() == qryModel->rowCount() - 1);

	ui->actRecFirst->setEnabled(!first);
	ui->actRecPrevious->setEnabled(!first);
	ui->actRecNext->setEnabled(!last);
	ui->actRecLast->setEnabled(!last);

	int curRecNo = theSelection->currentIndex().row();
	QSqlRecord curRec = qryModel->record(curRecNo);//获取当前记录
	int RecNo = curRec.value("RecNo").toInt();

	QSqlQuery query;
	query.prepare("select Discription from _7DOF where RecNo = :ID");//查询RecNo为ID的动作描述
	query.bindValue(":ID", RecNo);
	if(!query.exec())
		QMessageBox::critical(this, "错误", "删除记录错误\n" + query.lastError().text(), QMessageBox::Ok, QMessageBox::NoButton);
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
