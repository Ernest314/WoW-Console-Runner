#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	// Set application theme (dark theme).
	// `QAppliction::setStyle` works, `this->setStyle` does not
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QApplication::setPalette(Palettes::dark());
	this->setStyleSheet("QToolTip { color: #d0d0d0; background-color: #3c0d56; border: 1px solid #8a5ea2; }");
	// tooltip isn't styled by QPalette--getting overridden by something?

	ui->setupUi(this);

//	// Set up console processes.
//	console_polybius = new Console(
//			this,
//			ui->console_polybius,
//			ui->lineEdit_exe_polybius,
//			new QProcess(parent),
//			"polybius",
//			"polybius-",
//			"Polybius.exe" );
//	console_irene = new Console(
//			this,
//			ui->console_irene,
//			ui->lineEdit_exe_irene,
//			new QProcess(parent),
//			"irene",
//			"irene-",
//			"Irene.exe" );
}

MainWindow::~MainWindow()
{
	for (Console* console : consoles) {
		delete console;
	}

	delete ui;
}
