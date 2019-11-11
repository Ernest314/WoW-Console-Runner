#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>

#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QProcess>
#include <QStyleFactory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
	typedef std::string string;

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

	void clear_polybius();
	void clear_irene();

	void run_polybius();
	void run_irene();

	void read_polybius();
	void read_irene();

private:
	Ui::MainWindow* ui;
	QProcess* process_polybius;
	QProcess* process_irene;

	const unsigned int lines_buffer = 1024;	// seems like this is default for standard terminals
	const unsigned int chars_buffer = 80 * lines_buffer;	// standard terminal width

	const string dir_polybius = "C:\\Users\\Ernest\\Documents\\Programming\\Projects\\polybius\\Polybius\\bin\\Release\\";
	const string dir_irene = "C:\\Users\\Ernest\\Documents\\Programming\\Projects\\irene\\Irene\\bin\\Release\\";
	const string path_polybius = dir_polybius + "Polybius.exe";
	const string path_irene = dir_irene + "Irene.exe";

	QPalette dark_palette();

	QString get_working_dir(QString path);
};

#endif // MAINWINDOW_H
