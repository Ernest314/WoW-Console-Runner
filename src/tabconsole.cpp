#include "tabconsole.h"
#include "ui_tabconsole.h"

TabConsole::TabConsole(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TabConsole)
{
	ui->setupUi(this);
}

TabConsole::~TabConsole()
{
	delete ui;
}
