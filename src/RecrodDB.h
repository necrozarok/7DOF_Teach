#pragma once

#include <QDialog>
#include <QtSql>
#include <qdatawidgetmapper.h>
#include <QFileDialog>
#include <QMessageBox>
namespace Ui { class RecrodDB; };

class RecrodDB : public QDialog
{
	Q_OBJECT

public:
	RecrodDB(QWidget *parent = Q_NULLPTR);
	~RecrodDB();
	void ImportData(int action_count, QList<int> action_data);
	QList<int> OutData();
	int OutCount();
	bool IsRead();
private:
	Ui::RecrodDB *ui;
	QSqlDatabase DB;    //数据库连接
	QSqlQueryModel *qryModel;   //数据库模型
	QItemSelectionModel *theSelection;  //选择模型
	QDataWidgetMapper *dataMapper;  //数据映射
	int action_count;
	QList<int> action_data;
	bool is_read;
	void openTable();   //打开数据表
	void refreshTableView();   //获取字段名称，填充排序字段的comboBox
	void updateRecord(int recNo);//更新记录
private slots:
	void on_btnOpenDB_clicked();
	void on_btnCloseDB_clicked();
	void on_btnInsert_clicked();
	void on_btnRead_clicked();
	void on_btnDelete_clicked();
	void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
	void on_actRecFirst_clicked();
	void on_actRecPrevious_clicked();
	void on_actRecNext_clicked();
	void on_actRecLast_clicked();
private:
	void closeEvent(QCloseEvent *event);
};
