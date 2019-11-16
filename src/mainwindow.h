#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QDesktopServices>
#include <QMenu>
#include <QProcess>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTextStream>

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>

#include "utils.h"
#include "console.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow* ui;
	Console* console_polybius;
	Console* console_irene;

	static QPalette dark_palette();

private slots:
	void on_button_exe_polybius_clicked()	{ console_polybius->set_exe_path(); }
	void on_button_exe_irene_clicked()		{ console_irene->set_exe_path(); }
	void on_button_clear_polybius_clicked()	{ console_polybius->clear_buffer(); }
	void on_button_clear_irene_clicked()	{ console_irene->clear_buffer(); }
	void on_button_run_polybius_clicked()	{ console_polybius->start_process(); }
	void on_button_run_irene_clicked()		{ console_irene->start_process(); }
	void on_button_stop_polybius_clicked()	{ console_polybius->stop_process(); }
	void on_button_stop_irene_clicked()		{ console_irene->stop_process(); }
	void on_button_logs_polybius_clicked()	{ console_polybius->open_logs(); }
	void on_button_logs_irene_clicked()		{ console_irene->open_logs(); }

	void set_buttons_polybius(QProcess::ProcessState state);
	void set_buttons_irene(QProcess::ProcessState state);
};

#endif // MAINWINDOW_H
