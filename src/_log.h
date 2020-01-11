#pragma once
#include <string>
#include <fstream>
//<sstream>使用string对象来代替字符数组。这样可以避免缓冲区溢出的危险。
//而且，传入参数和目标对象的类型被自动推导出来，即使使用了不正确的格式化符也没有危险。
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

//将多种数值转换成字符串
template<class T>
void to_string(string & result, const T& t)
{
	ostringstream oss;//创建一个流
	oss << t;//把值传递如流中
	result = oss.str();//使用str()成员函数来获取流内部缓冲的一份拷贝,获取转换后的字符转并将其写入result
}

//任意类型之间的转换,将in_value值转换成out_type类型
template<class out_type, class in_value>
out_type convert(const in_value & t)
{
	stringstream stream;
	stream << t;//向流中传值
	out_type result;//这里存储转换结果
	stream >> result;//向result中写入值
	return result;
}