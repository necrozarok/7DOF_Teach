#pragma once

#include "Shadow_Widget.h"
#include "Push_Button.h"
#include "User_Setting.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

#define SHADOW_WIDTH            5
#define WINDOW_WIDTH            680
#define WINDOW_HEIGHT           372
#define WINDOW_START_X          0
#define WINDOW_START_Y          0
#define WINDOW_PAGE_COUNT       4
#define WINDOW_BUTTON_COUNT     4
#define WINDOW_PAGE_MOVE        20
#define WINDOW_ONEBUTTON_WIDTH  170

namespace Ui { class about; };

class about : public Shadow_Widget
{
	Q_OBJECT
public:
	//explicit������������������ʽת��,
	explicit about(QWidget *parent = Q_NULLPTR);
	~about();
	void translateLanguage();
protected:
	void paintEvent(QPaintEvent *event);
private:
	void init();
private:
	Ui::about *ui;
	QLabel *title_label; //����
	QLabel *title_icon_label; //����ͼ��
	QLabel *title_info_label; //7DOF��ARMͼ
	QLabel *info_label; //������Ҫ����ͼ
	QLabel *version_label; //������汾
	QLabel *mummy_label; //���ÿ�
	QLabel *copyright_label; //�汾��Ϣ
	QLabel *icon_label; //ͼ��
	Push_Button *close_button; //�رհ�ť
	QPushButton *ok_button; //ȷ����ť
};
