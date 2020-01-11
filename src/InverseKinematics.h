#pragma once
#include <math.h>
#include <stdlib.h>
# include <string.h>
#include <stdio.h>
#define MINUS 0.0001
class InverseKinematics
{
public:
	InverseKinematics();
	~InverseKinematics();
private:
	//计算单元
	struct stMat
	{
	public:
		float nx; float ny; float nz; float ox; float oy; float oz; float ax; float ay; float az; float px; float py; float pz;
		stMat(void)//姿态
		{
			nx = 1;
			ny = 0;
			nz = 0;
			ox = 0;
			oy = 1;
			oz = 0;
			ax = 0;
			ay = 0;
			az = 1;
			px = 0;
			py = 0;
			pz = 0;
		}
		stMat(float x, float y, float z)//位置+姿态
		{
			nx = 1;
			ny = 0;
			nz = 0;
			ox = 0;
			oy = 1;
			oz = 0;
			ax = 0;
			ay = 0;
			az = 1;
			px = x;
			py = y;
			pz = z;
		}
	};
	struct svTheta
	{
	public:
		float t1; float t2; float t3; float t4; float t5; float t6; float t7;
		svTheta(void) //初始化角度初值为0
		{
			t1 = t2 = t3 = t4 = t5 = t6 =t7= 0;
		}
	};
private:
	//矩阵相关
	struct _Matrix {
		float *arr;
	};
	void matrix_init(_Matrix *m);
	void matrix_free(_Matrix *m);
	float matrix_read(_Matrix *m, int i, int j);
	bool matrix_write(_Matrix *m, int i, int j, float val);
	bool matrix_multiply(_Matrix *A, _Matrix *B, _Matrix *C);
	void printMatrix(_Matrix *A);//打印矩阵
	void printSTMat(stMat*t);
public:
	svTheta currentTheta;//当前各轴角度-弧度值
	svTheta targetTheta;//各轴目标角度-弧度值
private:
	bool isVectorOrthogonal(stMat*t);//判断向量是否正交
	bool isEqual(float x1, float x2);//两float值是否相等
	void calcCurrentPosition(void);// 根据当前角度计算弧度值
	bool calcCurrentAngle(svTheta*v, float *angle, int num);//根据弧度值计算角度
};

