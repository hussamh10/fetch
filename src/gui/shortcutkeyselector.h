#ifndef SHORTCUTKEYSELECTOR_H
#define SHORTCUTKEYSELECTOR_H

#include <QDialog>

namespace Ui {
class ShortcutKeySelector;
}

class ShortcutKeySelector : public QDialog
{
	Q_OBJECT

public:
	explicit ShortcutKeySelector(QWidget *parent = 0);
	~ShortcutKeySelector();

	struct ShortcutKey {
		ShortcutKey(){}
		ShortcutKey(bool ctrl, bool alt, bool shift, int vk, QString keyname)
			:ctrl(ctrl), alt(alt), shift(shift), vk(vk), keyname(keyname) {}

		bool ctrl, alt, shift;
		int vk;
		QString keyname;

		int getModifiers();
		QString toString();
	};

private slots:
	void on_saveBtn_clicked();

	void on_closeBtn_clicked();

private:
	Ui::ShortcutKeySelector *ui;
	void initComboBox();
	void init();
};

#endif // SHORTCUTKEYSELECTOR_H
