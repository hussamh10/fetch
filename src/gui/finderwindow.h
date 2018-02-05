#ifndef FINDERWINDOW_H
#define FINDERWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>


namespace Ui {
class FinderWindow;
}

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

private slots:
    void initWindowSize();
	void searchResult();
	void launch();
	void newConnection();
	void on_searchBar_textEdited(const QString &arg1);
	void exit();

    void on_searchBar_returnPressed();

private:
    Ui::FinderWindow *ui;
	bool ignoreResults;
	int resultCount;

	void initUI();
	void initTray();
	void initFont();
	void initPyProcess();
	void initLocalServer();

	void clearResults();
	void search(QString query);
	void addResult(QString name, QString path);

	void killProcess();
	void revertSearch();
	void toggleWindow();

	QFont resultFont;
	QProcess *pyproc;
	QSystemTrayIcon *trayIcon;

	QLocalServer localServer;
	QLocalSocket *localSocket;

	static const QString name;
    bool indexed;

};

#endif // FINDERWINDOW_H
