#include "Label.h"

Label::Label(QWidget *parent)
	: QWidget(parent)
{
	initVariable();
	initSetupUi();
}

Label::~Label()
{
	delete m_pLabelIcon;
	delete m_pLabelText;
	delete m_pHLayout;
}

void Label::setPixmap(const QPixmap &pixmap)
{
	m_pLabelIcon->setPixmap(pixmap.scaled(QSize(30, 30), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Label::setText(const QString &text)
{
	m_pLabelText->setText(text);
}

void Label::setMouseEnterFlag(bool flag)
{
	m_mouseEnterFlag = flag;
	this->update();
}

void Label::setMousePressFlag(bool flag)
{
	m_mousePressFlag = flag;
	this->update();
}

void Label::enterEvent(QEvent *)
{
	if (!getMousePressFlag())
	{
		setMouseEnterFlag(true);
	}
	this->setCursor(Qt::PointingHandCursor);
}

void Label::leaveEvent(QEvent *)
{
	setMouseEnterFlag(false);
}

void Label::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		setMousePressFlag(true);
		emit signalLabelPress(this);
	}
}

void Label::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	if (getMouseEnterFlag())
	{
		paintWidget(50, &painter);
	}
	else if (getMousePressFlag())
	{
		paintWidget(80, &painter);
	}

	QWidget::paintEvent(event);
}

void Label::initVariable()
{
	setMouseEnterFlag(false);
	setMousePressFlag(false);
}

void Label::initSetupUi()
{
	createFrame();
	createWidget();
	createLayout();
}

void Label::createFrame()
{
	this->setStyleSheet("QWidget {background:transparent;border:0px;color:rgb(50,50,50);font-weight:bold;font-size:16px;}");
}

void Label::createWidget()
{
	m_pLabelIcon = new QLabel(this);
	m_pLabelText = new QLabel(this);
}

void Label::createLayout()
{
	m_pHLayout = new QHBoxLayout;
	m_pHLayout->setSpacing(10);
	m_pHLayout->setContentsMargins(QMargins(5, 0, 5, 0));
	m_pHLayout->addWidget(m_pLabelIcon);
	m_pHLayout->addWidget(m_pLabelText);
	m_pHLayout->addStretch();//添加可伸展间隔
}

void Label::paintWidget(int transparency, QPainter *device)
{
	QPen pen(Qt::NoBrush, 1);
	device->setPen(pen);
	QLinearGradient linear(this->rect().topLeft(), this->rect().bottomLeft());
	linear.setColorAt(0, QColor(255, 255, 255, transparency));

	QBrush brush(linear);
	device->setBrush(brush);
	device->drawRoundedRect(this->rect(), 2, 2);
}

bool Label::getMouseEnterFlag()
{
	return m_mouseEnterFlag;
}

bool Label::getMousePressFlag()
{
	return m_mousePressFlag;
}
