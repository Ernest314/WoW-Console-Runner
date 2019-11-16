#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	// Set application theme (dark theme).
	// `QAppliction::setStyle` works, `this->setStyle` does not
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QPalette palette = dark_palette();
	QApplication::setPalette(palette);
	this->setStyleSheet("QToolTip { color: #ffffff; background-color: #353535; border: 1px solid white; }");
	// tooltip isn't styled by QPalette

	ui->setupUi(this);

	console_polybius = new Console(
			this,
			ui->console_polybius,
			ui->lineEdit_exe_polybius,
			new QProcess(parent),
			"polybius:",
			"polybius-",
			"Polybius.exe" );
	console_irene = new Console(
			this,
			ui->console_irene,
			ui->lineEdit_exe_irene,
			new QProcess(parent),
			"irene:",
			"irene-",
			"Irene.exe" );

	// Change button state based on if processes are running.
	QObject::connect(
			console_polybius, &Console::state_changed,
			this, &MainWindow::set_buttons_polybius);
	QObject::connect(
			console_irene, &Console::state_changed,
			this, &MainWindow::set_buttons_irene);
}

MainWindow::~MainWindow()
{
	delete console_polybius;
	delete console_irene;

	delete ui;
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

void MainWindow::set_buttons_polybius(QProcess::ProcessState state)
{
	switch (state) {
	case QProcess::Starting : break;	// don't care about this
	case QProcess::Running :
		ui->button_stop_polybius->setEnabled(true);
		ui->button_run_polybius->setText("Restart");
		break;
	case QProcess::NotRunning :
		ui->button_stop_polybius->setEnabled(false);
		ui->button_run_polybius->setText("Run");
		break;
	}
}
void MainWindow::set_buttons_irene(QProcess::ProcessState state)
{
	switch (state) {
	case QProcess::Starting : break;	// don't care about this
	case QProcess::Running :
		ui->button_stop_irene->setEnabled(true);
		ui->button_run_irene->setText("Restart");
		break;
	case QProcess::NotRunning :
		ui->button_stop_irene->setEnabled(false);
		ui->button_run_irene->setText("Run");
		break;
	}
}
