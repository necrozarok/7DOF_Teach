#pragma once
#include <qthread.h>
#include <QMutex>
#include <QReadWriteLock>
#include <QWaitCondition>
#include <QSemaphore>
#include "Dynamixel.h"
#include "EPOS.h"

#define MAXON_THRESHOLD	1
class ActPosCheckThread :
	public QThread
{
	Q_OBJECT
private:
	QMutex mutex;//互斥量，用于线程同步，保护数据段
	//QReadWriteLock lock;//暂时不用
	QWaitCondition wait;//在一个线程解锁资源后能及时通知其他线程.wakeAll()唤醒.wait()等待被唤醒，用.wait()的线程只能用terminate强制结束
	//QSemaphore semaphore;//基于信号量做线程同步用，可以锁定一定数量的相同的资源，互斥量只能被锁一次，信号量可以多次使用
	bool m_paused = true;
	bool m_stop = false;
	//各个轴的目标位置
	int tar_pos;
	int cur_node;
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
protected:
	void run() Q_DECL_OVERRIDE;//Q_DECL_OVERRIDE在重写虚函数时用到，防止写错虚函数

public:
	ActPosCheckThread(QObject *parent = Q_NULLPTR);
	~ActPosCheckThread();
	void BeginThread();
	void PauseThread();
	void StopThread();//后续调用wait()等待线程退出
	void ReceiveCheckData(
		Dynamixel *dynamixel_object,
		EPOS *epos_object,
		int node,
		int pos
		);
signals:
	void ActPosCheckDone();

};

