#ifndef TABCONSOLE_H
#define TABCONSOLE_H

#include <QWidget>

namespace Ui {
class TabConsole;
}

class TabConsole : public QWidget
{
	Q_OBJECT

public:
	explicit TabConsole(QWidget *parent = nullptr);
	~TabConsole();

private:
	Ui::TabConsole *ui;
};

#endif // TABCONSOLE_H
