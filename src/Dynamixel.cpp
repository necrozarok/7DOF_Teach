#include "Dynamixel.h"



Dynamixel::Dynamixel()
{
	
}


Dynamixel::~Dynamixel()
{
	
	if (portHandler != nullptr)
		portHandler->closePort();
	portHandler = nullptr;
}

bool Dynamixel::Connect(int baudrate, const char *device_name)
{
	portHandler = dynamixel::PortHandler::getPortHandler(device_name);
	packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
	group = (dynamixel::GroupSyncWrite *)&dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_MX_GOAL_POSITION, LEN_MX_GOAL_POSITION);
	if (portHandler->openPort())
	{
		return true;
		//Dy_Initialize();//初始化速度，力矩以及旋钮取值区间
	}
	else
	{
		return false;
	}
}

bool Dynamixel::isconnected()
{
	return !(portHandler==nullptr);
}

int Dynamixel::checkPort(void)
{
	if (portHandler->openPort())
	{
		//log_->Add(logINFO) << "Port Opened" << endl;
		return 1;
	}
	else
	{
		//log_->Add(logERROR) << " Failed to open the port!" << endl;
		return 0;
	}
}

void Dynamixel::closePort(void)
{
	portHandler->closePort();
}

uint8_t Dynamixel::ismoving(int id)
{
	uint8_t flag = 0;
	dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, id, ADDR_MX_IS_MOVING, &flag, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		//return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		//return 0;
	}
	//printf("Reading...");
	return flag;
}

uint16_t Dynamixel::getPresentCurrent(int id)
{
	static uint16_t current = 0;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_CURRENT, &current, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	return current;
}

uint16_t Dynamixel::getPresentVelocity(int id)
{
	uint16_t velocity = 0;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_PRESENT_VELOCITY, &velocity, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	return velocity;
}

uint16_t Dynamixel::getPresentPosition(int id)
{
	uint16_t dxl_present_position=0;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_PRESENT_POSITION, &dxl_present_position, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		//return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		//return 0;
	}
	//printf("Reading...");
	return dxl_present_position;
}

uint16_t Dynamixel::getMaxTorque(int id)
{
	uint16_t dxl_present_torgue = 0;
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_MX_MAX_TORQUE, &dxl_present_torgue, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		//return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		//return 0;
	}
	//printf("Reading...");
	return dxl_present_torgue;
}

int Dynamixel::setPgain(int id, int pgain)
{
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_P_GAIN, pgain, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else
	{
		//printf("Dynamixel#%d has been successfully connected \n", id);
		return 1;
	}
}

int Dynamixel::setCW(int id, int cw)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_CW_POS_LIMIT, cw, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else
	{
		//printf("The Max Torque has been set successfully\n");
		return 1;
	}
}

int Dynamixel::setCCW(int id, int ccw)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_CCW_POS_LIMIT, ccw, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else
	{
		//printf("The Max Torque has been set successfully\n");
		return 1;
	}
}

int Dynamixel::setMaxTorque(int id, int torque)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_MAX_TORQUE, torque, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else		
		return 1;
}

int Dynamixel::setSpeed(int id, int speed)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_MAX_SPEED, speed, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else
	{
		//printf("The Max Torque has been set successfully\n");
		return 1;
	}
}

int Dynamixel::enableTorque(int id)
{
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else
	{
		//printf("Dynamixel#%d has been successfully connected \n", id);
		return 1;
	}
}

int Dynamixel::disableTorque(int id)
{
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else
	{
		//printf("Dynamixel#%d has been successfully connected \n", id);
		return 1;
	}
}

int Dynamixel::move(int id, int pos)
{
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_GOAL_POSITION, pos, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	else if (dxl_error != 0)
	{
		//packetHandler->printRxPacketError(dxl_error);
		return 0;
	}
	else
	{
		//max_torque[id - 1] = torque;
		//printf("The Max Torque has been set successfully\n");
		return 1;
	}
}

int Dynamixel::addPosParam(int id,int pos)
{
	// Allocate goal position value into byte array
	param_goal_position[0] = DXL_LOBYTE(pos);
	param_goal_position[1] = DXL_HIBYTE(pos);
	dxl_addparam_result = group->addParam(id, param_goal_position);//already made initialize
	if (dxl_addparam_result != true)
	{
		//log_->Add(logERROR) << " [ID:%03d] groupSyncWrite addparam failed" << endl;
		return 0;
	}
	return 1;
}

int Dynamixel::groupwriteAll(void)
{
	dxl_comm_result = group->txPacket();
	if (dxl_comm_result != COMM_SUCCESS) {
		//packetHandler->printTxRxResult(dxl_comm_result);
		return 0;
	}
	return 1;
}

void Dynamixel::groupclearAll()
{
	group->clearParam();
}
