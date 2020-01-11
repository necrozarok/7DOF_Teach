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
	//���㵥Ԫ
	struct stMat
	{
	public:
		float nx; float ny; float nz; float ox; float oy; float oz; float ax; float ay; float az; float px; float py; float pz;
		stMat(void)//��̬
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
		stMat(float x, float y, float z)//λ��+��̬
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
		svTheta(void) //��ʼ���Ƕȳ�ֵΪ0
		{
			t1 = t2 = t3 = t4 = t5 = t6 =t7= 0;
		}
	};
private:
	//�������
	struct _Matrix {
		float *arr;
	};
	void matrix_init(_Matrix *m);
	void matrix_free(_Matrix *m);
	float matrix_read(_Matrix *m, int i, int j);
	bool matrix_write(_Matrix *m, int i, int j, float val);
	bool matrix_multiply(_Matrix *A, _Matrix *B, _Matrix *C);
	void printMatrix(_Matrix *A);//��ӡ����
	void printSTMat(stMat*t);
public:
	svTheta currentTheta;//��ǰ����Ƕ�-����ֵ
	svTheta targetTheta;//����Ŀ��Ƕ�-����ֵ
private:
	bool isVectorOrthogonal(stMat*t);//�ж������Ƿ�����
	bool isEqual(float x1, float x2);//��floatֵ�Ƿ����
	void calcCurrentPosition(void);// ���ݵ�ǰ�Ƕȼ��㻡��ֵ
	bool calcCurrentAngle(svTheta*v, float *angle, int num);//���ݻ���ֵ����Ƕ�
};

