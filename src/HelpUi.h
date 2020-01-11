#pragma once

#include <QWidget>
#include <QLabel>
#include <QApplication>
#include "push_button.h"
#include "label.h"
#define SHADOW_WIDTH            5
#define WINDOW_WIDTH            680
#define WINDOW_HEIGHT           372
#define WINDOW_START_X          0
#define WINDOW_START_Y          0
#define WINDOW_PAGE_COUNT       4
#define WINDOW_BUTTON_COUNT     4
#define WINDOW_PAGE_MOVE        20
#define WINDOW_ONEBUTTON_WIDTH  170
namespace Ui { class HelpUi; };

class HelpUi : public QWidget
{
	Q_OBJECT

public:
	explicit HelpUi(QWidget *parent = Q_NULLPTR);
	~HelpUi();
	void translateLanguage();
	enum EButtonMouseState
	{
		EButtonMouseDefault = 0,
		EButtonMouseEnter,
		EButtonMousePress,
		EButtonMouseNone
	};
protected:
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *);
private slots:
	void changeCurrentPage(Label *label);
private:
	void changeCurrentButton();
	void setLabelMove(bool);
	void moveCurrentPage(bool);

	QStringList name_list;
	QPoint m_mouseSrcPos;
	QPoint m_mouseDstPos;
	int current_pos_x;
	bool mouse_press;
	bool mouse_move;
	bool label_move;
	QLabel *background_label; //����ͼƬ
	int current_index; //��ǰͼƬ�±�
	QLabel *total_label; //ͼƬ������壩
	Label *label_array[4]; //�洢ͼƬ������
	Push_Button *close_button; //�رհ�ť
private:
	Ui::HelpUi *ui;
};
