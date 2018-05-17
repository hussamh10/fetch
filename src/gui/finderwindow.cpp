#include "finderwindow.h"
#include "shortcutkeyselector.h"
#include "ui_finderwindow.h"

#include <windows.h>
#include <QProcess>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QGraphicsDropShadowEffect>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QMenu>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUrl>
#include <QTextDocument>
#include <QPainter>
#include <QKeyEvent>
#include <QScreen>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QActionGroup>
#include <QRegExpValidator>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFile>

const QString FinderWindow::SERVERNAME = "fetch";

FinderWindow::FinderWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FinderWindow) {
	ui->setupUi(this);
	localServer = new QLocalServer(this);
	manager = new QNetworkAccessManager(this);
	indexed = false;
	ignoreResults = false;
	resultCount = 0;
}

FinderWindow::~FinderWindow() {
	delete ui;
}

bool FinderWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
	Q_UNUSED(eventType);
	Q_UNUSED(result);
	MSG *msg = static_cast<MSG*>(message);
	if (msg->message == WM_HOTKEY) {
		toggleWindow();
		return true;
	}
    return QMainWindow::nativeEvent(eventType, message, result);
}

bool FinderWindow::isAlreadyRunning() {
	QLocalSocket socket;
	socket.connectToServer(SERVERNAME, QLocalSocket::ReadWrite);
	if (socket.waitForConnected()) {
		return true;
	}
	return false;
}

void FinderWindow::startListening() {
	localServer->removeServer(SERVERNAME);
	localServer->listen(SERVERNAME);
}

void FinderWindow::initUI() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowStaysOnTopHint | Qt::NoDropShadowWindowHint | Qt::Popup);

    setAttribute(Qt::WA_TranslucentBackground, true);

	QGraphicsDropShadowEffect* searchBarEffect = new QGraphicsDropShadowEffect(this);
	searchBarEffect->setBlurRadius(10);
	searchBarEffect->setOffset(0,0);
	searchBarEffect->setColor(QColor(0,0,0,200));

    ui->topArea->setGraphicsEffect(searchBarEffect);

	ui->scrollAreaContents->layout()->setAlignment(Qt::AlignTop);

    ui->searchBar->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9._- ]*"), ui->searchBar));
}

void FinderWindow::initTray() {
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/icons/app_icon"));

	QMenu *menu = new QMenu(this);
	QAction *startup = new QAction("Run on startup", menu);
	QAction *shortcutKeySelector = new QAction("Set shortcut", menu);
	QAction *exit = new QAction("Exit", menu);

	startup->setCheckable(true);
	startup->setChecked(Settings::getInstance()->runsOnBoot());

	connect(startup, SIGNAL(triggered(bool)), this, SLOT(toggleRunOnStartup(bool)));
	connect(shortcutKeySelector, SIGNAL(triggered(bool)), this, SLOT(launchShortcutKeySelector()));
	connect(exit, SIGNAL(triggered(bool)), this, SLOT(exit()));

	QMenu *themeMenu = new QMenu("Themes", menu);
	QAction *darkTheme = new QAction("Dark", themeMenu);
	QAction *lightTheme = new QAction("Light", themeMenu);

	darkTheme->setProperty("theme", DARK);
	lightTheme->setProperty("theme", LIGHT);

	darkTheme->setCheckable(true);
	lightTheme->setCheckable(true);

	QActionGroup *actionGroup = new QActionGroup(this);
	actionGroup->addAction(lightTheme);
	actionGroup->addAction(darkTheme);

	if (Settings::getInstance()->getCurrentTheme() == DARK)
		darkTheme->setChecked(true);
	else
		lightTheme->setChecked(true);

	if (Settings::getInstance()->runsOnBoot()) {
		startup->setChecked(true);
	}

	connect(darkTheme, SIGNAL(triggered(bool)), this, SLOT(setTheme()));
	connect(lightTheme, SIGNAL(triggered(bool)), this, SLOT(setTheme()));

	themeMenu->addAction(darkTheme);
	themeMenu->addAction(lightTheme);

	menu->addAction(shortcutKeySelector);
	menu->addMenu(themeMenu);
	menu->addAction(startup);
	menu->addAction(exit);

	trayIcon->setContextMenu(menu);
	trayIcon->show();
	trayIcon->showMessage("Fetch", "Press " + Settings::getInstance()->getShortcutKey()->toString() + " to begin.");
}

void FinderWindow::initPyProcess() {
	pyproc = new QProcess(this);
	pyproc->start("main.exe");
	connect(pyproc, SIGNAL(readyReadStandardOutput()), this, SLOT(pyProcOutputAvailable()));
}

void FinderWindow::initLocalServer() {
	connect(localServer, SIGNAL(newConnection()), this, SLOT(newInstance()));
}

void FinderWindow::initIndexer() {
	indexTimer = new QTimer(this);
	connect(indexTimer, SIGNAL(timeout()), this, SLOT(runIndexer()));
	indexTimer->start(3600000); // ‪3600000‬ms = 1hr
}

void FinderWindow::initUpdater() {
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(runUpdater()));
	updateTimer->start(3600000 * 3); // ‪3600000‬ * 3 ms = 3hrs
}

QString FinderWindow::getGlobalStyleSheet() {
	QFile file(":/themes/global");
	file.open(QFile::ReadOnly);
	return QString(file.readAll());
}

QString FinderWindow::getThemeFontColor() {
	return Settings::getInstance()->getCurrentTheme() == LIGHT ? "#000,#777" : "#fff,#ccc";
}

QString FinderWindow::getThemedStyleSheet(Theme t) {
	QFile file;
	switch(t) {
	case DARK:
		file.setFileName(":/themes/dark");
		break;
	case LIGHT:
		file.setFileName(":/themes/light");
		break;
	}
	file.open(QFile::ReadOnly);
	return QString(file.readAll());
}

QLabel *FinderWindow::createNrLabel() {
	QLabel *nrLabel = new QLabel(this);
	QString color = getThemeFontColor().split(",")[0];
	nrLabel->setText("<center><font face='Roboto Cn' color="+color+" size=5>No results found.</font></center>");
	return nrLabel;
}

void FinderWindow::initWindowSize() {
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->geometry();
	setFixedWidth(screenGeometry.width() / 2);
	setGeometry(screenGeometry.width() / 4, screenGeometry.height() / 4, width(), height());
	update();
}

void FinderWindow::setTheme() {
	QVariant v = QObject::sender()->property("theme");
	Theme t = *(Theme*)&v;
	setTheme((Theme)t);
	Settings::getInstance()->setCurrentTheme(t);
}

void FinderWindow::launchShortcutKeySelector() {
	(new ShortcutKeySelector(this))->show();
}

void FinderWindow::toggleRunOnStartup(bool checked) {
	Settings::getInstance()->toggleRunOnStartup(checked);
}

void FinderWindow::handleReply(QNetworkReply *r) {
	if(r->request().url().toString().contains("update.zip")) {
		updateDownloaded(r);
	} else {
		updateInfoAvailable(r);
	}
}

void FinderWindow::updateInfoAvailable(QNetworkReply *r) {

	if (r->error() != QNetworkReply::NoError)
		return;

	QString latest(r->readAll());
	if (latest != QApplication::applicationVersion()) {
		QNetworkRequest request(QUrl("https://raw.githubusercontent.com/hussamh10/fetch/gh-pages/update.zip"));
		manager->get(request);
		trayIcon->showMessage("Fetch", "Downloading updates...");
	}
}

void FinderWindow::updateDownloaded(QNetworkReply *r) {

	if (r->error() != QNetworkReply::NoError)
		return;

	QFile tmp;
	tmp.setFileName(Settings::getInstance()->getAppDir().absolutePath().append("/update.zip"));
	tmp.open(QFile::WriteOnly);
	tmp.write(r->readAll());
	tmp.close();

	(new QProcess(this))->startDetached("FetchUpdate.exe");
	QApplication::quit();
}

void FinderWindow::resetSearch() {
	ignoreResults = true;
	clearResults();
	ui->scrollAreaContents->hide();
	ui->searchBar->clear();
	ui->searchBar->setFocus();
}

void FinderWindow::clearResults() {
	resultCount = 0;
	QLayoutItem* child;
	while ((child = ui->scrollAreaContents->layout()->takeAt(0)) != 0) {
		delete child->widget();
		delete child;
	}
}

void FinderWindow::launch() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(QObject::sender()->property("path").toString()));
	hide();
}

void FinderWindow::toggleWindow() {
	resetSearch();
	if (this->isHidden()) {
		show();
		activateWindow();
        ui->searchBar->setFocus();
	} else {
        hide();
	}
}

void FinderWindow::search(QString query) {
	pyproc->write(query.toStdString().c_str());
	pyproc->write("\r\n");
}

void FinderWindow::etchButtonText(QPushButton *button, QString &name, QString &path) {
	QTextDocument Text;
	QList<QString> color = getThemeFontColor().split(",");
	Text.setHtml("<font face='Roboto Cn' color="+color[0]+" size=5>" +
				 name +
				 "</font>&nbsp;<font face='Roboto Cn' color="+color[1]+" size=4><i>"+
				 path +
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

void FinderWindow::setTheme(Theme t) {
	setStyleSheet(getGlobalStyleSheet() + getThemedStyleSheet(t));
}

void FinderWindow::scrollToTop() {
	ui->scrollArea->verticalScrollBar()->setValue(0);
}

void FinderWindow::scrollToBottom() {
	ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
}

void FinderWindow::appendResult(QString name, QString path) {
	QPushButton *button = new QPushButton(ui->scrollAreaContents);
	etchButtonText(button, name, path);
	button->setProperty("path", path);
	button->setDefault(true);
	connect(button, SIGNAL(clicked()), this, SLOT(launch()));
	resultCount++;
	if (resultCount == 1) {
		// first result
		button->setStyleSheet("border-top: none;");
		ui->scrollAreaContents->show();
		// delete 'no result' label
		QLayoutItem *item = ui->scrollAreaContents->layout()->takeAt(0);
		delete item->widget();
		delete item;
	}
	ui->scrollAreaContents->layout()->addWidget(button);
}

void FinderWindow::keyPressEvent(QKeyEvent *e) {
    if (!ui->searchBar->hasFocus()) {
		if (e->key() == Qt::Key_Escape) {
			resetSearch();
		} else {
			ui->searchBar->setFocus();
			// send key event to search bar
			ui->searchBar->event(e);
		}
	} else if (ui->searchBar->hasFocus() && e->key() == Qt::Key_Down) {
		QCoreApplication::postEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier));
		scrollToTop();
	} else if (ui->searchBar->hasFocus() && e->key() == Qt::Key_Up) {
		QCoreApplication::postEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::ShiftModifier));
		scrollToBottom();
	} else {
		QMainWindow::keyPressEvent(e);
	}
}

void FinderWindow::pyProcOutputAvailable() {
	while (pyproc->canReadLine()) {
		QString str(pyproc->readLine());
		if (!indexed && str.trimmed() == ":indexed") {
            ui->searchBar->setPlaceholderText("🔍 Search");
            ui->searchBar->setAlignment(Qt::AlignLeft);
            ui->searchBar->setEnabled(true);
			indexed = true;
		} else if (str.startsWith(":")) {
			if (str.remove(0,1) == ui->searchBar->text()) {
				// make sure results match the current query
				ignoreResults = false;
			}
			clearResults();
			ui->scrollAreaContents->layout()->addWidget(createNrLabel());
		} else if (indexed && !ignoreResults) {
			QList<QString> list = str.split('|');
			appendResult(list[0], list[1].trimmed());
		}
	}
}

void FinderWindow::runIndexer() {
	(new QProcess(this))->start("index.exe");
}

void FinderWindow::runUpdater() {
	QNetworkRequest request(QUrl("https://raw.githubusercontent.com/hussamh10/fetch/gh-pages/latest-release"));
	manager->get(request);
}

void FinderWindow::newInstance() {
	toggleWindow();
}

void FinderWindow::exit() {
	QApplication::quit();
}

void FinderWindow::init() {
	Settings::getInstance()->load();

	initLocalServer();
	initUI();
	initWindowSize();
	initTray();
	initPyProcess();
	initIndexer();
	initUpdater();

	setTheme(Settings::getInstance()->getCurrentTheme());
	Settings::getInstance()->registerHotKey();
	connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(initWindowSize()));
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleReply(QNetworkReply*)));
}

void FinderWindow::on_searchBar_returnPressed() {
	if (resultCount > 0) {
		((QPushButton*)ui->scrollAreaContents->layout()->itemAt(0)->widget())->animateClick();
	}
}

void FinderWindow::on_searchBar_textEdited(const QString &arg1) {
    ignoreResults = false;
    if (arg1.isEmpty()) {
		resetSearch();
		return;
	}
    scrollToTop();
    search(arg1);
}
