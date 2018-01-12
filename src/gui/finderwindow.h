#ifndef FINDERWINDOW_H
#define FINDERWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>

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

private slots:
	void searchResult();
	void launch();

	void on_searchBar_textEdited(const QString &arg1);

private:
    Ui::FinderWindow *ui;
	bool ignoreResults;
	int resultCount;

	void initWindowSize();
	void initUI();
	void initTray();
	void initFont();
	void initPyProcess();

	void clearResults();
	void search(QString query);
	void addResult(QString name, QString path);

	void killProcess();
	void revertSearch();
	void keyPressEvent(QKeyEvent*);

	QFont resultFont;
	QProcess *pyproc;
	QSystemTrayIcon *trayIcon;
};

#endif // FINDERWINDOW_H
