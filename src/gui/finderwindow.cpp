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
const int FinderWindow::MAX_HEIGHT = 400;
const int FinderWindow::MIN_HEIGHT = 81;

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

	QGraphicsDropShadowEffect* searchBarEffect = new QGraphicsDropShadowEffect();
	searchBarEffect->setBlurRadius(10);
	searchBarEffect->setOffset(0,0);
	searchBarEffect->setColor(QColor(0,0,0,200));

	QGraphicsDropShadowEffect* scrollAreaEffect = new QGraphicsDropShadowEffect();
	scrollAreaEffect->setBlurRadius(10);
	scrollAreaEffect->setOffset(0,0);
	scrollAreaEffect->setColor(QColor(0,0,0,100));

	ui->searchBar->setGraphicsEffect(searchBarEffect);
	ui->scroll_area_container->setGraphicsEffect(scrollAreaEffect);
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
	setGeometry(screenGeometry.width() / 4, (screenGeometry.height() - MAX_HEIGHT) / 2, screenGeometry.width() / 2, MIN_HEIGHT);
}

void FinderWindow::stylizeButton(QPushButton* button, QString maintext, QString subtext) {
	QTextDocument Text;
	Text.setHtml("<font face='Roboto Cn' color=#000 size=5>" +
				 maintext +
				 "</font>&nbsp;<font face='Roboto' color=#777 size=3><i>"+
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
	ui->scroll_area_container->show();
	ui->scroll_area->layout()->addWidget(btn);
	resultCount++;
	if (resultCount == 1) {
		// first result
		btn->setStyleSheet("border-top: none;");
	}
	// min_height + padding + total_results * height_for_each_result
	int calc_height = MIN_HEIGHT + 22 + resultCount * 49;
	setFixedHeight(calc_height > MAX_HEIGHT ? MAX_HEIGHT : calc_height);
}

void FinderWindow::clearResults() {
	resultCount = 0;
	qDeleteAll(ui->scroll_area->children());
	ui->scroll_area->setLayout(createLayout());
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
	if (msg->message == WM_HOTKEY) {
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
		} else if (str.startsWith(":")) {
			if (str.remove(0,1) == ui->searchBar->text()) {
				// make sure results match the current query
				ignoreResults = false;
			}
			clearResults();
		} else if (indexed && !ignoreResults) {
			QList<QString> list = str.split('|');
			resetSize();
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

QLayout *FinderWindow::createLayout() {
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(0);
	return layout;
}

void FinderWindow::on_searchBar_textEdited(const QString &arg1) {
	ignoreResults = false;
	resetSize();
	if (arg1.isEmpty()) {
		revertSearch();
		return;
	}
	search(arg1);
}

void FinderWindow::exit() {
	QApplication::quit();
}


void FinderWindow::on_searchBar_returnPressed() {
	if (ui->scroll_area->layout()->count() > 0) {
		((QPushButton*)ui->scroll_area->layout()->itemAt(0)->widget())->animateClick();
	}
}
