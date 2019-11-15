#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMenu>
#include <QProcess>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTextStream>

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_button_exe_polybius_clicked();
	void on_button_exe_irene_clicked();
	void on_button_clear_polybius_clicked()	{ clear_polybius(); }
	void on_button_clear_irene_clicked()	{ clear_irene(); }
	void on_button_run_polybius_clicked()	{ run_polybius(); }
	void on_button_run_irene_clicked()		{ run_irene(); }
	void on_button_stop_polybius_clicked()	{ stop_polybius(); }
	void on_button_stop_irene_clicked()		{ stop_irene(); }

	QString read_path_polybius();
	QString read_path_irene();
	void write_path_polybius(QString path);
	void write_path_irene(QString path);

	void contextMenu_polybius(const QPoint& pos);
	void contextMenu_irene(const QPoint& pos);

	void set_buttons_polybius(QProcess::ProcessState state);
	void set_buttons_irene(QProcess::ProcessState state);

	void clear_polybius();
	void clear_irene();

	void run_polybius();
	void run_irene();

	void stop_polybius();
	void stop_irene();

	void read_polybius();
	void read_irene();

private:
	Ui::MainWindow* ui;
	QProcess* process_polybius;
	QProcess* process_irene;
	QTextStream log_polybius;
	QTextStream log_irene;

	const unsigned int lines_buffer = 1024;	// seems like this is default for standard terminals
	const unsigned int chars_buffer = 80 * lines_buffer;	// standard terminal width

	const QString path_appdata =
			QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/";
	const QString path_saved_paths = path_appdata + "paths.txt";
	const QString path_logs = path_appdata + "Logs/";
	const QString prefix_polybius = "polybius:";
	const QString prefix_irene = "irene:";

	QPalette dark_palette();

	QString get_dir_of_file(QString path);
};

#endif // MAINWINDOW_H
