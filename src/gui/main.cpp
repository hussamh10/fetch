#include "finderwindow.h"
#include <windows.h>
#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FinderWindow w;
    return a.exec();
}
