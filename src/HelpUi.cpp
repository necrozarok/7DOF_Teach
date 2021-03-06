#include "HelpUi.h"
#include "ui_HelpUi.h"
#pragma execution_character_set("utf-8")
HelpUi::HelpUi(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::HelpUi();
	ui->setupUi(this);

	mouse_press = false;
	mouse_move = false;
	current_index = 0;
	current_pos_x = 0;

	this->resize(QSize(WINDOW_WIDTH, WINDOW_HEIGHT));
	setWindowFlags(Qt::FramelessWindowHint);

	background_label = new QLabel(this);
	background_label->setPixmap(QPixmap(":/help/ico/help/back"));
	background_label->setGeometry(QRect(0, 0, this->width(), this->height()));

	//将4张图片合成一张
	QPixmap pixmap(QSize(this->width()*WINDOW_PAGE_COUNT, WINDOW_HEIGHT));
	QPainter painter(&pixmap);
	for (int i = 0; i < WINDOW_PAGE_COUNT; i++)
	{
		painter.drawImage(QRect(WINDOW_WIDTH*i, 0, WINDOW_WIDTH, WINDOW_HEIGHT),
			QImage(QString(":/help/ico/help/help_%1").arg(i)));
	}
	total_label = new QLabel(this);
	total_label->resize(pixmap.size());
	total_label->setPixmap(pixmap);
	total_label->move(WINDOW_START_X, WINDOW_START_Y);

	close_button = new Push_Button(this);
	for (int i = 0; i < WINDOW_BUTTON_COUNT; i++)
	{
		Label *label = new Label(this);
		label->resize(QSize(155, 45));
		label->setPixmap(QPixmap(QString(":/help/ico/help/h_%1").arg(i)));
		label->move(8 + i * 170, 319);
		connect(label, SIGNAL(signalLabelPress(Label*)), this, SLOT(changeCurrentPage(Label*)));
		label_array[i] = label;
	}
	label_array[0]->setMousePressFlag(true);

	close_button->setPicName(":/sys/ico/help/close");
	close_button->move(this->width() - close_button->width() - 5, 0);

	connect(close_button, SIGNAL(clicked()), this, SLOT(hide()));

	this->translateLanguage();
}

HelpUi::~HelpUi()
{
	for (int i = 0; i < WINDOW_PAGE_COUNT; i++)
	{
		delete label_array[i];
	}
	delete total_label;
	delete close_button;
	delete background_label;
	delete ui;
}

void HelpUi::translateLanguage()
{
	label_array[0]->setText(tr("    保存数据"));
	label_array[1]->setText(tr("    读取数据"));
	label_array[2]->setText(tr("    电流曲线"));
	label_array[3]->setText(tr("    速度曲线"));
	close_button->setToolTip(tr("close"));
}

void HelpUi::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_mouseSrcPos = event->pos();
		if (m_mouseSrcPos.y() <= 40)
		{
			mouse_move = true;
		}
		else
		{
			current_pos_x = total_label->x();
			mouse_press = true;
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		if (label_move)
		{
			if (current_index > 0)
			{
				current_index--;
				moveCurrentPage(false); //右移
			}
		}
	}
}

void HelpUi::mouseReleaseEvent(QMouseEvent *event)
{
	int xpos = 0;

	if (mouse_press)
	{
		if (label_move)
		{
			m_mouseDstPos = event->pos();

			xpos = m_mouseDstPos.x() - m_mouseSrcPos.x();

			if (xpos > 0)//右移
			{
				if (xpos >= WINDOW_ONEBUTTON_WIDTH)
				{
					if (current_index > 0)
					{
						current_index--;
						moveCurrentPage(false); //右移
					}
					else
					{
						moveCurrentPage(true); //左移
					}
				}
				else
				{
					moveCurrentPage(true); //左移
				}
			}
			else //左移
			{
				if (xpos <= -WINDOW_ONEBUTTON_WIDTH)
				{
					if (current_index < WINDOW_PAGE_COUNT - 1)
					{
						current_index++;
						moveCurrentPage(true); //左移
					}
					else
					{
						moveCurrentPage(false); //右移
					}
				}
				else
				{
					moveCurrentPage(false); //右移
				}
			}

			mouse_press = false;
		}
	}
	else if (mouse_move)
	{
		mouse_move = false;
	}
}

void HelpUi::mouseMoveEvent(QMouseEvent *event)
{
	int x = 0;
	if (mouse_press)
	{
		if (label_move)
		{
			m_mouseDstPos = event->pos();
			x = m_mouseDstPos.x() - m_mouseSrcPos.x();

			setLabelMove(false);
			total_label->move(current_pos_x + x, WINDOW_START_Y);
			setLabelMove(true);
		}
	}
	else if (mouse_move)
	{
		m_mouseDstPos = event->pos();
		this->move(this->pos() + m_mouseDstPos - m_mouseSrcPos);
	}
}

void HelpUi::keyPressEvent(QKeyEvent *event)
{
	if (label_move)
	{
		switch (event->key())
		{
		case Qt::Key_Left:
		case Qt::Key_Up:
			if (current_index > 0)
			{
				current_index--;
				moveCurrentPage(false); //右移
			}
			break;

		case Qt::Key_Right:
		case Qt::Key_Down:
			if (current_index < WINDOW_PAGE_COUNT - 1)
			{
				current_index++;
				moveCurrentPage(true); //左移
			}
			break;

		default:
			break;
		}
	}
}

void HelpUi::changeCurrentButton()
{
	for (int i = 0; i < WINDOW_BUTTON_COUNT; i++)
	{
		if (i != current_index)
		{
			label_array[i]->setMousePressFlag(false);
		}
		else
		{
			label_array[i]->setMousePressFlag(true);
		}
	}
}

void HelpUi::setLabelMove(bool enable)
{
	label_move = enable;
}

void HelpUi::moveCurrentPage(bool direction)
{
	//改变当前页面对应的按钮
	changeCurrentButton();

	//图片的几个分割点
	//0-680, 680-1360, 1360-2040, 2040-2720
	//真:向左移;  假:向右移

	//左移的几种可能性,对于x坐标
	//index=0, 将label移动到-680*0
	//index=1, 将label移动到-680*1
	//index=2, 将label移动到-680*2
	//index=3, 将label移动到-680*3
	setLabelMove(false);
	int current_pos_x = total_label->x(); //当前label坐标
	int dest_pos_x = -WINDOW_WIDTH * current_index; //目标X坐标
	if (direction)
	{
		while (current_pos_x > dest_pos_x)
		{
			total_label->move(current_pos_x - WINDOW_PAGE_MOVE, WINDOW_START_Y);
			current_pos_x = total_label->x();
			qApp->processEvents(QEventLoop::AllEvents);
		}
	}
	else
	{
		while (current_pos_x < dest_pos_x)
		{
			total_label->move(current_pos_x + WINDOW_PAGE_MOVE, WINDOW_START_Y);
			current_pos_x = total_label->x();
			qApp->processEvents(QEventLoop::AllEvents);
		}
	}

	total_label->move(dest_pos_x, WINDOW_START_Y);
	setLabelMove(true);
}
void HelpUi::changeCurrentPage(Label *label)
{
	for (int i = 0; i < WINDOW_BUTTON_COUNT; i++)
	{
		if (label != label_array[i])
		{
			label_array[i]->setMousePressFlag(false);
		}
	}

	//获取点击的图标下标
	int index = 0;
	for (int i = 0; i < WINDOW_PAGE_COUNT; i++)
	{
		if (label == label_array[i])
		{
			index = i;
			break;
		}
	}

	//若下标小于当前下标右移，否则左移
	if (index < current_index)
	{
		while (index != current_index)
		{
			current_index--;
			moveCurrentPage(false);
		}
	}
	else if (index > current_index)
	{
		while (index != current_index)
		{
			current_index++;
			moveCurrentPage(true);
		}
	}
}