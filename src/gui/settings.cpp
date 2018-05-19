#include "settings.h"
#include <windows.h>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QCoreApplication>
#include <QAction>
#include <QList>
#include <QMainWindow>
#include <finderwindow.h>

Settings* Settings::instance = nullptr;

Settings::Settings() {
	appDir.setPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
	if (!appDir.exists()) {
		appDir.mkdir(appDir.path());
	}
	settings.setFileName(appDir.absolutePath().append("/fetch.ini"));
}

Settings* Settings::getInstance() {
	if (instance == nullptr) {
		instance = new Settings();
	}
	return instance;
}

void Settings::save() {
	settings.open(QFile::WriteOnly);
	QTextStream out(&settings);
	out << currentTheme << ","
		<< runOnBoot << ","
		<< shortcutKey->ctrl << ","
		<< shortcutKey->alt << ","
		<< shortcutKey->shift << ","
		<< shortcutKey->vk << ","
		<< shortcutKey->keyname;
	settings.close();
}

void Settings::load() {
	settings.open(QFile::ReadOnly);
	bool loadDefaults = true;
	if (settings.isOpen()) {
		QList<QByteArray> params = settings.readAll().trimmed().split(',');
		if (params.length() == 7) {
			currentTheme = (Theme)params[0].toInt();
			runOnBoot = params[1].toInt();
			shortcutKey = new ShortcutKeySelector::
					ShortcutKey(
						params[2].toInt(),
						params[3].toInt(),
						params[4].toInt(),
						params[5].toInt(),
						params[6]
					);
			loadDefaults = false;
		}
	}
	if (loadDefaults) {
		currentTheme = LIGHT;
		runOnBoot = false;
		shortcutKey = new ShortcutKeySelector::ShortcutKey(true, false, false, VK_SPACE, "Space");
	}
	settings.close();
}

Theme Settings::getCurrentTheme() {
	return currentTheme;
}

void Settings::setCurrentTheme(Theme t) {
	currentTheme = t;
	save();
}

bool Settings::runsOnBoot() {
	return runOnBoot;
}

void Settings::setMainWindow(FinderWindow *w) {
	this->window = w;
}

QDir Settings::getAppDir() {
	return appDir;
}

ShortcutKeySelector::ShortcutKey* Settings::getShortcutKey() {
	return shortcutKey;
}

void Settings::toggleRunOnStartup(bool checked) {
	QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	if (checked) {
		settings.setValue("Fetch", QCoreApplication::applicationFilePath()
						  .replace('/', '\\')
						  .append("\"")
						  .prepend("\""));
	} else {
		settings.remove("Fetch");
	}
	runOnBoot = checked;
	save();
}

void Settings::registerHotKey() {
	RegisterHotKey(HWND(window->winId()), 2054, shortcutKey->getModifiers(), shortcutKey->vk);
}

void Settings::registerHotKey(ShortcutKeySelector::ShortcutKey key) {
	*shortcutKey = key;
	UnregisterHotKey(HWND(window->winId()), 2054);
	registerHotKey();
	save();
}
