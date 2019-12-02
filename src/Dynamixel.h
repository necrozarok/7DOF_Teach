#pragma once
#include <qobject.h>
#include "./include/dynamixel_sdk/dynamixel_sdk.h"
// Control table address
#define ADDR_BAUDRATE					4
#define ADDR_CW_POS_LIMIT			    6
#define ADDR_CCW_POS_LIMIT			    8
#define ADDR_MX_TORQUE_ENABLE           24                  // Control table address is different in Dynamixel model
#define ADDR_P_GAIN			            28
#define ADDR_MX_GOAL_POSITION           30
#define ADDR_MX_MAX_SPEED				32		
#define ADDR_MX_MAX_TORQUE				34		
#define ADDR_MX_PRESENT_POSITION        36
#define ADDR_MX_PRESENT_VELOCITY        38
#define ADDR_MX_IS_MOVING				46
#define ADDR_MX_CURRENT					68
#define ADDR_MX_TORGUE_CONTROL          70
// Data Byte Length
#define LEN_MX_GOAL_POSITION            2
#define LEN_MX_PRESENT_POSITION         2
// Protocol version
#define PROTOCOL_VERSION                1.0                 // See which protocol version is used in the Dynamixel
// Default setting
#define BAUDRATE                        57142
#define DEVICENAME                      "COM5"      
#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MOVING_STATUS_THRESHOLD     30                  // Dynamixel moving status threshold
class Dynamixel : public QObject
{
	Q_OBJECT
private:
	int dxl_comm_result = COMM_TX_FAIL;
	bool dxl_addparam_result = false;
	uint8_t dxl_error = 0;                          // Dynamixel error
	uint8_t param_goal_position[2];
	//dynamixel initialization
	dynamixel::PortHandler *portHandler;
	dynamixel::PacketHandler *packetHandler;
	dynamixel::GroupSyncWrite *group;
public:
	Dynamixel();
	~Dynamixel();
	bool Connect(int baudrate,const char *device_name);
	bool isconnected();
	int checkPort(void);
	void closePort(void); //close communication port
	uint8_t ismoving(int id);
	uint16_t getPresentCurrent(int id);
	uint16_t getPresentVelocity(int id);
	uint16_t getPresentPosition(int id);
	uint16_t getMaxTorque(int id);
	int setPgain(int id, int pgain);
	int setCW(int id, int cw);
	int setCCW(int id, int ccw);
	int setMaxTorque(int id, int torque); //set the torque for EACH motor
	int setSpeed(int id, int speed);//为关节设置速度
	int enableTorque(int id); 
	int disableTorque(int id);
	int move(int id, int pos);
	int addPosParam(int id, int pos);
	int groupwriteAll(void);
	void groupclearAll();
};

