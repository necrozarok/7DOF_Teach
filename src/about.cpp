#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent)
	: Shadow_Widget(parent)
{
	ui = new Ui::about();
	ui->setupUi(this);
	this->setFixedSize(520, 290);
	this->init();

	QHBoxLayout *title_layout = new QHBoxLayout();
	title_layout->addWidget(title_icon_label, 0, Qt::AlignVCenter);
	title_layout->addWidget(title_label, 0, Qt::AlignVCenter);
	title_layout->addStretch();
	title_layout->addWidget(close_button, 0, Qt::AlignTop);
	title_layout->setSpacing(5);
	title_layout->setContentsMargins(10, 0, 5, 0);

	QVBoxLayout *v_layout = new QVBoxLayout();
	v_layout->addWidget(title_info_label);
	v_layout->addWidget(info_label);
	v_layout->addWidget(version_label);
	v_layout->addWidget(mummy_label);
	v_layout->addWidget(copyright_label);
	v_layout->addStretch();
	v_layout->setSpacing(5);
	v_layout->setContentsMargins(0, 15, 0, 0);
	info_label->setContentsMargins(0, 0, 0, 40);

	QHBoxLayout *bottom_layout = new QHBoxLayout();
	bottom_layout->addStretch();
	bottom_layout->addWidget(ok_button);
	bottom_layout->setSpacing(0);
	bottom_layout->setContentsMargins(0, 0, 30, 20);

	QHBoxLayout *h_layout = new QHBoxLayout();
	h_layout->addLayout(v_layout);
	h_layout->addWidget(icon_label);
	h_layout->setSpacing(0);
	h_layout->setContentsMargins(40, 0, 20, 10);

	QVBoxLayout *main_layout = new QVBoxLayout();
	main_layout->addLayout(title_layout);
	main_layout->addStretch();
	main_layout->addLayout(h_layout);
	main_layout->addLayout(bottom_layout);
	main_layout->setSpacing(0);
	main_layout->setContentsMargins(SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH);

	connect(ok_button, SIGNAL(clicked()), this, SLOT(hide()));
	connect(close_button, SIGNAL(clicked()), this, SLOT(hide()));

	this->setLayout(main_layout);
	this->translateLanguage();
}

about::~about()
{
	delete ui;
}

void about::translateLanguage()
{
	title_label->setText(tr("About 7DOF_Arm_Teach"));
	title_info_label->setText(tr("7DOF_Arm"));
	info_label->setText(tr("info"));
	version_label->setText(tr("version :") + QString(" V2.0"));
	mummy_label->setText(tr("update :") + QString(" 2019-12-10"));
	copyright_label->setText(tr("email : zarokCHP@163.com"));
	close_button->setToolTip(tr("close"));
	ok_button->setText(tr("ok"));
}

void about::paintEvent(QPaintEvent * event)
{
	Shadow_Widget::paintEvent(event);
	QString skin_name = User_Setting::getSkinName();
	int height = 35;
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawPixmap(QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - 2 * SHADOW_WIDTH, this->height() - 2 * SHADOW_WIDTH), QPixmap(skin_name));
	painter.drawRect(QRect(SHADOW_WIDTH, height, this->width() - 2 * SHADOW_WIDTH, this->height() - height - SHADOW_WIDTH));
}

void about::init()
{
	title_label = new QLabel();
	title_icon_label = new QLabel();
	title_info_label = new QLabel();
	info_label = new QLabel();
	version_label = new QLabel();
	mummy_label = new QLabel();
	copyright_label = new QLabel();
	icon_label = new QLabel();
	close_button = new Push_Button();
	ok_button = new QPushButton();

	QPixmap title_pixmap(":/img/");
	title_icon_label->setPixmap(title_pixmap);
	title_icon_label->setFixedSize(16, 16);
	title_icon_label->setScaledContents(true);

	close_button->setPicName(QString(":/sys/ico/sysButton/close_hover"));

	title_label->setFixedHeight(30);
	ok_button->setFixedSize(75, 25);
	QPixmap pixmap(":/help/ico/machine2");
	icon_label->setPixmap(pixmap);
	icon_label->setFixedSize(pixmap.size());

	title_label->setObjectName("whiteLabel");
	copyright_label->setObjectName("grayLabel");
	title_info_label->setObjectName("infoLabel");
	info_label->setObjectName("infoLabel");
	ok_button->setObjectName("okButton");

	QFont title_info_font("Î¢ÈíÑÅºÚ", 14, QFont::Bold, false);
	title_info_label->setFont(title_info_font);

	QFont info_font = info_label->font();
	info_font.setBold(true);
	info_label->setFont(info_font);
}
