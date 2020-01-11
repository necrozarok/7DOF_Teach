#pragma once

#include <QDialog>
#include <QMessageBox>
#include <QMainWindow> 
#include "Dynamixel.h"
#include "EPOS.h"
namespace Ui { class ConfigUi; };

class ConfigUi : public QDialog
{
	Q_OBJECT

public:
	ConfigUi(QWidget *parent = Q_NULLPTR);
	~ConfigUi();
	void init(EPOS *e_object, Dynamixel *dy_object);//以便在当前对象调用其他对象的方法

private:
	Ui::ConfigUi *ui;
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
private slots:
	//maxon
	void Node1MotionPara_Clicked();
	void Node2MotionPara_Clicked();
	void Node3MotionPara_Clicked();
	//dynamixel
	void ID1_Para_Write_clicked();
	void ID2_Para_Write_clicked();
	void ID3_Para_Write_clicked();
	void ID4_Para_Write_clicked();

signals:
	//maxon
	void SendMotionPara1(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void SendMotionPara2(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void SendMotionPara3(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	//dynamixel
	void Send_ID1_Para(int cw, int ccw, int velocity, int torque, int p_gain);
	void Send_ID2_Para(int cw, int ccw, int velocity, int torque, int p_gain);
	void Send_ID3_Para(int cw, int ccw, int velocity, int torque, int p_gain);
	void Send_ID4_Para(int cw, int ccw, int velocity, int torque, int p_gain);
};
