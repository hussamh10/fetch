#ifndef FINDERWINDOW_H
#define FINDERWINDOW_H

#include <QMainWindow>
#include <QLocalServer>

class QPushButton;
class QProcess;
class QSystemTrayIcon;
class QLocalSocket;
class QTimer;

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
    void reindex();

    void on_searchBar_returnPressed();

protected:
    void keyPressEvent(QKeyEvent* e);

private:
    Ui::FinderWindow *ui;
	bool ignoreResults;
	int resultCount;

	void initUI();
	void initTray();
	void initPyProcess();
	void initLocalServer();
    void initIndexer();

	void clearResults();
	void search(QString query);
	void addResult(QString name, QString path);

	void killProcess();
	void revertSearch();
	void toggleWindow();
    void resetSize();

    void stylizeButton(QPushButton *btn, QString text, QString subtext);

	QProcess *pyproc;
	QSystemTrayIcon *trayIcon;

	QLocalServer localServer;
	QLocalSocket *localSocket;

    QTimer *timer;

	static const QString name;
	QKeyEvent *tab;
	QKeyEvent *shift_tab;
    bool indexed;

};

#endif // FINDERWINDOW_H
