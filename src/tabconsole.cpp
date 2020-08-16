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
}

TabConsole::~TabConsole()
{
	delete ui;
}

void TabConsole::clear_buffer()
{
	ui->console->setPlainText("");
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
