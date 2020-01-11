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
	__int8 m_bMode;//����ģʽflag
	//BOOL Isabsolute = true;
	BOOL m_oImmediately=true;//VCS_MoveToPosition����
	BOOL m_oInitialisation;//�豸����flag
	BOOL m_oUpdateActive;//�Զ�ˢ��flag
	DWORD m_ulErrorCode;//����flag
	HANDLE m_KeyHandle;//�豸�˿ھ��
	WORD m_usNodeId;//�ڵ�id
	QTimer *timer = nullptr;//ˢ�����ݶ�ʱ��
	QString m_strActiveMode;//����ģʽ����
	QString m_strNodeId;//�ڵ�id����
	int     m_oRadio = 0;//Sets state to relative or absolute movement,1 absolute 0 relative
	long    m_lActualValue;//����ʵʱλ��
	long    m_lStartPosition;//��������ǰ�����λ��
	long    m_lTargetPosition;//����׼�������Ŀ��λ��


	//������豸��ȡ�ĵ������
	DWORD m_ulProfileAcceleration = 1000;
	DWORD m_ulProfileDeceleration = 1000;
	DWORD m_ulProfileVelocity = 100;

	//�豸����������ֵ
	DWORD m_Amaxvelocity;
	DWORD m_AmaxAcceleration;
	DWORD m_AmaxFollowingError;
	
private:
	//�ɰ�ӿ�
	BOOL OpenDevice();//Opens communication interface
	BOOL RestoreEPOS();//Restores the old settings, stops the timer and delete the key handle
	void StopTimer();//Stops timer. Status will be displayed as disconnected
	void UpdateNodeIdString();//Converts node id to string
	BOOL UpdateStatus();//������ʾ
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
	//����
	BOOL OpenDeviceInConnectUi(char* DeviceName, char* ProtocolStackName, char* InterfaceName, char* PortName);
	BOOL isconnected();
	//д��
	void SetMaxFollowingError(WORD node,DWORD follow_error);
	void SetMaxProfileVelocity(WORD node, DWORD profile_velocity);
	void SetMaxAcceleration(WORD node, DWORD max_acceleration);
	bool SetRecorderParameter(WORD node, WORD value);
	//��ȡ
	long GetPosition(WORD NodeID);
	long GetVelocityIs(WORD NodeID);
	short GetCurrentIsAveraged(WORD NodeID);
	WORD GetRecorderParameter(WORD NodeID);
	//����
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

