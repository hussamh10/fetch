#include "finderwindow.h"
#include "ui_finderwindow.h"
#include <windows.h>
#include <qscreen.h>
#include <qpushbutton.h>
#include <qaction.h>
#include <qlayout.h>
#include <qdesktopservices.h>
#include <qurl.h>
#include <QKeyEvent>

const QString FinderWindow::name = "fuzzyfinder";

FinderWindow::FinderWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FinderWindow)
{
	ui->setupUi(this);
}

void FinderWindow::init() {
	initWindowSize();
	initUI();
	initTray();
	initFont();
	initPyProcess();
	initLocalServer();
	RegisterHotKey(HWND(winId()), 0, 0, VK_F9);

	ignoreResults = false;
	resultCount = 0;
	ui->scroll_area_container->hide();
}

void FinderWindow::initPyProcess() {
	pyproc = new QProcess(this);
	pyproc->start("python main.py");
	connect(pyproc, SIGNAL(readyReadStandardOutput()), this, SLOT(searchResult()));
}

void FinderWindow::initLocalServer() {
	connect(&localServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void FinderWindow::newConnection() {
	toggleWindow();
}

void FinderWindow::keyPressEvent(QKeyEvent* event) {
	QMainWindow::keyPressEvent(event);
	if (event->key() == Qt::Key_Escape) {
		toggleWindow();
	}
}

void FinderWindow::initFont() {
	resultFont.setFamily("Segoe UI");
	resultFont.setPixelSize(14);
}

void FinderWindow::initUI() {
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
}

void FinderWindow::initTray() {
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
	trayIcon->show();
	trayIcon->showMessage("Fuzzy Finder is running", "Press F9 to open the finder window");
}

void FinderWindow::initWindowSize() {
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->geometry();
	setGeometry(screenGeometry.width() / 4, screenGeometry.height() / 3, screenGeometry.width() / 2, 80);
}

void FinderWindow::addResult(QString name, QString path) {
	QPushButton *btn = new QPushButton(this);
	btn->setText(name);
	btn->setFont(resultFont);
	btn->setProperty("path", path);
	btn->setDefault(true);
	connect(btn, SIGNAL(pressed()), this, SLOT(launch()));
	ui->scroll_area->layout()->addWidget(btn);

	resultCount++;
	int calc_height = 80 + resultCount * 50;
	setFixedHeight(calc_height > 400 ? 400 : calc_height);

}

void FinderWindow::clearResults() {
	resultCount = 0;
	qDeleteAll(ui->scroll_area->children());
	ui->scroll_area->setLayout(new QVBoxLayout());
}

void FinderWindow::launch() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(QObject::sender()->property("path").toString()));
	this->hide();
}

bool FinderWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType);
	Q_UNUSED(result);
	MSG *msg = static_cast<MSG*>(message);
	if(msg->message == WM_HOTKEY) {
		toggleWindow();
		return true;
	}
	return false;
}

bool FinderWindow::isAlreadyRunning() {
	QLocalSocket socket;
	socket.connectToServer(name, QLocalSocket::ReadWrite);
	if (socket.waitForConnected()) {
		return true;
	}
	return false;
}

void FinderWindow::startListening() {
	localServer.removeServer(name);
	localServer.listen(name);
}

FinderWindow::~FinderWindow() {
	killProcess();
	localServer.close();
	delete ui;
}


void FinderWindow::search(QString query) {
	pyproc->write(query.toStdString().c_str());
	pyproc->write("\r\n");
}

void FinderWindow::searchResult() {
	while (pyproc->canReadLine()) {
		QString str(pyproc->readLine());
		if (str.trimmed() == ":") {
			ignoreResults = false;
			clearResults();
		} else if (!ignoreResults) {
			QList<QString> list = str.split('|');
			addResult(list[0], list[1].trimmed());
		}
	}
}

void FinderWindow::killProcess() {
	if (pyproc != NULL) {
		pyproc->kill();
		delete pyproc;
		pyproc = NULL;
	}
}

void FinderWindow::revertSearch() {
	ignoreResults = true;
	clearResults();
	setFixedHeight(80);
	ui->searchBar->setText("");
	ui->searchBar->setFocus();
	ui->scroll_area_container->hide();
}

void FinderWindow::toggleWindow() {
	if (this->isHidden()) {
		this->show();
		this->activateWindow();
	} else {
		revertSearch();
		this->hide();
	}
}

void FinderWindow::on_searchBar_textEdited(const QString &arg1) {
	ignoreResults = true;
	if (arg1.isEmpty()) {
		revertSearch();
		return;
	}
	if (arg1.contains(' ')) {
		QString string = arg1;
		ui->searchBar->setText(string.remove(' '));
	}
	ui->scroll_area_container->show();
	search(ui->searchBar->text());
}

