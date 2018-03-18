#include "finderwindow.h"
#include <QApplication>

int main(int argc, char *argv[])  {

	QApplication a(argc, argv);
	FinderWindow w;

    a.setApplicationName("Fetch");

	if (w.isAlreadyRunning()) {
		return 0;
	}

	w.init();
    w.startListening();

	return a.exec();
}
