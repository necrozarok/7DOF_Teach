#pragma once

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QApplication>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QCheckBox.h>
#include <QMdiSubWindow>
#include "ui_DOF_Teach.h"
#include "ConfigUi.h"
#include "ConnectUi.h"
#include "CurrentUi.h"
#include "TorgueUi.h"
#include "VelocityUi.h"
#include "about.h"
#include "HelpUi.h"
#include "TuningButton.h"
#include "RecrodDB.h"
class DOF_Teach : public QMainWindow
{
	Q_OBJECT
public:
	DOF_Teach(QWidget *parent = Q_NULLPTR);
	~DOF_Teach();
	void init(EPOS *e_object, Dynamixel *dy_object, ConfigUi * dlg_config, ConnectUi * dlg_connect);//�Ա��ڵ�ǰ���������������ķ���	
private:
	Ui::DOF_TeachClass ui;
	//ui����
	RecrodDB *dlg_recordDB = nullptr;
	ConfigUi *dlg_config = nullptr;
	ConnectUi *dlg_connect = nullptr;
	CurrentUi *dlg_current = nullptr;
	TorgueUi  *dlg_torgue = nullptr;
	VelocityUi *dlg_velocity= nullptr;
	about *dlg_about = nullptr;
	HelpUi *dlg_help = nullptr;
	//�ŷ���������
	Dynamixel *dynamixel_object = nullptr;
	EPOS *epos_object = nullptr;

	bool m_moving = false;//�����Ƿ������������ƶ�
	QPoint m_lastPos;//��һ�����λ��

	bool isLoopRecurring = FALSE;
	bool isConnect = FALSE;
	int table_row = 0;
	int row_count;
	int column_count;
	int current_row=0;

public slots:
	void StatusBar_Show_Pos(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	bool saveDataAsStream(QString &aFileName);//����Ϊstm�ļ�
	bool saveBinaryFile(QString &aFileName);//����Ϊdat�ļ�
	bool openDataAsStream(QString &aFileName);//��stm�ļ�
	bool openBinaryFile(QString &aFileName);//��dat�ļ�
	//Action����
	void on_actionConfig_triggered();
	void on_actionConnect_triggered();
	void on_actionSave_triggered();
	void on_actionOpen_triggered();
	void on_actionAbout_triggered();
	void on_actionDemo_triggered();
	//���滬����
	void on_sliderActuate_valueChanged(int value);
	//��̬������ʾ
	void btn_Current_clicked();
	void btn_Current_closed();
	void btn_Torgue_clicked();
	void btn_Torgue_closed();
	void btn_Velocity_clicked();
	void btn_Velocity_closed();
	//ʾ�̲����ؼ�
	void btn_GetPos_clicked();
	void btn_GetPos_clicked_Receive(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	void btn_DeletePos_clicked();
	void btn_Recurring_clicked();//��������
	void btn_LoopRecurring_clicked();//ѭ������
	void SingleOrLoopRecurring();
	void Recurring_Halt_clicked();//��ͣ
	void Recurring_UrgencyHalt_clicked();
	//Option�ؼ�
	void cb_Actuate_checked();
	void cb_Relate_checked();
	void cb_Torgue_checked();	
	void cb_CurrentThreshold_checked();
	//�������ر����ؼ�
	void on_sbCurrentThreshold1_valueChanged(int i);
	void on_sbCurrentThreshold2_valueChanged(int i);
	void on_sbCurrentThreshold3_valueChanged(int i);
	//΢���ؼ�����
	void on_Node1_tuningUp_pressingSignal();
	void on_Node1_tuningDown_pressingSignal();
	void on_Node2_tuningUp_pressingSignal();
	void on_Node2_tuningDown_pressingSignal();
	void on_Node3_tuningUp_pressingSignal();
	void on_Node3_tuningDown_pressingSignal();
	void on_dial_ID1_sliderMoved(int value);
	void on_dial_ID2_sliderMoved(int value);
	void on_dial_ID3_sliderMoved(int value);
	void on_dial_ID4_sliderMoved(int value);
	void on_tabWidget_currentChanged(int page);
	void update_dial(uint16_t cw[4], uint16_t ccw[4], uint16_t current_value[4]);
	//���ݿ�
	void on_actionDB_triggered();
protected:
	//��������϶����ڵ�����¼�
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);	
	//�����˳��󱣴�����λ��(���ڳ���������豸λ��Ҳ�п��ܸı䣬�˲������岻��)�Լ��رն�̬����
	bool SaveLastPosition();
	void closeEvent(QCloseEvent *event);
	//����������ȡ����λ��
	bool ReadLastPosition();
signals:
	void InTeachPageSignal();
	void NoInTeachPageSignal();
	void TeachReadPosSignal();
	void TeachRecurringSignal(long node1_pos, long node2_pos, long node3_pos,int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	void UrgencyHaltSignal();
	void ActuateCheckedSignal();
	void ActuateunCheckedSignal();
	void ActuateSliderSignal(int value);
	void TorquecheckedSignal();
	void TorqueuncheckedSignal();
	void CurrentThresholdCheckedSignal();
	void CurrentThresholdUncheckedSignal();
	void CurrentThresholdValueChangedSignal1(int value);
	void CurrentThresholdValueChangedSignal2(int value);
	void CurrentThresholdValueChangedSignal3(int value);
	void TuningUp_Singnal1(int step, int velocity);
	void TuningUp_Singnal2(int step, int velocity);
	void TuningUp_Singnal3(int step, int velocity);
	void TuningDown_Singnal1(int step, int velocity);
	void TuningDown_Singnal2(int step, int velocity);
	void TuningDown_Singnal3(int step, int velocity);
	void Tuning_Dial_Singnal(int id, int value);
	void Tuning_Dial_Init_Singnal();
};
