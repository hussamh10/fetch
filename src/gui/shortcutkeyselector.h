#ifndef SHORTCUTKEYSELECTOR_H
#define SHORTCUTKEYSELECTOR_H

#include <QDialog>

namespace Ui {
class ShortcutKeySelectorSelector;
}

class ShortcutKeySelectorSelector : public QDialog
{
	Q_OBJECT

public:
	explicit ShortcutKeySelectorSelector(QWidget *parent = 0);
	~ShortcutKeySelectorSelector();

private:
	Ui::ShortcutKeySelector *ui;
	initComboBox();
};

#endif // SHORTCUTKEYSELECTOR_H
