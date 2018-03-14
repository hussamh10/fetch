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
}

Settings* Settings::getInstance() {
	if (instance == nullptr) {
		instance = new Settings();
	}
	return instance;
}

void Settings::save() {
	QFile file(appDir.absolutePath().append("/theme"));
	file.open(QFile::ReadWrite);
	QTextStream out(&file);
	out << currentTheme;
}

void Settings::load() {
	QFile file(appDir.absolutePath().append("/theme"));
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		currentTheme = (Theme)QString(file.readAll()).toInt();
	} else {
		currentTheme = LIGHT;
	}
}

Theme Settings::getCurrentTheme() {
	return currentTheme;
}

void Settings::setCurrentTheme(Theme t) {
	currentTheme = t;
}
