#pragma once
#include <qobject.h>
#include <QMessageBox>
#include <QTimer>
#include <QEvent>
#include <QString>
#include <QCloseEvent>
#include "Dynamixel.h"
#include "EPOS.h"
#include "PosCheckThread.h"
#include "ActPosCheckThread.h"

#define M1_Node	1
#define M2_Node	2
#define M3_Node	3
#define DY_ID_A		1
#define DY_ID_B		2
#define DY_ID_C		3
#define DY_ID_D		4
#define ISABSOLUTE		1
#define ISRELATE		0
#define ISIMMEDIATELY	1
#define M1_Node_POS_THRESHOLD 1
#define M2_Node_POS_THRESHOLD 4
#define M3_Node_POS_THRESHOLD 300
class _7DOF_Arm :
	public QObject
{
	Q_OBJECT
public:
	_7DOF_Arm();
	~_7DOF_Arm();
	void init(EPOS *e_object, Dynamixel *dy_object);//以便在当前对象调用其他对象的方法
public:
	PosCheckThread pos_check_thread;//可选使用公共信号started()和finished()
	ActPosCheckThread act_pos_check_thread;
private:
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
	QTimer *read_Timer,*actuate_Timer;
	bool maxon_moving=false;
	bool isConnected = false;
	int actuate_value;
	//实时位置qc
	long    m_lActualPosition1;
	long    m_lActualPosition2;
	long    m_lActualPosition3;
	//运行前电机的位置qc
	long    m_lStartPosition1;
	long    m_lStartPosition2;
	long    m_lStartPosition3;
	//目标位置qc
	long    m_lTargetPosition1;
	long    m_lTargetPosition2;
	long    m_lTargetPosition3;
	//实时速度rpm
	long    m_lActuaSpeed1;
	long    m_lActuaSpeed2;
	long    m_lActuaSpeed3;
	//实时电流mA
	short   m_lActuaCurrent1;
	short   m_lActuaCurrent2;
	short   m_lActuaCurrent3;
	//电流阈值mA
	short   m_ThresholdCurrent1 = 4000;
	short   m_ThresholdCurrent2 = 4000;
	short   m_ThresholdCurrent3 = 5000;
	//实时转矩mNm
	long    m_lActuaTorgue1;
	long    m_lActuaTorgue2;
	long    m_lActuaTorgue3;//9.55f * 18 * m_Ccurrent/ m_CTargetVelocity
	//加速度rpm/s
	DWORD m_ulProfileAcceleration1;
	DWORD m_ulProfileAcceleration2;
	DWORD m_ulProfileAcceleration3;
	//减速度rpm/s
	DWORD m_ulProfileDeceleration1;
	DWORD m_ulProfileDeceleration2;
	DWORD m_ulProfileDeceleration3;
	//速度rpm
	DWORD m_ulProfileVelocity1;
	DWORD m_ulProfileVelocity2;
	DWORD m_ulProfileVelocity3;
	//最大速度rpm
	DWORD m_Amaxvelocity1=3500;
	DWORD m_Amaxvelocity2 = 3500;
	DWORD m_Amaxvelocity3 = 3500;
	//最大加速度rpm/s
	DWORD m_AmaxAcceleration1=65535;
	DWORD m_AmaxAcceleration2 = 65535;
	DWORD m_AmaxAcceleration3 = 65535;
	//最大跟随误差qc
	DWORD m_AmaxFollowingError1=2000;
	DWORD m_AmaxFollowingError2 = 2000;
	DWORD m_AmaxFollowingError3 = 2000;
	
	//Dynamixel
	int max_torque[4] = { 600,600,600,300 };
	int pgain[4] = { 32,32,32,32 };
	int loosen_torque[4]{28,25,10,8};
	int max_speed[4] = { 50, 50, 50, 100 };
	const int min_degree[4] = { 0, 5, 0, 0 };
	const int max_degree[4] = { 90, 90, 90, 90 };
	const int min_value[4] = { 1100, 150, 220, 200 };
	const int max_value[4] = { 2100, 1020, 500, 500 };
	uint16_t cw_pos_limit[4] = { 10, 1900, 10, 10 };
	uint16_t ccw_pos_limit[4] = { 3500, 3400, 4192, 4192 };
	int dxl_goal_position[4] = { 0, 0, 0, 0 };  // Goal position
	uint16_t dxl_present_position[4] = { 0, 0, 0, 0 };
public slots:
	void StartTimer();
	void StopTimer();
	void closeEvent(QCloseEvent *event);
	void Initialize();
	//motion parameter(速度模式下会重读设备覆盖以下某些参数;以下函数只会直接更新物理参数最大值)
	void MotionPara1(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void MotionPara2(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	void MotionPara3(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError);
	//dynamixel参数写入
	void ID1_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain);
	void ID2_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain);
	void ID3_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain);
	void ID4_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain);
	//位置读取
	void ReadAllPos();//绑定timer
	//Teach
	void ReadPosInTeach();
	void RecurringInTeach(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	void Move();
	void UrgencyHalt();
	//Maxon监测
	void EPOSCheck();//绑定timer
	void cb_CurrentThreshold_checked();
	void cb_CurrentThreshold_unchecked();
	//过载保护电流阈值
	void sb_ThresholdCurrent_valueChanged1(int value);
	void sb_ThresholdCurrent_valueChanged2(int value);
	void sb_ThresholdCurrent_valueChanged3(int value);
	//内驱力
	void cb_Actuate_checked();
	void cb_Actuate_unchecked();
	void AutoActuate();//绑定actuate_Timer
	void AutoActuateDone();
	void slider__Actuate_valueChanged(int value);
	//力矩
	void cb_Torque_checked();
	void cb_Torque_unchecked();
	//微调
	void btn_TuningUp1(int step, int velocity);
	void btn_TuningUp2(int step, int velocity);
	void btn_TuningUp3(int step, int velocity);
	void btn_TuningDown1(int step, int velocity);
	void btn_TuningDown2(int step, int velocity);
	void btn_TuningDown3(int step, int velocity);
	void dial_Tuning(int id,int value);	
	void dial_Init();
signals:
	//位置信息发送
	void PosSignal(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	void TeachPosSignal(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos);
	void Dynamixel_Dial_Init_Signal(uint16_t cw[4], uint16_t ccw[4], uint16_t current_value[4]);
};

