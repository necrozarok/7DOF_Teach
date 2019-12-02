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
	QMutex mutex;//�������������߳�ͬ�����������ݶ�
	//QReadWriteLock lock;//��ʱ����
	QWaitCondition wait;//��һ���߳̽�����Դ���ܼ�ʱ֪ͨ�����߳�.wakeAll()����.wait()�ȴ������ѣ���.wait()���߳�ֻ����terminateǿ�ƽ���
	//QSemaphore semaphore;//�����ź������߳�ͬ���ã���������һ����������ͬ����Դ��������ֻ�ܱ���һ�Σ��ź������Զ��ʹ��
	bool m_paused = true;
	bool m_stop = false;
	//�������Ŀ��λ��
	int tar_pos;
	int cur_node;
	Dynamixel *dynamixel_object;
	EPOS *epos_object;
protected:
	void run() Q_DECL_OVERRIDE;//Q_DECL_OVERRIDE����д�麯��ʱ�õ�����ֹд���麯��

public:
	ActPosCheckThread(QObject *parent = Q_NULLPTR);
	~ActPosCheckThread();
	void BeginThread();
	void PauseThread();
	void StopThread();//��������wait()�ȴ��߳��˳�
	void ReceiveCheckData(
		Dynamixel *dynamixel_object,
		EPOS *epos_object,
		int node,
		int pos
		);
signals:
	void ActPosCheckDone();

};

