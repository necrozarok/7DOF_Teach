#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QApplication>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QCheckBox.h>
#include "ui_DOF_Teach.h"
#include "ConfigUi.h"
#include "ConnectUi.h"
#include "CurrentUi.h"
#include "TorgueUi.h"
#include "VelocityUi.h"
class DOF_Teach : public QMainWindow
{
	Q_OBJECT
public:
	DOF_Teach(QWidget *parent = Q_NULLPTR);
	void init(EPOS *e_object, Dynamixel *dy_object, ConfigUi * dlg_config, ConnectUi * dlg_connect);//�Ա��ڵ�ǰ���������������ķ���	
private:
	Ui::DOF_TeachClass ui;
	ConfigUi *dlg_config = nullptr;
	ConnectUi *dlg_connect = nullptr;
	CurrentUi *dlg_current = nullptr;
	TorgueUi  *dlg_torgue = nullptr;
	VelocityUi *dlg_velocity= nullptr;
	Dynamixel *dynamixel_object = nullptr;
	EPOS *epos_object = nullptr;
	bool m_moving = false;//�����Ƿ������������ƶ�
	QPoint m_lastPos;//��һ�����λ��

	bool isLoopRecurring = FALSE;
	int table_row = 0;
	int row_count;
	int column_count;
	int current_row=0;
public slots:
	void StatusBar_Show_Pos(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	bool saveDataAsStream(QString &aFileName);//����Ϊstm�ļ�
	bool openDataAsStream(QString &aFileName);//��stm�ļ�
	void on_actionConfig_triggered();
	void on_actionConnect_triggered();
	void on_actionSave_triggered();
	void on_actionOpen_triggered();
	void btn_Current_clicked();
	void btn_Current_closed();
	void btn_Torgue_clicked();
	void btn_Torgue_closed();
	void btn_Velocity_clicked();
	void btn_Velocity_closed();
	void btn_GetPos_clicked();
	void btn_GetPos_clicked_Receive(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	void btn_DeletePos_clicked();
	void btn_Recurring_clicked();//��������
	void btn_LoopRecurring_clicked();//ѭ������
	void SingleOrLoopRecurring();
	void Recurring_Halt_clicked();//��ͣ
	void Recurring_UrgencyHalt_clicked();
	void cb_Actuate_checked();
	void cb_Relate_checked();
	void cb_Torgue_checked();

	
protected:
	//��������϶����ڵ�����¼�
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
signals:
	void InTeachPageSignal();
	void NoInTeachPageSignal();
	void TeachReadPosSignal();
	void TeachRecurringSignal(long node1_pos, long node2_pos, long node3_pos,int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	void UrgencyHaltSignal();
	void ActuateCheckedSignal();
	void ActuateunCheckedSignal();
	void TorquecheckedSignal();
	void TorqueuncheckedSignal();
};
