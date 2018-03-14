#ifndef SETTINGS_H
#define SETTINGS_H


enum Theme { LIGHT, DARK };

#include <QDir>

class Settings
{
private:
	Settings();
	static Settings* instance;

	QDir appDir;
	Theme currentTheme;
public:
	static Settings* getInstance();

	void save();
	void load();

	Theme getCurrentTheme();
	void setCurrentTheme(Theme t);

};

#endif // SETTINGS_H
