#pragma once
#include <QSettings>
#include <QFontMetrics>
#define DEFAULT_SKIN            ":/skin/1"
class User_Setting
{
public:
	User_Setting();
	~User_Setting();
public:
	static bool writeInit(QString path, QString user_key, QString user_value);
	static bool readInit(QString path, QString user_key, QString &user_value);
	static bool updateText(QString text, int max_width, QString &elided_text);
	static QString getSkinName();
};

