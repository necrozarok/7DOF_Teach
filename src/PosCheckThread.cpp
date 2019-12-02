#include "PosCheckThread.h"

void PosCheckThread::run()
{
	m_stop = false;
	while (!m_stop)
	{
		if (!m_paused)
		{
			//mutex.lock();//尝试锁定互斥量，阻塞执行到其他线程解锁这个信号量
			if (
				(abs(node1_pos - epos_object->GetPosition(M1_Node)) < MAXON_THRESHOLD)
				&& (abs(node2_pos - epos_object->GetPosition(M2_Node)) < MAXON_THRESHOLD)
				&& (abs(node3_pos - epos_object->GetPosition(M3_Node)) < MAXON_THRESHOLD)
				&& (abs(ID1_pos - dynamixel_object->getPresentPosition(1)) < DY_THRESHOLD)
				&& (abs(ID2_pos - dynamixel_object->getPresentPosition(2)) < DY_THRESHOLD)
				&& (abs(ID3_pos - dynamixel_object->getPresentPosition(3)) < DY_THRESHOLD)
				&& (abs(ID4_pos - dynamixel_object->getPresentPosition(4)) < DY_THRESHOLD)
				)
			{
				emit PosCheckDone();
				m_paused = true;
				m_stop = true;
				//msleep(10);//线程休眠10ms
			}
			//mutex.unlock();
		}
		//msleep(100);//线程休眠100ms
	}
	quit();//相当于exit(0）,退出线程时间循环
}

PosCheckThread::PosCheckThread(QObject *parent)
	: QThread(parent)
{
}

PosCheckThread::~PosCheckThread()
{
}

void PosCheckThread::BeginThread()
{
	m_paused = false;
}

void PosCheckThread::PauseThread()
{
	m_paused = true;
}

void PosCheckThread::StopThread()
{
	m_stop = true;
}

void PosCheckThread::ReceiveCheckData(Dynamixel * dynamixel_object, EPOS * epos_object, int node1_pos, int node2_pos, int node3_pos, int ID1_pos, int ID2_pos, int ID3_pos, int ID4_pos)
{
	this->dynamixel_object = dynamixel_object;
	this->epos_object = epos_object;
	this->node1_pos = node1_pos;
	this->node2_pos= node2_pos;
	this->node3_pos= node3_pos;
	this->ID1_pos = ID1_pos;
	this->ID2_pos= ID2_pos;
	this->ID3_pos= ID3_pos;
	this->ID4_pos= ID4_pos;
}

