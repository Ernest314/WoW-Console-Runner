#ifndef TABCONSOLE_H
#define TABCONSOLE_H

#include <QDir>
#include <QMenu>
#include <QPlainTextEdit>
#include <QProcess>
#include <QString>
#include <QWidget>

#include "ui_consolehost.h"

namespace Ui {
class ConsoleHost;
}

class ConsoleHost : public QWidget
{
	Q_OBJECT

public:
	Ui::ConsoleHost* ui;

	explicit ConsoleHost(QWidget *parent = nullptr);
	~ConsoleHost();

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
