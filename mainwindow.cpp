#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow),
	process_polybius(new QProcess(parent)),
	process_irene(new QProcess(parent))
{
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QPalette palette = dark_palette();
	QApplication::setPalette(palette);
	this->setStyleSheet("QToolTip { color: #ffffff; background-color: #353535; border: 1px solid white; }");

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

QPalette MainWindow::dark_palette()
{
	// https://gist.github.com/QuantumCD/6245215
	QPalette palette;

	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(25, 25, 25));
	palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	palette.setColor(QPalette::ToolTipBase, QColor(206, 206, 206));
	palette.setColor(QPalette::ToolTipText, QColor(206, 206, 206));
	palette.setColor(QPalette::PlaceholderText, QColor(130, 130, 130));
	palette.setColor(QPalette::Text, QColor(206, 206, 206));
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, QColor(206, 206, 206));
	palette.setColor(QPalette::BrightText, Qt::red);

	palette.setColor(QPalette::Light, QColor(40, 40, 40));
	palette.setColor(QPalette::Midlight,QColor(45, 45, 45) );
	palette.setColor(QPalette::Dark, QColor(66, 66, 66));
	palette.setColor(QPalette::Mid, QColor(60, 60, 60));
	palette.setColor(QPalette::Shadow, QColor(85, 85, 85));

	palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	palette.setColor(QPalette::HighlightedText, QColor(25, 25, 25));

	palette.setColor(QPalette::Link, QColor(42, 130, 218));
	palette.setColor(QPalette::LinkVisited, QColor(130, 42, 218));

	return palette;
}

QString MainWindow::get_working_dir(QString path)
{
	QDir dir = QDir(path);
	dir.cdUp();
	return dir.path();
}
