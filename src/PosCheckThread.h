#pragma once

#include <QThread>
#include <QMutex>
#include <QReadWriteLock>
#include <QWaitCondition>
#include <QSemaphore>
#include "Dynamixel.h"
#include "EPOS.h"
#define M1_Node	1
#define M2_Node	2
#define M3_Node	3
#define DY_ID_A		1
#define DY_ID_B		2
#define DY_ID_C		3
#define DY_ID_D		4
#define DY_THRESHOLD	30
#define MAXON_THRESHOLD	300

class PosCheckThread : public QThread
{
	Q_OBJECT
private:
	QMutex mutex;//�������������߳�ͬ�����������ݶ�
	//QReadWriteLock lock;//��ʱ����
	QWaitCondition wait;//��һ���߳̽�����Դ���ܼ�ʱ֪ͨ�����߳�.wakeAll()����.wait()�ȴ������ѣ���.wait()���߳�ֻ����terminateǿ�ƽ���
	//QSemaphore semaphore;//�����ź������߳�ͬ���ã���������һ����������ͬ����Դ��������ֻ�ܱ���һ�Σ��ź������Զ��ʹ��
	bool m_paused = true;
	bool m_stop = false;
	//�������Ŀ��λ��
	int node1_pos,
		node2_pos,
		node3_pos,
		ID1_pos,
		ID2_pos,
		ID3_pos,
		ID4_pos;
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
protected:
	void run() Q_DECL_OVERRIDE;//Q_DECL_OVERRIDE����д�麯��ʱ�õ�����ֹд���麯��
public:
	PosCheckThread(QObject *parent = Q_NULLPTR);
	~PosCheckThread();
	void BeginThread();
	void PauseThread();
	void StopThread();//��������wait()�ȴ��߳��˳�
	void ReceiveCheckData(
		Dynamixel *dynamixel_object,
		EPOS *epos_object, 
		int node1_pos,
		int node2_pos,
		int node3_pos, 
		int ID1_pos,
		int ID2_pos,
		int ID3_pos,
		int ID4_pos);
signals:
	void PosCheckDone();
};
