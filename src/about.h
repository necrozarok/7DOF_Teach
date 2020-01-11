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
	//explicit可以抑制内置类型隐式转换,
	explicit about(QWidget *parent = Q_NULLPTR);
	~about();
	void translateLanguage();
protected:
	void paintEvent(QPaintEvent *event);
private:
	void init();
private:
	Ui::about *ui;
	QLabel *title_label; //标题
	QLabel *title_icon_label; //标题图标
	QLabel *title_info_label; //7DOF—ARM图
	QLabel *info_label; //程序主要功能图
	QLabel *version_label; //主程序版本
	QLabel *mummy_label; //备用库
	QLabel *copyright_label; //版本信息
	QLabel *icon_label; //图标
	Push_Button *close_button; //关闭按钮
	QPushButton *ok_button; //确定按钮
};
