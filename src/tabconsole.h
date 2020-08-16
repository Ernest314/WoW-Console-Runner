#ifndef TABCONSOLE_H
#define TABCONSOLE_H

#include <QPlainTextEdit>
#include <QWidget>

#include "ui_tabconsole.h"

namespace Ui {
class TabConsole;
}

class TabConsole : public QWidget
{
	Q_OBJECT

public:
	Ui::TabConsole* ui;

	explicit TabConsole(QWidget *parent = nullptr);
	~TabConsole();
};

#endif // TABCONSOLE_H
