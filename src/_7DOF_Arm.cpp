#include "_7DOF_Arm.h"
#pragma execution_character_set("utf-8")


_7DOF_Arm::_7DOF_Arm()
{
	//位置读取定时器
	read_Timer = new QTimer(this);
	//自激定时器
	actuate_Timer=new QTimer(this);
}


_7DOF_Arm::~_7DOF_Arm()
{
	if (dynamixel_object->checkPort())
	{
		dynamixel_object->disableTorque(DY_ID_A);
		dynamixel_object->disableTorque(DY_ID_B);
		dynamixel_object->disableTorque(DY_ID_C);
		dynamixel_object->disableTorque(DY_ID_D);
		
	}
	dynamixel_object = nullptr;
}

void _7DOF_Arm::init(EPOS * e_object, Dynamixel * dy_object)
{
	epos_object = e_object;
	dynamixel_object = dy_object;
}

void _7DOF_Arm::closeEvent(QCloseEvent *event)
{
	//窗口关闭时间必须结束线程
	if (pos_check_thread.isRunning())
	{
		pos_check_thread.StopThread();
		//pos_check_thread.wait();
	}
	event->accept();
}

void _7DOF_Arm::Initialize()
{
	StartTimer();
	//EPOS初始化
	epos_object->ClearFault(M1_Node);
	epos_object->ClearFault(M2_Node);
	epos_object->ClearFault(M3_Node);

	m_ulProfileAcceleration1 = 200;
	m_ulProfileDeceleration1 = 200;
	m_ulProfileVelocity1 = 500;

	m_ulProfileAcceleration2 = 200;
	m_ulProfileDeceleration2 = 200;
	m_ulProfileVelocity2 = 500;

	m_ulProfileAcceleration3 = 200;
	m_ulProfileDeceleration3 = 200;
	m_ulProfileVelocity3 = 500;

	m_lStartPosition1 = epos_object->GetPosition(M1_Node);
	m_lStartPosition2 = epos_object->GetPosition(M2_Node);
	m_lStartPosition3 = epos_object->GetPosition(M3_Node);
	//Dynamixel初始化
	dynamixel_object->setSpeed(DY_ID_A, max_speed[0]);
	dynamixel_object->setSpeed(DY_ID_B, max_speed[1]);
	dynamixel_object->setSpeed(DY_ID_C, max_speed[2]);
	dynamixel_object->setSpeed(DY_ID_D, max_speed[3]);

	dynamixel_object->setPgain(DY_ID_A, pgain[0]);
	dynamixel_object->setPgain(DY_ID_B, pgain[1]);
	dynamixel_object->setPgain(DY_ID_C, pgain[2]);
	dynamixel_object->setPgain(DY_ID_D, pgain[3]);

	dynamixel_object->setMaxTorque(DY_ID_A, loosen_torque[0]);
	dynamixel_object->setMaxTorque(DY_ID_B, loosen_torque[1]);
	dynamixel_object->setMaxTorque(DY_ID_C, loosen_torque[2]);
	dynamixel_object->setMaxTorque(DY_ID_D, loosen_torque[3]);

	dynamixel_object->enableTorque(DY_ID_A);
	dynamixel_object->enableTorque(DY_ID_B);
	dynamixel_object->enableTorque(DY_ID_C);
	dynamixel_object->enableTorque(DY_ID_D);
	return;
}

void _7DOF_Arm::MotionPara1(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError)
{
	m_ulProfileVelocity1 = ProfileVelocity;
	m_ulProfileAcceleration1 = ProfileAcceleration;
	m_ulProfileDeceleration1 = ProfileDeceleration;
	m_Amaxvelocity1 = Amaxvelocity;
	m_AmaxAcceleration1 = AmaxAcceleration;
	m_AmaxFollowingError1 = AmaxFollowingError;
	epos_object->SetMaxFollowingError(M1_Node, AmaxFollowingError);
	epos_object->SetMaxProfileVelocity(M1_Node, Amaxvelocity);
	epos_object->SetMaxAcceleration(M1_Node, AmaxAcceleration);
	QMessageBox::about(NULL, "", "Maxon Node 1 参数设置成功。速度，启动加速度，停止加速度将在运动Mode激活后写入。");
}

void _7DOF_Arm::MotionPara2(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError)
{
	m_ulProfileVelocity2 = ProfileVelocity;
	m_ulProfileAcceleration2 = ProfileAcceleration;
	m_ulProfileDeceleration2 = ProfileDeceleration;
	m_Amaxvelocity2 = Amaxvelocity;
	m_AmaxAcceleration2 = AmaxAcceleration;
	m_AmaxFollowingError2 = AmaxFollowingError;
	epos_object->SetMaxFollowingError(M2_Node, AmaxFollowingError);
	epos_object->SetMaxProfileVelocity(M2_Node, Amaxvelocity);
	epos_object->SetMaxAcceleration(M2_Node, AmaxAcceleration);
	QMessageBox::about(NULL, "", "Maxon Node 2 参数设置成功。速度，启动加速度，停止加速度将在运动Mode激活后写入。");
}

void _7DOF_Arm::MotionPara3(DWORD ProfileVelocity, DWORD ProfileAcceleration, DWORD ProfileDeceleration, DWORD Amaxvelocity, DWORD AmaxAcceleration, DWORD AmaxFollowingError)
{
	m_ulProfileVelocity3 = ProfileVelocity;
	m_ulProfileAcceleration3 = ProfileAcceleration;
	m_ulProfileDeceleration3 = ProfileDeceleration;
	m_Amaxvelocity3 = Amaxvelocity;
	m_AmaxAcceleration3 = AmaxAcceleration;
	m_AmaxFollowingError3 = AmaxFollowingError;
	epos_object->SetMaxFollowingError(M3_Node, AmaxFollowingError);
	epos_object->SetMaxProfileVelocity(M3_Node, Amaxvelocity);
	epos_object->SetMaxAcceleration(M3_Node, AmaxAcceleration);
	QMessageBox::about(NULL, "", "Maxon Node 3 参数设置成功。速度，启动加速度，停止加速度将在运动Mode激活后写入。");
}

void _7DOF_Arm::ID1_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain)
{
	dynamixel_object->setMaxTorque(1, torque);
	dynamixel_object->setCW(1, cw);
	dynamixel_object->setCCW(1, ccw);
	dynamixel_object->setSpeed(1, velocity);
	dynamixel_object->setPgain(1, p_gain);
	max_torque[0] = torque;
	QMessageBox::about(NULL, "", "ID1 参数写入成功");
}

void _7DOF_Arm::ID2_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain)
{
	dynamixel_object->setMaxTorque(2, torque);
	dynamixel_object->setCW(2, cw);
	dynamixel_object->setCCW(2, ccw);
	dynamixel_object->setSpeed(2, velocity);
	dynamixel_object->setPgain(2, p_gain);
	max_torque[1] = torque;
	QMessageBox::about(NULL, "", "ID2 参数写入成功");
}

void _7DOF_Arm::ID3_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain)
{
	dynamixel_object->setMaxTorque(3, torque);
	dynamixel_object->setCW(3, cw);
	dynamixel_object->setCCW(3, ccw);
	dynamixel_object->setSpeed(3, velocity);
	dynamixel_object->setPgain(3, p_gain);
	max_torque[2] = torque;
	QMessageBox::about(NULL, "", "ID3 参数写入成功");
}

void _7DOF_Arm::ID4_Para_Write(int cw, int ccw, int velocity, int torque, int p_gain)
{
	dynamixel_object->setMaxTorque(4, torque);
	dynamixel_object->setCW(4, cw);
	dynamixel_object->setCCW(4, ccw);
	dynamixel_object->setSpeed(4, velocity);
	dynamixel_object->setPgain(4, p_gain);
	max_torque[2] = torque;
	QMessageBox::about(NULL, "", "ID4 参数写入成功");
}

void _7DOF_Arm::StartTimer()
{
	isConnected = true;
	read_Timer->start(500);
	connect(read_Timer, SIGNAL(timeout()), this, SLOT(ReadAllPos()));
	connect(read_Timer, SIGNAL(timeout()), this, SLOT(EPOSCheck()));
}

void _7DOF_Arm::ReadAllPos()
{
	//再次校验端口是否连接，无连接则停止定时器，直到再次收到Connected信号
	if (!(dynamixel_object->isconnected() && epos_object->isconnected()))
		StopTimer();
	else
	emit PosSignal(
		epos_object->GetPosition(M1_Node),
		epos_object->GetPosition(M2_Node),
		epos_object->GetPosition(M3_Node),
		dynamixel_object->getPresentPosition(DY_ID_A),
		dynamixel_object->getPresentPosition(DY_ID_B),
		dynamixel_object->getPresentPosition(DY_ID_C),
		dynamixel_object->getPresentPosition(DY_ID_D)
	);
}

void _7DOF_Arm::StopTimer()
{
	isConnected = false;
	read_Timer->stop();
	actuate_Timer->stop();
}

void _7DOF_Arm::ReadPosInTeach()
{
	//再次校验端口是否连接，无连接则不读取
	if (!(dynamixel_object->isconnected() && epos_object->isconnected()))
		return;
	//发送已获取的位置
	emit TeachPosSignal(
		epos_object->GetPosition(M1_Node),
		epos_object->GetPosition(M2_Node),
		epos_object->GetPosition(M3_Node),
		dynamixel_object->getPresentPosition(DY_ID_A),
		dynamixel_object->getPresentPosition(DY_ID_B),
		dynamixel_object->getPresentPosition(DY_ID_C),
		dynamixel_object->getPresentPosition(DY_ID_D)
	);
	//保持当前位置
	dynamixel_object->move(DY_ID_A, dynamixel_object->getPresentPosition(DY_ID_A));
	dynamixel_object->move(DY_ID_B, dynamixel_object->getPresentPosition(DY_ID_B));
	dynamixel_object->move(DY_ID_C, dynamixel_object->getPresentPosition(DY_ID_C));
	dynamixel_object->move(DY_ID_D, dynamixel_object->getPresentPosition(DY_ID_D));
	//松开
	dynamixel_object->setMaxTorque(DY_ID_A, loosen_torque[0]);
	dynamixel_object->setMaxTorque(DY_ID_B, loosen_torque[1]);
	dynamixel_object->setMaxTorque(DY_ID_C, loosen_torque[2]);
	dynamixel_object->setMaxTorque(DY_ID_D, loosen_torque[3]);
	////启动内驱力
	//actuate_Timer->start(250);
}

void _7DOF_Arm::RecurringInTeach(int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos)
{
	
	//关闭内驱力
	actuate_Timer->stop();
	//QMessageBox::about(NULL, "", QString::asprintf("%d %d %d %d %d %d %d ", node1_pos, node2_pos, node3_pos, ID1_pos, ID2_pos, ID3_pos, ID4_pos));
	m_lTargetPosition1 = node1_pos;
	m_lTargetPosition2 = node2_pos;
	m_lTargetPosition3 = node3_pos;
	dxl_goal_position[0] = ID1_pos;
	dxl_goal_position[1] = ID2_pos;
	dxl_goal_position[2] = ID3_pos;
	dxl_goal_position[3] = ID4_pos;
	pos_check_thread.ReceiveCheckData
	(
		dynamixel_object,
		epos_object,
		node1_pos,
		node2_pos,
		node3_pos,
		ID1_pos,
		ID2_pos,
		ID3_pos,
		ID4_pos
	);
	Move();
	pos_check_thread.start();
	pos_check_thread.BeginThread();
	//emit MoveStart(control->GetGroupPos(), control->getThreshold());//发送目标位置给running线程
	//current_row += 1;
}

void _7DOF_Arm::Move()
{
	epos_object->Move_Position_Mode(
		M1_Node,
		m_ulProfileVelocity1,
		m_ulProfileAcceleration1,
		m_ulProfileDeceleration1,
		m_lStartPosition1,
		m_lTargetPosition1,
		m_lActualPosition1,
		ISABSOLUTE,
		ISIMMEDIATELY
	);
	epos_object->Move_Position_Mode(
		M2_Node,
		m_ulProfileVelocity2,
		m_ulProfileAcceleration2,
		m_ulProfileDeceleration2,
		m_lStartPosition2,
		m_lTargetPosition2,
		m_lActualPosition2,
		ISABSOLUTE,
		ISIMMEDIATELY
	);
	epos_object->Move_Position_Mode(
		M3_Node,
		m_ulProfileVelocity3,
		m_ulProfileAcceleration3,
		m_ulProfileDeceleration3,
		m_lStartPosition3,
		m_lTargetPosition3,
		m_lActualPosition3,
		ISABSOLUTE,
		ISIMMEDIATELY
	);

	dynamixel_object->enableTorque(DY_ID_A);
	dynamixel_object->enableTorque(DY_ID_B);
	dynamixel_object->enableTorque(DY_ID_C);
	dynamixel_object->enableTorque(DY_ID_D);
	dynamixel_object->setMaxTorque(DY_ID_A, max_torque[0]);
	dynamixel_object->setMaxTorque(DY_ID_B, max_torque[1]);
	dynamixel_object->setMaxTorque(DY_ID_C, max_torque[2]);
	dynamixel_object->setMaxTorque(DY_ID_D, max_torque[3]);

	dynamixel_object->move(DY_ID_A, dxl_goal_position[0]);
	dynamixel_object->move(DY_ID_B, dxl_goal_position[1]);
	dynamixel_object->move(DY_ID_C, dxl_goal_position[2]);
	dynamixel_object->move(DY_ID_D, dxl_goal_position[3]);

	//同步写暂时不可用
	/*dynamixel_object->addPosParam(DY_ID_A, ID1_pos);
	dynamixel_object->addPosParam(DY_ID_B, ID2_pos);
	dynamixel_object->addPosParam(DY_ID_C, ID3_pos);
	dynamixel_object->addPosParam(DY_ID_D, ID4_pos);
	dynamixel_object->groupwriteAll();*/
}

void _7DOF_Arm::UrgencyHalt()
{
	pos_check_thread.StopThread();
	if (!epos_object->HaltPositionMovement(M1_Node))
		epos_object->ShowErrorInformation();
	if (!epos_object->HaltPositionMovement(M2_Node))
		epos_object->ShowErrorInformation();
	if (!epos_object->HaltPositionMovement(M3_Node))
		epos_object->ShowErrorInformation();
	dynamixel_object->move(DY_ID_A, dynamixel_object->getPresentPosition(DY_ID_A));
	dynamixel_object->move(DY_ID_B, dynamixel_object->getPresentPosition(DY_ID_B));
	dynamixel_object->move(DY_ID_C, dynamixel_object->getPresentPosition(DY_ID_C));
	dynamixel_object->move(DY_ID_D, dynamixel_object->getPresentPosition(DY_ID_D));
}

void _7DOF_Arm::EPOSCheck()
{
	//再次校验端口是否连接，无连接则停止定时器，直到再次收到Connected信号
	if (!(dynamixel_object->isconnected() && epos_object->isconnected()))
		StopTimer();
	m_lActuaCurrent1 = epos_object->GetCurrentIsAveraged(M1_Node);
	if (m_lActuaCurrent1 > m_ThresholdCurrent1)
	{
		QMessageBox::about(NULL, "", "Node1电流过大(大于7000mA)，自动制动");
		if (!epos_object->HaltVelocityMovement(M1_Node))
		{
			epos_object->ShowErrorInformation();
		}
		if (!epos_object->SetDisableState( M1_Node))
		{
			epos_object->ShowErrorInformation();
		}
	}

	m_lActuaCurrent2 = epos_object->GetCurrentIsAveraged(M2_Node);
	if (m_lActuaCurrent2 > m_ThresholdCurrent2)
	{
		QMessageBox::about(NULL, "", "Node2电流过大(大于7000mA)，自动制动");
		if (!epos_object->HaltVelocityMovement(M2_Node))
		{
			epos_object->ShowErrorInformation();
		}
		if (!epos_object->SetDisableState(M2_Node))
		{
			epos_object->ShowErrorInformation();
		}
	}

	m_lActuaCurrent3 = epos_object->GetCurrentIsAveraged(M3_Node);
	if (m_lActuaCurrent3 > m_ThresholdCurrent3)
	{
		QMessageBox::about(NULL, "", "Node3电流过大(大于7000mA)，自动制动");
		if (!epos_object->HaltVelocityMovement(M3_Node))
		{
			epos_object->ShowErrorInformation();
		}
		if (!epos_object->SetDisableState(M3_Node))
		{
			epos_object->ShowErrorInformation();
		}
	}
}

void _7DOF_Arm::cb_Actuate_checked()
{
	//QMessageBox::about(NULL, "", "open Actuate");
	//设置激励时前三轴的运动参数
	m_ulProfileAcceleration1 = 1000;
	m_ulProfileDeceleration1 = 1000;
	m_ulProfileVelocity1 = 1000;

	m_ulProfileAcceleration2 = 1000;
	m_ulProfileDeceleration2 = 1000;
	m_ulProfileVelocity2 = 1000;

	m_ulProfileAcceleration3 = 1000;
	m_ulProfileDeceleration3 = 1000;
	m_ulProfileVelocity3 = 1000;
	//关闭力矩并刷新起始位置
	epos_object->Disable(M1_Node);
	epos_object->Disable(M2_Node);
	epos_object->Disable(M3_Node);
	Sleep(300);
	m_lStartPosition1 = epos_object->GetPosition(M1_Node);
	m_lStartPosition2 = epos_object->GetPosition(M2_Node);
	m_lStartPosition3 = epos_object->GetPosition(M3_Node);
	
	//后四轴力矩释放
	dynamixel_object->setMaxTorque(DY_ID_A, loosen_torque[0]);
	dynamixel_object->setMaxTorque(DY_ID_B, loosen_torque[1]);
	dynamixel_object->setMaxTorque(DY_ID_C, loosen_torque[2]);
	dynamixel_object->setMaxTorque(DY_ID_D, loosen_torque[3]);

	actuate_Timer->start(50);
	connect(actuate_Timer, SIGNAL(timeout()), this, SLOT(AutoActuate()));
	connect(&act_pos_check_thread, SIGNAL(ActPosCheckDone()), this, SLOT(AutoActuateDone()));
}

void _7DOF_Arm::cb_Actuate_unchecked()
{
	//取消激励时恢复前三轴的运动参数
	m_ulProfileAcceleration1 = 200;
	m_ulProfileDeceleration1 = 200;
	m_ulProfileVelocity1 = 500;

	m_ulProfileAcceleration2 = 200;
	m_ulProfileDeceleration2 = 200;
	m_ulProfileVelocity2 = 500;

	m_ulProfileAcceleration3 = 200;
	m_ulProfileDeceleration3 = 200;
	m_ulProfileVelocity3 = 500;
	actuate_Timer->stop();
	disconnect(actuate_Timer, SIGNAL(timeout()), this, SLOT(AutoActuate()));
	disconnect(&act_pos_check_thread, SIGNAL(ActPosCheckDone()), this, SLOT(AutoActuateDone()));
}

void _7DOF_Arm::AutoActuate()
{
	m_lActualPosition1 = epos_object->GetPosition(M1_Node);
	if (abs(m_lStartPosition1 -m_lActualPosition1) > M1_Node_POS_THRESHOLD)
	{
		//QMessageBox::about(NULL, "", QString::asprintf("1actual:%d  1start:%d", m_lActualPosition1, m_lStartPosition1));
		actuate_Timer->stop();
		disconnect(actuate_Timer, SIGNAL(timeout()), this, SLOT(AutoActuate()));
		m_lTargetPosition1 = (m_lActualPosition1 > m_lStartPosition1) ? epos_object->GetPosition(M1_Node) + 40000 : epos_object->GetPosition(M1_Node) - 40000;
		//QMessageBox::about(NULL, "", QString::asprintf("1target:%d", m_lTargetPosition1));
		//QMessageBox::about(NULL, "", QString::asprintf("1target:%d", m_lTargetPosition1));
		epos_object->Move_Position_Mode(
			M1_Node,
			m_ulProfileVelocity1,
			m_ulProfileAcceleration1,
			m_ulProfileDeceleration1,
			m_lStartPosition1,
			m_lTargetPosition1,
			m_lActualPosition1,
			ISABSOLUTE,
			ISIMMEDIATELY
		);
		act_pos_check_thread.ReceiveCheckData(
			dynamixel_object,
			epos_object,
			M1_Node,
			m_lTargetPosition1
		);
		act_pos_check_thread.start();
		return;
	}

	m_lActualPosition2 = epos_object->GetPosition(M2_Node);
	if (abs(m_lStartPosition2 -m_lActualPosition2) > M2_Node_POS_THRESHOLD)
	{
		//QMessageBox::about(NULL, "", QString::asprintf("2actual:%d  2start:%d", m_lActualPosition2, m_lStartPosition2));
		actuate_Timer->stop();
		disconnect(actuate_Timer, SIGNAL(timeout()), this, SLOT(AutoActuate()));
		m_lTargetPosition2 = (m_lActualPosition2 > m_lStartPosition2) ? epos_object->GetPosition(M2_Node) + 30000 : epos_object->GetPosition(M2_Node) - 30000;
		//QMessageBox::about(NULL, "", QString::asprintf("2target:%d", m_lTargetPosition2));
		
		epos_object->Move_Position_Mode(
			M2_Node,
			m_ulProfileVelocity2,
			m_ulProfileAcceleration2,
			m_ulProfileDeceleration2,
			m_lStartPosition2,
			m_lTargetPosition2,
			m_lActualPosition2,
			ISABSOLUTE,
			ISIMMEDIATELY
		);
		act_pos_check_thread.ReceiveCheckData(
			dynamixel_object,
			epos_object,
			M2_Node,
			m_lTargetPosition2
		);
		act_pos_check_thread.start();
		return;
	}

	//m_lActualPosition3 = epos_object->GetPosition(M3_Node);
	//if (abs(m_lStartPosition3- m_lActualPosition3) > M3_Node_POS_THRESHOLD)
	//{
	//	//QMessageBox::about(NULL, "", QString::asprintf("3actual:%d  3start:%d", m_lActualPosition3, m_lStartPosition3));
	//	actuate_Timer->stop();
	//	//disconnect(actuate_Timer, SIGNAL(timeout()), this, SLOT(AutoActuate()));
	//	m_lTargetPosition3 = (m_lActualPosition3 > m_lStartPosition3)? epos_object->GetPosition(M3_Node) + 4000 : epos_object->GetPosition(M3_Node) - 4000;
	//	//QMessageBox::about(NULL, "", QString::asprintf("3target:%d", m_lTargetPosition3));
	//	
	//	act_pos_check_thread.ReceiveCheckData(
	//		dynamixel_object,
	//		epos_object,
	//		M3_Node,
	//		m_lTargetPosition3
	//	);

	//	epos_object->Move_Position_Mode(
	//		M3_Node,
	//		m_ulProfileVelocity3,
	//		m_ulProfileAcceleration3,
	//		m_ulProfileDeceleration3,
	//		m_lStartPosition3,
	//		m_lTargetPosition3,
	//		m_lActualPosition3,
	//		ISRELATE,
	//		ISIMMEDIATELY
	//	);
	//	
	//	act_pos_check_thread.start();
	//	return;
	//}
}

void _7DOF_Arm::AutoActuateDone()
{
	
	
	epos_object->Disable(M1_Node);
	
	epos_object->Disable(M2_Node);
	
	epos_object->Disable(M3_Node);
	//QMessageBox::about(NULL, "", "1");
	actuate_Timer->start(50);
	connect(actuate_Timer, SIGNAL(timeout()), this, SLOT(AutoActuate()));
	Sleep(400);
		m_lStartPosition1 = epos_object->GetPosition(M1_Node);
		m_lStartPosition2 = epos_object->GetPosition(M2_Node);
		m_lStartPosition3 = epos_object->GetPosition(M3_Node);
	
	
}

void _7DOF_Arm::cb_Torque_checked()
{
	if (!(dynamixel_object->isconnected() && epos_object->isconnected()))
		return;
	epos_object->Enable(M1_Node);
	epos_object->Enable(M2_Node);
	epos_object->Enable(M3_Node);
	dynamixel_object->setMaxTorque(DY_ID_A, max_torque[0]);
	dynamixel_object->setMaxTorque(DY_ID_B, max_torque[1]);
	dynamixel_object->setMaxTorque(DY_ID_C, max_torque[2]);
	dynamixel_object->setMaxTorque(DY_ID_D, max_torque[3]);
}

void _7DOF_Arm::cb_Torque_unchecked()
{
	if (!(dynamixel_object->isconnected() && epos_object->isconnected()))
		return;
	epos_object->Disable(M1_Node);
	epos_object->Disable(M2_Node);
	epos_object->Disable(M3_Node);
	dynamixel_object->setMaxTorque(DY_ID_A, loosen_torque[0]);
	dynamixel_object->setMaxTorque(DY_ID_B, loosen_torque[1]);
	dynamixel_object->setMaxTorque(DY_ID_C, loosen_torque[2]);
	dynamixel_object->setMaxTorque(DY_ID_D, loosen_torque[3]);
}
