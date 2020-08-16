#include "tabconsole.h"
#include "ui_tabconsole.h"

TabConsole::TabConsole(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabConsole)
{
	ui->setupUi(this);

	// Grab context menu event to add "clear" option.
	QObject::connect(
			ui->console, &QPlainTextEdit::customContextMenuRequested,
			this, &TabConsole::context_menu );

	// Validate line edit's path every time it's updated.
	// This is important because the line edit's path is the authoritative
	// source of what executable is being run in the console.
	QObject::connect(
			ui->lineEdit_path, &QLineEdit::editingFinished,
			this, &TabConsole::validate_path );
}

TabConsole::~TabConsole()
{
	delete ui;
}

bool TabConsole::is_path_valid()
{
	QDir path_check = QDir(get_path());
	// QDir::exists only checks for files if absolute path is given
	if (path_check.exists(get_path()) && get_path().endsWith(".exe"))
		return true;
	else
		return false;
}

QString TabConsole::get_exe_name()
{
	QString exe_name = "";
	if (is_path_valid()) {
		int i = get_path().lastIndexOf("/");
		exe_name = get_path().mid(i);	// QString doesn't have substr
		exe_name.remove(".exe");		// is_path_valid() guarantees this
	}

	// Since this function is just looking for a textual representation
	// of the .exe file, if one isn't found, placeholder text makes sense
	// (instead of returning null).
	if (exe_name == "") exe_name = "UNKNOWN";
	return exe_name;
}

void TabConsole::validate_path()
{
	// don't highlight text box if empty
	if (is_path_valid() || get_path() == "") {
		// reset stylesheet
		ui->lineEdit_path->setStyleSheet("");
	} else {
		// highlight text
		ui->lineEdit_path->setStyleSheet(invalid_path_stylesheet);
	}
}

void TabConsole::update_buttons(QProcess::ProcessState state)
{
	switch (state) {
	case QProcess::Starting : break;	// don't care about this
	case QProcess::Running :
		ui->button_stop->setEnabled(true);
		ui->button_run->setText("Restart");
		// Cosmetic change. Slot always attempts to stop process first.
		break;
	case QProcess::NotRunning :
		ui->button_stop->setEnabled(false);
		ui->button_run->setText("Run");
		break;
	}
}

void TabConsole::context_menu(const QPoint& pos)
{
	// Take standard (QPlainTextEdit) context menu, and return the same
	// menu but with additional items inserted.
	QMenu* menu = ui->console->createStandardContextMenu(pos);
	menu->addAction("Clea&r", this, &TabConsole::clear_buffer);
	menu->exec(ui->console->mapToGlobal(pos));
	delete menu;
}
