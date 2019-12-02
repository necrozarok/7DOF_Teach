#include "DOF_Teach.h"
#include "ConfigUi.h"
#include "ConnectUi.h"
#include "CurrentUi.h"
#include "Dynamixel.h"
#include "EPOS.h"
#include "_7DOF_Arm.h"
#include "PosCheckThread.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	ConnectUi *dlgLogin = new ConnectUi;
	ConfigUi *dlgConfig = new ConfigUi;
	Dynamixel *main_dy_obj = new Dynamixel;
	EPOS *main_epos_obj = new EPOS;
	_7DOF_Arm *main_7DOF_Arm_obj = new _7DOF_Arm;
	//指针形式调用Dy，EPOS对象的方法
	dlgLogin->init(main_epos_obj,main_dy_obj);
	dlgConfig->init(main_epos_obj, main_dy_obj);
	main_7DOF_Arm_obj->init(main_epos_obj, main_dy_obj);
	//数值关联ConfigUi to _7DOF_Arm
	QObject::connect(dlgConfig, &ConfigUi::SendMotionPara1, main_7DOF_Arm_obj, &_7DOF_Arm::MotionPara1);
	QObject::connect(dlgConfig, &ConfigUi::SendMotionPara2, main_7DOF_Arm_obj, &_7DOF_Arm::MotionPara2);
	QObject::connect(dlgConfig, &ConfigUi::SendMotionPara3, main_7DOF_Arm_obj, &_7DOF_Arm::MotionPara3);
	QObject::connect(dlgConfig, &ConfigUi::Send_ID1_Para, main_7DOF_Arm_obj, &_7DOF_Arm::ID1_Para_Write);
	QObject::connect(dlgConfig, &ConfigUi::Send_ID2_Para, main_7DOF_Arm_obj, &_7DOF_Arm::ID2_Para_Write);
	QObject::connect(dlgConfig, &ConfigUi::Send_ID3_Para, main_7DOF_Arm_obj, &_7DOF_Arm::ID3_Para_Write);
	QObject::connect(dlgConfig, &ConfigUi::Send_ID4_Para, main_7DOF_Arm_obj, &_7DOF_Arm::ID4_Para_Write);
	//ConnectUi to _7DOF_Arm
	QObject::connect(dlgLogin, &ConnectUi::Connected, main_7DOF_Arm_obj, &_7DOF_Arm::Initialize);
	//点击连接才显示主界面
	if (dlgLogin->exec() == QDialog::Accepted)
	{
		DOF_Teach teach_window;
		teach_window.init(main_epos_obj, main_dy_obj, dlgConfig, dlgLogin);		
		teach_window.show();
		//DOF_Teach to _7DOF_Arm
		QObject::connect(&teach_window, &DOF_Teach::TeachReadPosSignal, main_7DOF_Arm_obj,&_7DOF_Arm::ReadPosInTeach);
		QObject::connect(&teach_window, &DOF_Teach::TeachRecurringSignal, main_7DOF_Arm_obj, &_7DOF_Arm::RecurringInTeach);
		QObject::connect(&teach_window, &DOF_Teach::UrgencyHaltSignal, main_7DOF_Arm_obj, &_7DOF_Arm::UrgencyHalt);
		QObject::connect(&teach_window, &DOF_Teach::ActuateCheckedSignal, main_7DOF_Arm_obj, &_7DOF_Arm::cb_Actuate_checked);
		QObject::connect(&teach_window, &DOF_Teach::ActuateunCheckedSignal, main_7DOF_Arm_obj, &_7DOF_Arm::cb_Actuate_unchecked);
		QObject::connect(&teach_window, &DOF_Teach::TorquecheckedSignal, main_7DOF_Arm_obj, &_7DOF_Arm::cb_Torque_checked);
		QObject::connect(&teach_window, &DOF_Teach::TorqueuncheckedSignal, main_7DOF_Arm_obj, &_7DOF_Arm::cb_Torque_unchecked);
		//_7DOF_Arm to DOF_Teach
		QObject::connect(main_7DOF_Arm_obj, &_7DOF_Arm::PosSignal, &teach_window, &DOF_Teach::StatusBar_Show_Pos);
		QObject::connect(main_7DOF_Arm_obj, &_7DOF_Arm::TeachPosSignal, &teach_window, &DOF_Teach::btn_GetPos_clicked_Receive);
		QObject::connect(&main_7DOF_Arm_obj->pos_check_thread, &PosCheckThread::PosCheckDone, &teach_window, &DOF_Teach::SingleOrLoopRecurring);
		return a.exec();
	}
	else {
		return 0;
	}
}
