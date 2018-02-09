#include "finderwindow.h"
#include "ui_finderwindow.h"
#include <windows.h>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QScreen>
#include <QPushButton>
#include <QAction>
#include <QLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QKeyEvent>
#include <QMenu>
#include <QTextDocument>
#include <QPainter>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QScrollBar>

const QString FinderWindow::name = "fuzzyfinder";

FinderWindow::FinderWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FinderWindow)
{
	ui->setupUi(this);
    indexed = false;
}

void FinderWindow::init() {
    initLocalServer();
	initWindowSize();
    initUI();
	initTray();
    initPyProcess();
    initIndexer();
	RegisterHotKey(HWND(winId()), 0, 0, VK_F9);

	ignoreResults = false;
	resultCount = 0;
	tab = new QKeyEvent (QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
	shift_tab = new QKeyEvent (QEvent::KeyPress, Qt::Key_Tab, Qt::ShiftModifier);
	resetSize();

	QGraphicsDropShadowEffect* e = new QGraphicsDropShadowEffect();
	e->setBlurRadius(10);
	e->setOffset(3,3);
	e->setColor(QColor(0,0,0,100));
	QGraphicsDropShadowEffect* e2 = new QGraphicsDropShadowEffect();
	e2->setBlurRadius(10);
	e2->setOffset(3,3);
	e2->setColor(QColor(0,0,0,100));
	ui->searchBar->setGraphicsEffect(e);
	ui->scroll_area_container->setGraphicsEffect(e2);
}

void FinderWindow::initPyProcess() {
	pyproc = new QProcess(this);
	pyproc->start("python main.py");
	connect(pyproc, SIGNAL(readyReadStandardOutput()), this, SLOT(searchResult()));
}

void FinderWindow::initIndexer() {
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(reindex()));
    timer->start(60 * 60 * 1000);
}

void FinderWindow::initLocalServer() {
	connect(&localServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void FinderWindow::newConnection() {
	toggleWindow();
}

void FinderWindow::initUI() {
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint | Qt::Popup | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
}

void FinderWindow::initTray() {
	trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/res/fuzzy.ico"));

	QMenu *menu = new QMenu(this);
    QAction *recenter = new QAction("Recenter", this);
	QAction *exit = new QAction("Exit", this);
    connect(recenter, SIGNAL(triggered(bool)), this, SLOT(initWindowSize()));
	connect(exit, SIGNAL(triggered(bool)), this, SLOT(exit()));
    menu->addAction(recenter);
	menu->addAction(exit);

    trayIcon->setContextMenu(menu);
    trayIcon->show();
    trayIcon->showMessage("Fuzzy Finder", "Fuzzy Finder is indexing your directories.");
}

void FinderWindow::initWindowSize() {
	QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
	setGeometry(screenGeometry.width() / 4, screenGeometry.height() / 8, screenGeometry.width() / 2, 150);
}

void FinderWindow::stylizeButton(QPushButton* button, QString maintext, QString subtext) {
    QTextDocument Text;
	Text.setHtml("<font face='Roboto Lt' color=#000 size=5>" +
                 maintext +
				 "</font>&nbsp;<font face='Roboto Th' color=#333 size=3><i>"+
                 subtext +
				 "</i></font>");

    // crop so it fits inside the button
    QPixmap pixmap(this->size().width() * 0.9, Text.size().height());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    Text.drawContents(&painter, pixmap.rect());

    QIcon ButtonIcon(pixmap);
    button->setIcon(ButtonIcon);
    button->setIconSize(pixmap.size());
}

void FinderWindow::addResult(QString name, QString path) {
    QPushButton *btn = new QPushButton(this);
    stylizeButton(btn, name, path);
    btn->setProperty("path", path);
    btn->setDefault(true);
    connect(btn, SIGNAL(clicked()), this, SLOT(launch()));
    ui->scroll_area->layout()->addWidget(btn);
	resultCount++;
	int calc_height = 90 + resultCount * 60;
	setFixedHeight(calc_height > 400 ? 400 : calc_height);
}

void FinderWindow::clearResults() {
	resultCount = 0;
	qDeleteAll(ui->scroll_area->children());
	ui->scroll_area->setLayout(new QVBoxLayout());
}

void FinderWindow::launch() {
    toggleWindow();
    QDesktopServices::openUrl(QUrl::fromLocalFile(QObject::sender()->property("path").toString()));
}

void FinderWindow::reindex() {
    (new QProcess(this))->start("python libs/index.py");
}

void FinderWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape && !ui->searchBar->hasFocus()) {
        revertSearch();
        return;
	} else if (e->key() == Qt::Key_Down) {
		if (ui->searchBar->hasFocus() && resultCount > 0) {
			QCoreApplication::postEvent(this, tab);
		}
	} else if (e->key() == Qt::Key_Up) {
		if (ui->searchBar->hasFocus() && resultCount > 0) {
			QCoreApplication::postEvent(this, shift_tab);
		}
	}
    QMainWindow::keyPressEvent(e);
}

bool FinderWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
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
        if (!indexed && str.trimmed() == ":indexed") {
            trayIcon->showMessage("Fuzzy Finder", "Indexing complete. Press F9 to open the finder window.");
            indexed = true;
        } else if (str.trimmed() == ":") {
			ignoreResults = false;
			clearResults();
        } else if (indexed && !ignoreResults) {
			QList<QString> list = str.split('|');
			resetSize();
			ui->scroll_area_container->show();
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
	resetSize();
    ui->searchBar->clear();
	ui->searchBar->setFocus();
}

void FinderWindow::toggleWindow() {
    if (!indexed) {
        trayIcon->showMessage("Fuzzy Finder", "Your directories are currently being indexed. Please wait.");
    } else if (this->isHidden()) {
		revertSearch();
		this->show();
		this->activateWindow();
		ui->searchBar->setFocus();
	} else {
		this->hide();
    }
}

void FinderWindow::resetSize() {
	setFixedHeight(81);
	ui->scroll_area_container->hide();
}

void FinderWindow::on_searchBar_textEdited(const QString &arg1) {
    ignoreResults = true;
	resetSize();
    if (arg1.isEmpty()) {
        revertSearch();
		return;
    }
    search(ui->searchBar->text());
}

void FinderWindow::exit() {
	QApplication::quit();
}


void FinderWindow::on_searchBar_returnPressed() {
    ((QPushButton*)ui->scroll_area->layout()->itemAt(0)->widget())->animateClick();
}
