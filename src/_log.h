#pragma once
#include <string>
#include <fstream>
//<sstream>ʹ��string�����������ַ����顣�������Ա��⻺���������Σ�ա�
//���ң����������Ŀ���������ͱ��Զ��Ƶ���������ʹʹ���˲���ȷ�ĸ�ʽ����Ҳû��Σ�ա�
#include <sstream>
#include <iostream>
#include <ctime>
#if defined(_WIN32) || defined(_WIN64)
// Win Socket Header File(s)
#include <Windows.h>
#include <process.h>
#else
// POSIX Socket Header File(s)
#include <errno.h>
#include <pthread.h>
#endif
using namespace std;
enum logLevel
{
	logERROR = 0,
	logWARNING = 1,
	logINFO = 2,
	logDEBUG = 3
};
class _log
{
public:
	//demo:log_object->Add(logERROR) << " Failed to open the port!" << endl;
	_log();
	~_log();
	ostringstream& Add(logLevel level = logINFO);
	static string getTime();
	static string getLevel(logLevel level);
	void outputFile();
private:
	ostringstream os;
};

//��������ֵת�����ַ���
template<class T>
void to_string(string & result, const T& t)
{
	ostringstream oss;//����һ����
	oss << t;//��ֵ����������
	result = oss.str();//ʹ��str()��Ա��������ȡ���ڲ������һ�ݿ���,��ȡת������ַ�ת������д��result
}

//��������֮���ת��,��in_valueֵת����out_type����
template<class out_type, class in_value>
out_type convert(const in_value & t)
{
	stringstream stream;
	stream << t;//�����д�ֵ
	out_type result;//����洢ת�����
	stream >> result;//��result��д��ֵ
	return result;
}