#include "ActPosCheckThread.h"



void ActPosCheckThread::run()
{
	m_stop = false;
	while (!m_stop)
	{
			//mutex.lock();//��������������������ִ�е������߳̽�������ź���
		
			if (
				abs(epos_object->GetPosition(cur_node)- tar_pos)< MAXON_THRESHOLD
				//epos_object->GetCurrentIsAveraged(cur_node)==0
				)
			{
				//msleep(10);//�߳�����10ms
				m_stop = true;
				
				//m_paused = true;
				
				
			}
			//mutex.unlock();
		//msleep(100);//�߳�����100ms
	}
	emit ActPosCheckDone();
	quit();//�൱��exit(0��,�˳��߳�ʱ��ѭ��
}

ActPosCheckThread::ActPosCheckThread(QObject *parent)
	: QThread(parent)
{
}


ActPosCheckThread::~ActPosCheckThread()
{
}

void ActPosCheckThread::BeginThread()
{
	m_paused = false;
}

void ActPosCheckThread::PauseThread()
{
	m_paused = true;
}

void ActPosCheckThread::StopThread()
{
	m_stop = true;
}

void ActPosCheckThread::ReceiveCheckData(Dynamixel * dynamixel_object, EPOS * epos_object, int node, int pos)
{
	this->dynamixel_object = dynamixel_object;
	this->epos_object = epos_object;
	this->tar_pos = pos;
	this->cur_node = node;
	
}
