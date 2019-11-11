#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow),
	process_polybius(new QProcess(parent)),
	process_irene(new QProcess(parent))
{
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	ui->setupUi(this);

	QObject::connect(
			process_polybius,
			&QProcess::readyReadStandardOutput,
			this,
			&MainWindow::read_polybius
	);
	QObject::connect(
			process_irene,
			&QProcess::readyReadStandardOutput,
			this,
			&MainWindow::read_irene
	);

	ui->lineEdit_exe_polybius->setText(path_polybius.c_str());
	ui->lineEdit_exe_irene->setText(path_irene.c_str());
}

MainWindow::~MainWindow()
{
	delete process_polybius;
	delete process_irene;

	delete ui;
}


void MainWindow::on_button_exe_polybius_clicked()
{
	QString path =
			QFileDialog::getOpenFileName(
				this,
				"Polybius.exe path",
				QDir::homePath(),
				"Executable (*.exe)"
			);
	if (path != nullptr)
		ui->lineEdit_exe_polybius->setText(path);
}

void MainWindow::on_button_exe_irene_clicked()
{
	QString path =
			QFileDialog::getOpenFileName(
				this,
				"Irene.exe path",
				QDir::homePath(),
				"Executable (*.exe)"
			);
	if (path != nullptr)
		ui->lineEdit_exe_irene->setText(path);
}

void MainWindow::on_button_run_polybius_clicked()
{
	ui->console_polybius->clear();
	if (process_polybius->program() == nullptr) {
		process_polybius->close();
	}
	QString path = ui->lineEdit_exe_polybius->text();
	process_polybius->setProgram(path);
	process_polybius->setWorkingDirectory(get_working_dir(path));
	process_polybius->setReadChannel(QProcess::StandardOutput);
	process_polybius->start();
}

void MainWindow::on_button_run_irene_clicked()
{
	ui->console_irene->clear();
	if (process_irene->program() == nullptr) {
		process_irene->close();
	}
	QString path = ui->lineEdit_exe_irene->text();
	process_irene->setProgram(path);
	process_irene->setWorkingDirectory(get_working_dir(path));
	process_irene->setReadChannel(QProcess::StandardOutput);
	process_irene->start();
}

void MainWindow::read_polybius()
{
	while (process_polybius->canReadLine()) {
		QString line = process_polybius->readLine();
		QString buffer = ui->console_polybius->toPlainText();
		ui->console_polybius->setPlainText(buffer + line);
		// TODO: only keep certain amount of buffer
	}
}

void MainWindow::read_irene()
{
	while (process_irene->canReadLine()) {
		QString line = process_irene->readLine();
		QString buffer = ui->console_irene->toPlainText();
		ui->console_irene->setPlainText(buffer + line);
		// TODO: only keep certain amount of buffer
	}
}

QString MainWindow::get_working_dir(QString path)
{
	QDir dir = QDir(path);
	dir.cdUp();
	return dir.path();
}
