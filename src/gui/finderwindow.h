#ifndef FINDERWINDOW_H
#define FINDERWINDOW_H

#include <QMainWindow>
#include "settings.h"

namespace Ui {
	class FinderWindow;
}

class QPushButton;
class QProcess;
class QSystemTrayIcon;
class QLocalServer;
class QLocalSocket;
class QTimer;
class QLabel;

class FinderWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit FinderWindow(QWidget *parent = 0);
	~FinderWindow();
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
	bool isAlreadyRunning();
	void startListening();
	void init();

protected:
	void keyPressEvent(QKeyEvent* e);

private slots:
	void pyProcOutputAvailable();
	void runIndexer();
	void newInstance();
	void exit();
	void launch();
	void initWindowSize();
	void setTheme();
	void toggleRunOnStartup(bool);

	void on_searchBar_returnPressed();
	void on_searchBar_textEdited(const QString &arg1);

private:
	Ui::FinderWindow *ui;

	void initUI();
	void initTray();
	void initPyProcess();
	void initLocalServer();
	void initIndexer();

	QString getGlobalStyleSheet();
	QString getThemedStyleSheet(Theme t);
	QString getThemeFontColor();
	QString getPyProcessCommand(QString scriptname);

	QLabel* createNrLabel();
	void resetSearch();
	void clearResults();
	void toggleWindow();
	void search(QString query);
	void appendResult(QString name, QString path);
	void etchButtonText(QPushButton* btn, QString& name, QString& path);
	void setTheme(Theme t);
	void scrollToTop();
	void scrollToBottom();

	static const QString SERVERNAME;

	bool ignoreResults;
	bool indexed;
	int resultCount;

	QProcess *pyproc;
	QSystemTrayIcon *trayIcon;
	QLocalServer *localServer;
	QLocalSocket *localSocket;
	QTimer *timer;

	Theme *theme;
};

#endif // FINDERWINDOW_H
