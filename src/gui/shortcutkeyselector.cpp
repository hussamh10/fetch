#include "shortcutkeyselector.h"
#include "ui_shortcutkeyselector.h"
#include "windows.h"
#include "settings.h"
#include <QKeyEvent>

ShortcutKeySelector::ShortcutKeySelector(QWidget *parent) :
	QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint),
	ui(new Ui::ShortcutKeySelector)
{
	ui->setupUi(this);
	setWindowTitle("Set shortcut key to launch Fetch");
	initComboBox();
	init();
}

ShortcutKeySelector::~ShortcutKeySelector() {
	delete ui;
}

void ShortcutKeySelector::initComboBox() {
	ui->comboBox->addItem("A", 0x41);
	ui->comboBox->addItem("B", 0x42);
	ui->comboBox->addItem("C", 0x43);
	ui->comboBox->addItem("D", 0x44);
	ui->comboBox->addItem("E", 0x45);
	ui->comboBox->addItem("F", 0x46);
	ui->comboBox->addItem("G", 0x47);
	ui->comboBox->addItem("H", 0x48);
	ui->comboBox->addItem("I", 0x49);
	ui->comboBox->addItem("J", 0x4A);
	ui->comboBox->addItem("K", 0x4B);
	ui->comboBox->addItem("L", 0x4C);
	ui->comboBox->addItem("M", 0x4D);
	ui->comboBox->addItem("N", 0x4E);
	ui->comboBox->addItem("O", 0x4F);
	ui->comboBox->addItem("P", 0x50);
	ui->comboBox->addItem("Q", 0x51);
	ui->comboBox->addItem("R", 0x52);
	ui->comboBox->addItem("S", 0x53);
	ui->comboBox->addItem("T", 0x54);
	ui->comboBox->addItem("U", 0x55);
	ui->comboBox->addItem("V", 0x56);
	ui->comboBox->addItem("W", 0x57);
	ui->comboBox->addItem("X", 0x58);
	ui->comboBox->addItem("Y", 0x59);
	ui->comboBox->addItem("Z", 0x5A);
	ui->comboBox->addItem("0", 0x30);
	ui->comboBox->addItem("1", 0x31);
	ui->comboBox->addItem("2", 0x32);
	ui->comboBox->addItem("3", 0x33);
	ui->comboBox->addItem("4", 0x34);
	ui->comboBox->addItem("5", 0x35);
	ui->comboBox->addItem("6", 0x36);
	ui->comboBox->addItem("7", 0x37);
	ui->comboBox->addItem("8", 0x38);
	ui->comboBox->addItem("9", 0x39);
	ui->comboBox->addItem("Space", 0x20);
	ui->comboBox->addItem("PgUp", 0x21);
	ui->comboBox->addItem("PgDn", 0x22);
	ui->comboBox->addItem("End", 0x23);
	ui->comboBox->addItem("Home", 0x24);
	ui->comboBox->addItem("Ins", 0x2D);
	ui->comboBox->addItem("Del", 0x2E);
	ui->comboBox->addItem("F1", 0x70);
	ui->comboBox->addItem("F2", 0x71);
	ui->comboBox->addItem("F3", 0x72);
	ui->comboBox->addItem("F4", 0x73);
	ui->comboBox->addItem("F5", 0x74);
	ui->comboBox->addItem("F6", 0x75);
	ui->comboBox->addItem("F7", 0x76);
	ui->comboBox->addItem("F8", 0x77);
	ui->comboBox->addItem("F9", 0x78);
	ui->comboBox->addItem("F10", 0x79);
	ui->comboBox->addItem("F11", 0x7A);
	ui->comboBox->addItem("F12", 0x7B);
	ui->comboBox->addItem("RShift", 0xA1);
	ui->comboBox->addItem("RCtrl", 0xA3);
	ui->comboBox->addItem("NUM0", 0x60);
	ui->comboBox->addItem("NUM1", 0x61);
	ui->comboBox->addItem("NUM2", 0x62);
	ui->comboBox->addItem("NUM3", 0x63);
	ui->comboBox->addItem("NUM4", 0x64);
	ui->comboBox->addItem("NUM5", 0x65);
	ui->comboBox->addItem("NUM6", 0x66);
	ui->comboBox->addItem("NUM7", 0x67);
	ui->comboBox->addItem("NUM8", 0x68);
	ui->comboBox->addItem("NUM9", 0x69);
}

void ShortcutKeySelector::init() {
	ShortcutKey sk = *(Settings::getInstance()->getShortcutKey());
	ui->ctrlMod->setChecked(sk.ctrl);
	ui->altMod->setChecked(sk.alt);
	ui->shiftMod->setChecked(sk.shift);
	ui->comboBox->setCurrentText(sk.keyname);
}

void ShortcutKeySelector::on_saveBtn_clicked() {
	QString name = ui->comboBox->currentText();
	int vk = ui->comboBox->currentData().toInt();
	bool ctrl = ui->ctrlMod->checkState() == Qt::Checked;
	bool alt = ui->altMod->checkState() == Qt::Checked;
	bool shift = ui->shiftMod->checkState() == Qt::Checked;
	ShortcutKeySelector::ShortcutKey key(ctrl, alt, shift, vk, name);
	Settings::getInstance()->registerHotKey(key);
	setVisible(false);
}

void ShortcutKeySelector::on_closeBtn_clicked() {
	setVisible(false);
}

int ShortcutKeySelector::ShortcutKey::getModifiers() {
	int modifier = 0;
	if (ctrl) modifier |= MOD_CONTROL;
	if (alt) modifier |= MOD_ALT;
	if (shift) modifier |= MOD_SHIFT;
	return modifier;
}

QString ShortcutKeySelector::ShortcutKey::toString() {
	QString str = "";
	if (ctrl) str.append("Ctrl + ");
	if (alt) str.append("Alt + ");
	if (shift) str.append("Shift + ");
	str.append(keyname);
	return str;
}
