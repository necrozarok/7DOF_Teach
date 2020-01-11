#pragma once
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
class Label : public QWidget
{
	Q_OBJECT
public:
	explicit Label(QWidget *parent = 0);
	~Label();
	void setPixmap(const QPixmap &);
	void setText(const QString &);
	void setMouseEnterFlag(bool);
	void setMousePressFlag(bool);
protected:
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
signals:
	void signalLabelPress(Label *);
private:
	void initVariable();
	void initSetupUi();
	void createFrame();
	void createWidget();
	void createLayout();
	void paintWidget(int, QPainter *);
	bool getMouseEnterFlag();
	bool getMousePressFlag();

	bool m_mouseEnterFlag;
	bool m_mousePressFlag;

	QHBoxLayout *m_pHLayout;
	QLabel *m_pLabelIcon;
	QLabel *m_pLabelText;
};
