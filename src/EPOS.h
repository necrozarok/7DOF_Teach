#pragma once
#include <qobject.h>
#include <QMessageBox>
#include <QTimer>
#include "Definitions.h"
#define PI 3.1415926
#define MINUS 0.0001
#define USR_BAUDRATE 1000000
#define USR_TIMEOUT 500
class EPOS : public QObject
{
	Q_OBJECT
private:
	__int8 m_bMode;//操作模式flag
	//BOOL Isabsolute = true;
	BOOL m_oImmediately=true;//VCS_MoveToPosition参数
	BOOL m_oInitialisation;//设备开启flag
	BOOL m_oUpdateActive;//自动刷新flag
	DWORD m_ulErrorCode;//错误flag
	HANDLE m_KeyHandle;//设备端口句柄
	WORD m_usNodeId;//节点id
	QTimer *timer = nullptr;//刷新数据定时器
	QString m_strActiveMode;//操作模式名称
	QString m_strNodeId;//节点id名称
	int     m_oRadio = 0;//Sets state to relative or absolute movement,1 absolute 0 relative
	long    m_lActualValue;//运行实时位置
	long    m_lStartPosition;//保存运行前电机的位置
	long    m_lTargetPosition;//保存准备到达的目标位置


	//保存从设备读取的电机参数
	DWORD m_ulProfileAcceleration = 1000;
	DWORD m_ulProfileDeceleration = 1000;
	DWORD m_ulProfileVelocity = 100;

	//设备物理参数最大值
	DWORD m_Amaxvelocity;
	DWORD m_AmaxAcceleration;
	DWORD m_AmaxFollowingError;
	
private:
	//旧版接口
	BOOL OpenDevice();//Opens communication interface
	BOOL RestoreEPOS();//Restores the old settings, stops the timer and delete the key handle
	void StopTimer();//Stops timer. Status will be displayed as disconnected
	void UpdateNodeIdString();//Converts node id to string
	BOOL UpdateStatus();//更新显示
signals:
	void RadioSignal(BOOL radio);//1 absolute 0 relative
	void EnableSignal(BOOL enable);//1 inactivate Setting Enable, activate Disable Move Halt;0 activate Setting Enable, inactivate Disable Move Halt
	void UnconnectedSignal();
	void StartPosSignal(long Pos);
	void ActualPosSignal(long Pos);
	void OperateModeSignal(QString mod);
public:
	EPOS();
	~EPOS();
	BOOL ShowErrorInformation(DWORD p_ulErrorCode);//Shows a message box with error description of the input error code
	BOOL ShowErrorInformation();
	//连接
	BOOL OpenDeviceInConnectUi(char* DeviceName, char* ProtocolStackName, char* InterfaceName, char* PortName);
	BOOL isconnected();
	//写入
	void SetMaxFollowingError(WORD node,DWORD follow_error);
	void SetMaxProfileVelocity(WORD node, DWORD profile_velocity);
	void SetMaxAcceleration(WORD node, DWORD max_acceleration);
	bool SetRecorderParameter(WORD node, WORD value);
	//读取
	long GetPosition(WORD NodeID);
	long GetVelocityIs(WORD NodeID);
	short GetCurrentIsAveraged(WORD NodeID);
	WORD GetRecorderParameter(WORD NodeID);
	//操作
	void ShowChannelDataDlg(WORD NodeID);
	bool ClearFault(WORD NodeID);
	void Disable(WORD NodeID);
	void Enable(WORD NodeID);
	void Move_Position_Mode(
		WORD NodeID, 
		DWORD ProfileVelocity, 
		DWORD ProfileAcceleration,
		DWORD ProfileDeceleration, 
		long StartPosition,
		long TargetPosition,
		long ActualValue,
		bool is_absolute,
		bool is_immediately
	);
	bool HaltVelocityMovement(WORD NodeID);
	bool HaltPositionMovement(WORD NodeID);
	bool SetDisableState(WORD NodeID);
};

