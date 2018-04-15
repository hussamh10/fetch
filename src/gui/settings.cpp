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
	out << currentTheme;
	endl(out);
	out << runOnBoot;
	settings.close();
}

void Settings::load() {
	settings.open(QFile::ReadOnly);
	if (settings.isOpen()) {
		currentTheme = (Theme)settings.readLine().trimmed().toInt();
	} else {
		currentTheme = LIGHT;
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

QDir Settings::getAppDir() {
	return appDir;
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
