#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QStyleFactory>

#include <string>

using std::string;

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

	string read_path_polybius();
	string read_path_irene();
	void write_path_polybius(string path);
	void write_path_irene(string path);

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

	const unsigned int lines_buffer = 1024;	// seems like this is default for standard terminals
	const unsigned int chars_buffer = 80 * lines_buffer;	// standard terminal width

	const string path_saved_paths = "paths.txt";
	const string path_logs = "Logs\\";
	const string prefix_polybius = "polybius:";
	const string prefix_irene = "irene:";

	QPalette dark_palette();

	QString get_dir_of_file(QString path);
};

#endif // MAINWINDOW_H
