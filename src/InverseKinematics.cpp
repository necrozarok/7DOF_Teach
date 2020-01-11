#include "InverseKinematics.h"



InverseKinematics::InverseKinematics()
{
}


InverseKinematics::~InverseKinematics()
{
}

void InverseKinematics::matrix_init(_Matrix * m)
{
	m->arr = (float *)malloc(4 * 4 * sizeof(float));
	memset(m->arr, 0x00, 4 * 4 * sizeof(float));
}

void InverseKinematics::matrix_free(_Matrix * m)
{
	free(m->arr);
}

float InverseKinematics::matrix_read(_Matrix * m, int i, int j)
{
	if (i >= 4 || j >= 4)
	{
		return -1;
	}
	return *(m->arr + i * 4 + j);
}

bool InverseKinematics::matrix_write(_Matrix * m, int i, int j, float val)
{
	if (i >= 4 || j >= 4)
	{
		return false;
	}
	*(m->arr + i * 4 + j) = val;
	return true;
}

bool InverseKinematics::matrix_multiply(_Matrix * A, _Matrix * B, _Matrix * C)
{
	int i = 0;
	int j = 0;
	int k = 0;
	float temp = 0;

	//运算
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			temp = 0;
			for (k = 0; k < 4; k++)
			{
				temp += matrix_read(A, i, k) * matrix_read(B, k, j);
			}
			matrix_write(C, i, j, temp);
		}
	}
	return true;
}

void InverseKinematics::printMatrix(_Matrix * A)
{
	int i = 0;
	int j = 0;
	char m[12];
	char temp[128];
	//运算
	for (i = 0; i < 4; i++)
	{
		temp[0] = 0x00;
		for (j = 0; j < 4; j++)
		{
			//dtostrf(matrix_read(A, i, j), 4, 3, m);
			sprintf(m, "%g", matrix_read(A, i, j));
			strcat(temp, "\t");
			strcat(temp, m);
		}
		//print_debug("%s\r\n", temp);
	}
}

void InverseKinematics::printSTMat(stMat * t)
{
	char temp[256];
	char m[12];
	sprintf(m, "%g",t->ox); strcat(temp, "\t"); strcat(temp, m);
	sprintf(m, "%g",t->ax); strcat(temp, "\t"); strcat(temp, m);
	sprintf(m, "%g",t->px); strcat(temp, "\t"); strcat(temp, m);

	sprintf(m, "%g",t->ny); strcat(temp, "\r\n"); strcat(temp, m);
	sprintf(m, "%g",t->oy); strcat(temp, "\t"); strcat(temp, m);
	sprintf(m, "%g",t->ay); strcat(temp, "\t"); strcat(temp, m);
	sprintf(m, "%g",t->py); strcat(temp, "\t"); strcat(temp, m);

	sprintf(m, "%g",t->nz); strcat(temp, "\r\n"); strcat(temp, m);
	sprintf(m, "%g",t->oz); strcat(temp, "\t"); strcat(temp, m);
	sprintf(m, "%g",t->az); strcat(temp, "\t"); strcat(temp, m);
	sprintf(m, "%g",t->pz); strcat(temp, "\t"); strcat(temp, m); 
	
	strcat(temp, "\r\n0\t0\t0\t1\t\r\n");
	//print_debug(temp);
}

bool InverseKinematics::isVectorOrthogonal(stMat * t)
{
	//t->nx,t->ny, t->nz的平方和为1
    //t->n t->o t->a  n*o n*a o*a 内积均为0则正交
	if (fabs((t->nx) * (t->nx) + (t->ny) * (t->ny) + (t->nz) * (t->nz) - 1) > MINUS) return false;
	if (fabs((t->ox) * (t->ox) + (t->oy) * (t->oy) + (t->oz) * (t->oz) - 1) > MINUS) return false;
	if (fabs((t->ax) * (t->ax) + (t->ay) * (t->ay) + (t->az) * (t->az) - 1) > MINUS) return false;
	if (fabs((t->nx) * (t->ox) + (t->ny) * (t->oy) + (t->nz) * (t->oz)) > MINUS) return false;
	if (fabs((t->nx) * (t->ax) + (t->ny) * (t->ay) + (t->nz) * (t->az)) > MINUS) return false;
	if (fabs((t->ax) * (t->ox) + (t->ay) * (t->oy) + (t->az) * (t->oz)) > MINUS) return false;
	return true;
}

bool InverseKinematics::isEqual(float x1, float x2)
{
	if (fabs(x1 - x2) <= MINUS)
		return true;
	else
		return false;
}

void InverseKinematics::calcCurrentPosition(void)
{
	//currentTheta.t1 = axisRg[0].current * 0.017453;//PI / 180.0;
	//currentTheta.t2 = axisRg[1].current * 0.017453;
	//currentTheta.t3 = axisRg[2].current * 0.017453;
	//currentTheta.t4 = axisRg[3].current * 0.017453;
	//currentTheta.t5 = axisRg[4].current * 0.017453;
	//currentTheta.t6 = axisRg[5].current * 0.017453;
}

bool InverseKinematics::calcCurrentAngle(svTheta * v, float * angle, int num)
{
	if (num != 6) return false;
	angle[0] = v->t1 * 57.295780;//180.0 / PI
	angle[1] = v->t2 * 57.295780;
	angle[2] = v->t3 * 57.295780;
	angle[3] = v->t4 * 57.295780;
	angle[4] = v->t5 * 57.295780;
	angle[5] = v->t6 * 57.295780;
	return true;
}
