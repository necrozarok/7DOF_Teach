#include "EPOS.h"



BOOL EPOS::OpenDevice()
{
	HANDLE hNewKeyHandle;
	//Close Previous Device
	if (m_KeyHandle)
	{
		if (m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
		m_KeyHandle = 0;
	}
	hNewKeyHandle = VCS_OpenDeviceDlg(&m_ulErrorCode);
	if (hNewKeyHandle)
	{
		m_KeyHandle = hNewKeyHandle;

		//Clear Error History
		if (VCS_ClearFault(m_KeyHandle, m_usNodeId, &m_ulErrorCode))
		{
			//Read Operation Mode
			if (VCS_GetOperationMode(m_KeyHandle, m_usNodeId, &m_bMode, &m_ulErrorCode))
			{
				//Read Position Profile Objects
				if (VCS_GetPositionProfile(m_KeyHandle, m_usNodeId, &m_ulProfileVelocity, &m_ulProfileAcceleration, &m_ulProfileDeceleration, &m_ulErrorCode))
				{
					//Write Profile Position Mode
					if (VCS_SetOperationMode(m_KeyHandle, m_usNodeId, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode))
					{
						//Write Profile Position Objects
						if (VCS_SetPositionProfile(m_KeyHandle, m_usNodeId, 100, 1000, 1000, &m_ulErrorCode))
						{
							//Read Actual Position
							if (VCS_GetPositionIs(m_KeyHandle, m_usNodeId, &m_lStartPosition, &m_ulErrorCode))
							{
								m_oInitialisation = TRUE;
								timer = new QTimer(this);
								timer->start(1000);
								connect(timer, SIGNAL(timeout()), this, SLOT(UpdateStatus()));
								m_oUpdateActive = TRUE;
								QMessageBox::about(NULL, "Information", QString::fromLocal8Bit("已开启定时器"));
								return TRUE;
							}
						}
					}
				}
			}
		}
		ShowErrorInformation(m_ulErrorCode);
	}
	else
	{
		m_oInitialisation = FALSE;
		QMessageBox::about(NULL, "Information", "Can't open device!");
		return FALSE;
	}
}

BOOL EPOS::OpenDeviceInConnectUi(char* DeviceName,char* ProtocolStackName, char* InterfaceName, char* PortName)
{
	HANDLE hNewKeyHandle;
	//Close Previous Device
	if (m_KeyHandle)
	{
		if (m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
		m_KeyHandle = 0;
	}
	//connect
	hNewKeyHandle = VCS_OpenDevice("EPOS2", ProtocolStackName, InterfaceName, PortName, &m_ulErrorCode);;
	if (hNewKeyHandle)
	{
		m_KeyHandle = hNewKeyHandle;
		m_oInitialisation = TRUE;
		//Set Protocol Stack Settings
		if (VCS_SetProtocolStackSettings(hNewKeyHandle, USR_BAUDRATE, USR_TIMEOUT, &m_ulErrorCode))
		{	
			return TRUE;
		}
	}
	ShowErrorInformation(m_ulErrorCode);
	return FALSE;
}

BOOL EPOS::isconnected()
{
	return !(m_KeyHandle==nullptr);
}

BOOL EPOS::RestoreEPOS()
{
	//Restore Settings
	if (m_oInitialisation)
	{
		VCS_SetDisableState(m_KeyHandle, m_usNodeId, &m_ulErrorCode);
		VCS_SetOperationMode(m_KeyHandle, m_usNodeId, m_bMode, &m_ulErrorCode);
		VCS_SetPositionProfile(m_KeyHandle, m_usNodeId, m_ulProfileVelocity, m_ulProfileAcceleration, m_ulProfileDeceleration, &m_ulErrorCode);
	}
	//Stop Updating
	StopTimer();
	//Close Device
	if (m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
	return TRUE;
}

BOOL EPOS::ShowErrorInformation(DWORD p_ulErrorCode)
{
	char* pStrErrorInfo;
	QString strDescription;

	if ((pStrErrorInfo = (char*)malloc(100)) == NULL)
	{
		QMessageBox::about(NULL, "Information", "Not enough memory to allocate buffer for error information string!");
		return FALSE;
	}

	if (VCS_GetErrorInfo(p_ulErrorCode, pStrErrorInfo, 100))
	{
		strDescription = pStrErrorInfo;
		QMessageBox::about(NULL, "Information", strDescription);
		free(pStrErrorInfo);
		return TRUE;
	}
	else
	{
		free(pStrErrorInfo);
		QMessageBox::about(NULL, "Information", "Error information can't be read!");
		return FALSE;
	}
}

BOOL EPOS::ShowErrorInformation()
{
	char* pStrErrorInfo;
	QString strDescription;

	if ((pStrErrorInfo = (char*)malloc(100)) == NULL)
	{
		QMessageBox::about(NULL, "Information", "Not enough memory to allocate buffer for error information string!");
		return FALSE;
	}

	if (VCS_GetErrorInfo(m_ulErrorCode, pStrErrorInfo, 100))
	{
		strDescription = pStrErrorInfo;
		QMessageBox::about(NULL, "Information", strDescription);
		free(pStrErrorInfo);
		return TRUE;
	}
	else
	{
		free(pStrErrorInfo);
		QMessageBox::about(NULL, "Information", "Error information can't be read!");
		return FALSE;
	}
}

void EPOS::StopTimer()
{
	if (timer != NULL)
		if (timer->isActive()) {
			timer->stop();
		}
	m_oUpdateActive = FALSE;
	UpdateStatus();
}

void EPOS::UpdateNodeIdString()
{
	const size_t size(3);
	char strNodeId[size];
	_itoa(m_usNodeId, strNodeId, 10);
	m_strNodeId = strNodeId;
}

BOOL EPOS::UpdateStatus()
{
	//QMessageBox::about(NULL, "Information", "UpdateStatus");
	BOOL oEnable = TRUE;
	BOOL oResult = m_oUpdateActive;
	if (m_oRadio == 0)
	{
		emit RadioSignal(0);
		//m_Move.SetWindowText("&Move Relative");
	}
	else
	{
		emit RadioSignal(1);
		//m_Move.SetWindowText("&Move Absolute");
	}

	if (oResult)
	{
		oResult = VCS_GetOperationMode(m_KeyHandle, m_usNodeId, &m_bMode, &m_ulErrorCode);
		if (oResult)
		{
			switch (m_bMode)
			{
			case -6: m_strActiveMode = "Step/Direction Mode"; break;
			case -5: m_strActiveMode = "Master Encoder Mode"; break;
			case -3: m_strActiveMode = "Current Mode"; break;
			case -2: m_strActiveMode = "Velocity Mode"; break;
			case -1: m_strActiveMode = "Position Mode"; break;
			case 1: m_strActiveMode = "Profile Position Mode"; break;
			case 3: m_strActiveMode = "Profile Velocity Mode"; break;
			case 6: m_strActiveMode = "Homing Mode"; break;
			case 7: m_strActiveMode = "Interpolated Position Mode"; break;
			default: m_strActiveMode = "Unknown Mode";
			}
		}
		else
		{
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			m_strActiveMode = "Unknown Mode";
		}
	}
	else
	{
		m_strActiveMode = "Unknown Mode";
	}
	emit OperateModeSignal(m_strActiveMode);
	if (oResult)
	{
		oResult = VCS_GetEnableState(m_KeyHandle, m_usNodeId, &oEnable, &m_ulErrorCode);//oEnable 1: Device enabled 0: Device not enabled
		if (oResult)
		{
			emit EnableSignal(oEnable);
			//QMessageBox::about(NULL, "Information", "can recive enable state");
			/*m_DeviceSettings.EnableWindow(!oEnable);
			m_Enable.EnableWindow(!oEnable);
			m_Disable.EnableWindow(oEnable);
			m_Move.EnableWindow(oEnable);
			m_Halt.EnableWindow(oEnable);*/
		}
		else
		{
			QMessageBox::about(NULL, "Information", "cant recive enable state");
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			emit EnableSignal(!oEnable);
			/*m_DeviceSettings.EnableWindow(oEnable);
			m_Enable.EnableWindow(oEnable);
			m_Disable.EnableWindow(!oEnable);
			m_Move.EnableWindow(!oEnable);
			m_Halt.EnableWindow(!oEnable);*/
		}
	}
	else
	{
		emit UnconnectedSignal();
		/*m_DeviceSettings.EnableWindow(oEnable);
		m_Enable.EnableWindow(!oEnable);
		m_Disable.EnableWindow(!oEnable);
		m_Move.EnableWindow(!oEnable);
		m_Halt.EnableWindow(!oEnable);*/
	}
	if (oResult)
	{
		oResult = VCS_GetPositionIs(m_KeyHandle, m_usNodeId, &m_lActualValue, &m_ulErrorCode);
		if (!oResult)
		{
			StopTimer();
			ShowErrorInformation(m_ulErrorCode);
			m_lActualValue = 0;
			m_lStartPosition = 0;
		}
	}
	else
	{
		m_lActualValue = 0;
		m_lStartPosition = 0;
	}
	emit ActualPosSignal(m_lActualValue);


	return oResult;
}

EPOS::EPOS()
{
	//Settings
	m_oImmediately = TRUE;
	m_oUpdateActive = FALSE;
	m_usNodeId = 1;
}


EPOS::~EPOS()
{
	//Close Device
	if (m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);

	if (timer != nullptr)
		delete timer;
	timer = nullptr;
}

void EPOS::SetMaxFollowingError(WORD node, DWORD follow_error)
{
	VCS_SetMaxFollowingError(m_KeyHandle, node, follow_error, &m_ulErrorCode);
}

void EPOS::SetMaxProfileVelocity(WORD node, DWORD profile_velocity)
{
	VCS_SetMaxProfileVelocity(m_KeyHandle, node, profile_velocity, &m_ulErrorCode);
}

void EPOS::SetMaxAcceleration(WORD node, DWORD max_acceleration)
{
	VCS_SetMaxAcceleration(m_KeyHandle, node, max_acceleration, &m_ulErrorCode);
}

long EPOS::GetPosition(WORD NodeID)
{
	long _pos = 0;
	if (VCS_GetPositionIs(m_KeyHandle, NodeID, &_pos, &m_ulErrorCode))
		return _pos;
}

long EPOS::GetVelocityIs(WORD NodeID)
{
	long _vel = 0;
	if (VCS_GetVelocityIs(m_KeyHandle, NodeID, &_vel, &m_ulErrorCode))
		return _vel;
}

short EPOS::GetCurrentIsAveraged(WORD NodeID)
{
	short _cur = 0;
	if (VCS_GetCurrentIsAveraged(m_KeyHandle, NodeID, &_cur, &m_ulErrorCode))
		return _cur;
}

bool EPOS::ClearFault(WORD NodeID)
{
	return VCS_ClearFault(m_KeyHandle, NodeID, &m_ulErrorCode);
}

void EPOS::Disable(WORD NodeID)
{
	if (!VCS_SetDisableState(m_KeyHandle, NodeID, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
}

void EPOS::Enable(WORD NodeID)
{
	BOOL oFault = FALSE;
	if (!VCS_GetFaultState(m_KeyHandle, NodeID, &oFault, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
		return;
	}
	if (oFault)
	{
		if (!VCS_ClearFault(m_KeyHandle, NodeID, &m_ulErrorCode))
		{
			ShowErrorInformation(m_ulErrorCode);
			return;
		}
	}
	if (!VCS_SetEnableState(m_KeyHandle, NodeID, &m_ulErrorCode))
	{
		ShowErrorInformation(m_ulErrorCode);
	}
}

void EPOS::Move_Position_Mode(WORD NodeID, DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, long StartPosition, long TargetPosition, long ActualValue,bool is_absolute,bool is_immediately)
{
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, NodeID, &m_ulErrorCode))
	{
		//Write Profile Position Mode
		if (VCS_SetOperationMode(m_KeyHandle, NodeID, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode))
		{
			//Write Profile Position Objects
			if (VCS_SetPositionProfile(m_KeyHandle, NodeID, ProfileVelocity, ProfileAcceleration, ProfileDeceleration, &m_ulErrorCode))
			{
				//Read Actual Position
				if (VCS_GetPositionIs(m_KeyHandle, NodeID, &StartPosition, &m_ulErrorCode))
				{
					BOOL oFault = FALSE;
					if (!VCS_GetFaultState(m_KeyHandle, NodeID, &oFault, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
						return;
					}
					if (oFault)
					{
						if (!VCS_ClearFault(m_KeyHandle, NodeID, &m_ulErrorCode))
						{
							ShowErrorInformation(m_ulErrorCode);
							return;
						}
					}
					if (!VCS_SetEnableState(m_KeyHandle, NodeID, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					if (!VCS_MoveToPosition(m_KeyHandle, NodeID, TargetPosition, is_absolute, is_immediately, &m_ulErrorCode))
					{
						ShowErrorInformation(m_ulErrorCode);
					}
					//UpdateStatus();
					return;
				}
			}
		}
	}
	ShowErrorInformation(m_ulErrorCode);
}

bool EPOS::HaltVelocityMovement(WORD NodeID)
{
	return VCS_HaltVelocityMovement(m_KeyHandle, NodeID, &m_ulErrorCode);
}

bool EPOS::HaltPositionMovement(WORD NodeID)
{
	return VCS_HaltPositionMovement(m_KeyHandle, NodeID, &m_ulErrorCode);
}

bool EPOS::SetDisableState(WORD NodeID)
{
	return VCS_SetDisableState(m_KeyHandle, NodeID, &m_ulErrorCode);
}
