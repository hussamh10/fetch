#ifndef SETTINGS_H
#define SETTINGS_H


enum Theme { LIGHT, DARK };

#include <QDir>

class QAction;

class Settings
{
private:
	Settings();
	static Settings* instance;

	QDir appDir;
	QFile settings;
	Theme currentTheme;
	QAction *startup;
public:
	static Settings* getInstance();

	void save();
	void load();

	Theme getCurrentTheme();
	void setCurrentTheme(Theme t);

	void toggleRunOnStartup(bool checked);

	void setStartup(QAction *startup);

};

#endif // SETTINGS_H
