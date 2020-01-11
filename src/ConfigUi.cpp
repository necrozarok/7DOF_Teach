#include "ConfigUi.h"
#include "ui_ConfigUi.h"
#pragma execution_character_set("utf-8")
ConfigUi::ConfigUi(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ConfigUi();
	ui->setupUi(this);
	QIcon icon("/ico/machine2.ico");
	setWindowIcon(icon);

	connect(ui->Node1MotionPara, SIGNAL(clicked()), this, SLOT(Node1MotionPara_Clicked()));
	connect(ui->Node2MotionPara, SIGNAL(clicked()), this, SLOT(Node2MotionPara_Clicked()));
	connect(ui->Node3MotionPara, SIGNAL(clicked()), this, SLOT(Node3MotionPara_Clicked()));

	connect(ui->ID1ParaWrite, SIGNAL(clicked()), this, SLOT(ID1_Para_Write_clicked()));
	connect(ui->ID2ParaWrite, SIGNAL(clicked()), this, SLOT(ID2_Para_Write_clicked()));
	connect(ui->ID3ParaWrite, SIGNAL(clicked()), this, SLOT(ID3_Para_Write_clicked()));
	connect(ui->ID4ParaWrite, SIGNAL(clicked()), this, SLOT(ID4_Para_Write_clicked()));
}

ConfigUi::~ConfigUi()
{
	delete ui;
}

void ConfigUi::init(EPOS * e_object, Dynamixel * dy_object)
{
	epos_object = e_object;
	dynamixel_object = dy_object;
}

void ConfigUi::Node1MotionPara_Clicked()
{
	int velo, acce, dece, max_velo, max_acce, max_follow_error;
	//�����ٶ�
	if (ui->mspeed1->text() == "")
		velo = 200;
	else
		velo = ui->mspeed1->text().toInt();
	//�������ٶ�
	if (ui->macceleration1->text() == "")
		acce = 1000;
	else
		acce = ui->macceleration1->text().toInt();
	//��ֹ���ٶ�
	if (ui->mdeceleration1->text() == "")
		dece = 1000;
	else
		dece = ui->mdeceleration1->text().toInt();
	//��������ٶ�
	if (ui->mmspeed1->text() == "")
		max_velo = 2000;
	else
		max_velo = ui->mmspeed1->text().toInt();
	//���������ٶ�
	if (ui->mmacceleration1->text() == "")
		max_acce = 10000;
	else
		max_acce = ui->mmacceleration1->text().toInt();
	//���������
	if (ui->mmFollowingError1->text() == "")
		max_follow_error = 2000;
	else
		max_follow_error = ui->mmFollowingError1->text().toInt();
	emit SendMotionPara1(velo, acce, dece, max_velo, max_acce, max_follow_error);
}

void ConfigUi::Node2MotionPara_Clicked()
{
	int velo, acce, dece, max_velo, max_acce, max_follow_error;
	//�����ٶ�
	if (ui->mspeed2->text() == "")
		velo = 200;
	else
		velo = ui->mspeed2->text().toInt();
	//�������ٶ�
	if (ui->macceleration2->text() == "")
		acce = 1000;
	else
		acce = ui->macceleration2->text().toInt();
	//��ֹ���ٶ�
	if (ui->mdeceleration2->text() == "")
		dece = 1000;
	else
		dece = ui->mdeceleration2->text().toInt();
	//��������ٶ�
	if (ui->mmspeed2->text() == "")
		max_velo = 2000;
	else
		max_velo = ui->mmspeed2->text().toInt();
	//���������ٶ�
	if (ui->mmacceleration2->text() == "")
		max_acce = 10000;
	else
		max_acce = ui->mmacceleration2->text().toInt();
	//���������
	if (ui->mmFollowingError2->text() == "")
		max_follow_error = 2000;
	else
		max_follow_error = ui->mmFollowingError2->text().toInt();
	emit SendMotionPara2(velo, acce, dece, max_velo, max_acce, max_follow_error);
}

void ConfigUi::Node3MotionPara_Clicked()
{
	int velo, acce, dece, max_velo, max_acce, max_follow_error;
	//�����ٶ�
	if (ui->mspeed3->text() == "")
		velo = 200;
	else
		velo = ui->mspeed3->text().toInt();
	//�������ٶ�
	if (ui->macceleration3->text() == "")
		acce = 1000;
	else
		acce = ui->macceleration3->text().toInt();
	//��ֹ���ٶ�
	if (ui->mdeceleration3->text() == "")
		dece = 1000;
	else
		dece = ui->mdeceleration3->text().toInt();
	//��������ٶ�
	if (ui->mmspeed3->text() == "")
		max_velo = 2000;
	else
		max_velo = ui->mmspeed3->text().toInt();
	//���������ٶ�
	if (ui->mmacceleration3->text() == "")
		max_acce = 10000;
	else
		max_acce = ui->mmacceleration3->text().toInt();
	//���������
	if (ui->mmFollowingError3->text() == "")
		max_follow_error = 2000;
	else
		max_follow_error = ui->mmFollowingError3->text().toInt();
	emit SendMotionPara3(velo, acce, dece, max_velo, max_acce, max_follow_error);
}

void ConfigUi::ID1_Para_Write_clicked()
{
	int cw, ccw, velocity, torque, p_gain;
	//cw��λ
	if (ui->ID1CW->text() == "")
		cw = 50;
	else
	{
		cw = ui->ID1CW->text().toInt();

		if (cw < 0 || cw>4095)
		{
			QMessageBox::about(NULL, "cwȡֵ��Χ����", "������0~4095��");
			return;
		}
		else
		{
			//control->setCW(1, cw);			
		}		
	}
	//ccw��λ
	if (ui->ID1CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui->ID1CCW->text().toInt();

		if (ccw < 0 || ccw>4095)
		{
			QMessageBox::about(NULL, "ccwȡֵ��Χ����", "������0~4095��");
			return;
		}	
		else
		{
			//control->setCCW(1, ccw);
		}	
	}
	//�ٶ�
	if (ui->ID1Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui->ID1Velocity->text().toInt();

		if (velocity < 0 || velocity>1023)
		{
			QMessageBox::about(NULL, "velocityȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setSpeed(1, velocity);
		}
	}
	//ת��
	if (ui->ID1Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui->ID1Torque->text().toInt();

		if (torque < 0 || torque>1023)
		{
			QMessageBox::about(NULL, "torqueȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setMaxTorque(1, torque);
		}
	}
	//P����
	if (ui->ID1Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui->ID1Pgain->text().toInt();

		if (!(p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128))
		{
			QMessageBox::about(NULL, "p_gainȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
			return;
		}
		else
		{
			//control->setPgain(1, p_gain);
		}			
	}
	emit Send_ID1_Para(cw, ccw, velocity, torque, p_gain);
}

void ConfigUi::ID2_Para_Write_clicked()
{
	int cw, ccw, velocity, torque, p_gain;
	//cw��λ
	if (ui->ID2CW->text() == "")
		cw = 50;
	else
	{
		cw = ui->ID2CW->text().toInt();

		if (cw < 0 || cw>4095)
		{
			QMessageBox::about(NULL, "cwȡֵ��Χ����", "������0~4095��");
			return;
		}
		else
		{
			//control->setCW(2, cw);			
		}
	}
	//ccw��λ
	if (ui->ID2CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui->ID2CCW->text().toInt();

		if (ccw < 0 || ccw>4095)
		{
			QMessageBox::about(NULL, "ccwȡֵ��Χ����", "������0~4095��");
			return;
		}
		else
		{
			//control->setCCW(2, ccw);
		}
	}
	//�ٶ�
	if (ui->ID2Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui->ID2Velocity->text().toInt();

		if (velocity < 0 || velocity>1023)
		{
			QMessageBox::about(NULL, "velocityȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setSpeed(2, velocity);
		}
	}
	//ת��
	if (ui->ID2Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui->ID2Torque->text().toInt();

		if (torque < 0 || torque>1023)
		{
			QMessageBox::about(NULL, "torqueȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setMaxTorque(2, torque);
		}
	}
	//P����
	if (ui->ID2Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui->ID2Pgain->text().toInt();

		if (!(p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128))
		{
			QMessageBox::about(NULL, "p_gainȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
			return;
		}
		else
		{
			//control->setPgain(2, p_gain);
		}
	}
	emit Send_ID2_Para(cw, ccw, velocity, torque, p_gain);
}

void ConfigUi::ID3_Para_Write_clicked()
{
	int cw, ccw, velocity, torque, p_gain;
	//cw��λ
	if (ui->ID3CW->text() == "")
		cw = 50;
	else
	{
		cw = ui->ID3CW->text().toInt();

		if (cw < 0 || cw>4095)
		{
			QMessageBox::about(NULL, "cwȡֵ��Χ����", "������0~4095��");
			return;
		}
		else
		{
			//control->setCW(3, cw);			
		}
	}
	//ccw��λ
	if (ui->ID3CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui->ID3CCW->text().toInt();

		if (ccw < 0 || ccw>4095)
		{
			QMessageBox::about(NULL, "ccwȡֵ��Χ����", "������0~4095��");
			return;
		}
		else
		{
			//control->setCCW(3, ccw);
		}
	}
	//�ٶ�
	if (ui->ID3Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui->ID3Velocity->text().toInt();

		if (velocity < 0 || velocity>1023)
		{
			QMessageBox::about(NULL, "velocityȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setSpeed(3, velocity);
		}
	}
	//ת��
	if (ui->ID3Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui->ID3Torque->text().toInt();

		if (torque < 0 || torque>1023)
		{
			QMessageBox::about(NULL, "torqueȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setMaxTorque(3, torque);
		}
	}
	//P����
	if (ui->ID3Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui->ID3Pgain->text().toInt();

		if (!(p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128))
		{
			QMessageBox::about(NULL, "p_gainȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
			return;
		}
		else
		{
			//control->setPgain(3, p_gain);
		}
	}
	emit Send_ID3_Para(cw, ccw, velocity, torque, p_gain);
}

void ConfigUi::ID4_Para_Write_clicked()
{
	int cw, ccw, velocity, torque, p_gain;
	//cw��λ
	if (ui->ID4CW->text() == "")
		cw = 50;
	else
	{
		cw = ui->ID4CW->text().toInt();

		if (cw < 0 || cw>4095)
		{
			QMessageBox::about(NULL, "cwȡֵ��Χ����", "������0~4095��");
			return;
		}
		else
		{
			//control->setCW(4, cw);			
		}
	}
	//ccw��λ
	if (ui->ID4CCW->text() == "")
		ccw = 4000;
	else
	{
		ccw = ui->ID4CCW->text().toInt();

		if (ccw < 0 || ccw>4095)
		{
			QMessageBox::about(NULL, "ccwȡֵ��Χ����", "������0~4095��");
			return;
		}
		else
		{
			//control->setCCW(4, ccw);
		}
	}
	//�ٶ�
	if (ui->ID4Velocity->text() == "")
		velocity = 100;
	else
	{
		velocity = ui->ID4Velocity->text().toInt();

		if (velocity < 0 || velocity>1023)
		{
			QMessageBox::about(NULL, "velocityȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setSpeed(4, velocity);
		}
	}
	//ת��
	if (ui->ID4Torque->text() == "")
		torque = 600;
	else
	{
		torque = ui->ID4Torque->text().toInt();

		if (torque < 0 || torque>1023)
		{
			QMessageBox::about(NULL, "torqueȡֵ��Χ����", "������0~1023��");
			return;
		}
		else
		{
			//control->setMaxTorque(4, torque);
		}
	}
	//P����
	if (ui->ID4Pgain->text() == "")
		p_gain = 32;
	else
	{
		p_gain = ui->ID4Pgain->text().toInt();

		if (!(p_gain == 8 || p_gain == 16 || p_gain == 32 || p_gain == 64 || p_gain == 128))
		{
			QMessageBox::about(NULL, "p_gainȡֵ��Χ����", "���{8,16,32,64,128}��ȡֵ��");
			return;
		}
		else
		{
			//control->setPgain(4, p_gain);
		}
	}
	emit Send_ID4_Para(cw, ccw, velocity, torque, p_gain);
}

