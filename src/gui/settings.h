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
	bool runOnBoot;
public:
	static Settings* getInstance();

	void save();
	void load();

	Theme getCurrentTheme();
	void setCurrentTheme(Theme t);
	bool runsOnBoot();
	QDir getAppDir();

	void toggleRunOnStartup(bool checked);
};

#endif // SETTINGS_H
