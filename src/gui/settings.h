#ifndef SETTINGS_H
#define SETTINGS_H


enum Theme { LIGHT, DARK };

#include <QDir>
#include "shortcutkeyselector.h"

class QAction;

class FinderWindow;

class Settings
{
public:
	static Settings* getInstance();

	void save();
	void load();

	Theme getCurrentTheme();
	void setCurrentTheme(Theme t);
	bool runsOnBoot();
	void setMainWindow(FinderWindow*);
	QDir getAppDir();
	ShortcutKeySelector::ShortcutKey *getShortcutKey();

	void toggleRunOnStartup(bool checked);
	void registerHotKey();
	void registerHotKey(ShortcutKeySelector::ShortcutKey key);

private:
	Settings();
	static Settings* instance;

	QDir appDir;
	QFile settings;
	Theme currentTheme;
	ShortcutKeySelector::ShortcutKey *shortcutKey;
	bool runOnBoot;
	FinderWindow *window;

};

#endif // SETTINGS_H
