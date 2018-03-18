#include "settings.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QCoreApplication>
#include <QAction>

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
	settings.open(QFile::ReadWrite);
	QTextStream out(&settings);
	out << currentTheme << endl;
	out << startup->isChecked() << endl;
	settings.close();
}

void Settings::load() {
	settings.open(QFile::ReadOnly);
	if (settings.isOpen()) {
		currentTheme = (Theme)settings.readLine().toInt();
		startup->setChecked(settings.readLine().toInt());
	} else {
		currentTheme = LIGHT;
		startup->setChecked(false);
	}
	settings.close();
}

Theme Settings::getCurrentTheme() {
	return currentTheme;
}

void Settings::setCurrentTheme(Theme t) {
	currentTheme = t;
}

void Settings::setStartup(QAction *startup) {
	this->startup = startup;
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
}
