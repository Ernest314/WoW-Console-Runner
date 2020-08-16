#ifndef TABCONSOLE_H
#define TABCONSOLE_H

#include <QDir>
#include <QMenu>
#include <QPlainTextEdit>
#include <QProcess>
#include <QString>
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

	bool is_path_valid();
	QString get_path()	{ return ui->lineEdit_path->text(); }
	QString get_exe_name();

public slots:
	void clear_buffer() { ui->console->setPlainText(""); }
	void validate_path();
	void update_buttons(QProcess::ProcessState state);

private:
	inline static const QString invalid_path_stylesheet =
			"QLineEdit {"
			"color: white;"
			"background-color: orangered;"
			"}" ;

private slots:
	void context_menu(const QPoint& pos);

	void on_button_clear_clicked() { clear_buffer(); }
};

#endif // TABCONSOLE_H
