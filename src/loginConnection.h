#pragma once
#include <qdialog.h>
#include "ui_loginConnection.h"
class loginConnection : public QDialog
{
	Q_OBJECT
public:
	loginConnection(QDialog *parent = Q_NULLPTR);
	~loginConnection();
private:
	Ui::Form ui;
};

