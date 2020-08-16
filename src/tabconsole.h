#ifndef TABCONSOLE_H
#define TABCONSOLE_H

#include <QMenu>
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

public slots:
	void clear_buffer();

private slots:
	void context_menu(const QPoint& pos);
};

#endif // TABCONSOLE_H
