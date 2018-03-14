#include "finderwindow.h"
#include <windows.h>
#include <QApplication>

int main(int argc, char *argv[])  {

	QApplication a(argc, argv);
	FinderWindow w;

	a.setApplicationName("Fuzzy Finder");

	if (w.isAlreadyRunning()) {
		return 0;
	}

	w.init();
	w.startListening();

	return a.exec();
}
