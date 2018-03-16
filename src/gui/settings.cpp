#include "settings.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>

Settings* Settings::instance = nullptr;

Settings::Settings() {
	appDir.setPath(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
	if (!appDir.exists()) {
		appDir.mkdir(appDir.path());
	}
    appTheme.setFileName(appDir.absolutePath().append("/fetch-theme"));
}

Settings* Settings::getInstance() {
	if (instance == nullptr) {
		instance = new Settings();
	}
	return instance;
}

void Settings::save() {
    appTheme.open(QFile::ReadWrite);
    QTextStream out(&appTheme);
	out << currentTheme;
    appTheme.close();
}

void Settings::load() {
    appTheme.open(QFile::ReadOnly);
    if (appTheme.isOpen()) {
        currentTheme = (Theme)QString(appTheme.readAll()).toInt();
	} else {
		currentTheme = LIGHT;
	}
    appTheme.close();
}

Theme Settings::getCurrentTheme() {
	return currentTheme;
}

void Settings::setCurrentTheme(Theme t) {
	currentTheme = t;
}
