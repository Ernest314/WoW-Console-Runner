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
	this->setStyleSheet("QToolTip { color: #d0d0d0; background-color: #3c0d56; border: 1px solid #8a5ea2; }");
	// tooltip isn't styled by QPalette--getting overridden by something

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

QPalette MainWindow::dark_palette()
{
	QPalette palette;

	// QPalette::Active
	{
		palette.setColor(QPalette::Window,			QColor( 39, 39, 39));
		palette.setColor(QPalette::WindowText,		QColor(208,208,208));
		palette.setColor(QPalette::Base,			QColor( 26, 26, 26));
		palette.setColor(QPalette::AlternateBase,	QColor( 60, 13, 86));
		palette.setColor(QPalette::ToolTipBase,		QColor( 39, 39, 39));
		palette.setColor(QPalette::ToolTipText,		QColor(208,208,208));
		palette.setColor(QPalette::PlaceholderText,	QColor(138, 94,162));
		palette.setColor(QPalette::Text,			QColor(208,208,208));
		palette.setColor(QPalette::Button,			QColor( 39, 39, 39));
		palette.setColor(QPalette::ButtonText,		QColor(208,208,208));
		palette.setColor(QPalette::BrightText,		QColor(138, 94,162));

		palette.setColor(QPalette::Light,			QColor( 26, 26, 26));
		palette.setColor(QPalette::Midlight,		QColor( 32, 32, 32));
		palette.setColor(QPalette::Dark,			QColor( 52, 52, 52));
		palette.setColor(QPalette::Mid,				QColor(130,130,130));
		palette.setColor(QPalette::Shadow,			QColor(208,208,208));

		palette.setColor(QPalette::Highlight,		QColor(138, 94,162));
		palette.setColor(QPalette::HighlightedText,	QColor( 26, 26, 26));

		palette.setColor(QPalette::Link,			QColor(178, 69,102));
		palette.setColor(QPalette::LinkVisited,		QColor( 67, 67,140));
	}

	// QPalette::Inactive
	{
//		palette.setColor(QPalette::Inactive, QPalette::Window,			QColor( 39, 39, 39));
//		palette.setColor(QPalette::Inactive, QPalette::WindowText,		QColor(208,208,208));
//		palette.setColor(QPalette::Inactive, QPalette::Base,			QColor( 26, 26, 26));
//		palette.setColor(QPalette::Inactive, QPalette::AlternateBase,	QColor( 60, 13, 86));
//		palette.setColor(QPalette::Inactive, QPalette::ToolTipBase,		QColor( 39, 39, 39));
//		palette.setColor(QPalette::Inactive, QPalette::ToolTipText,		QColor(208,208,208));
//		palette.setColor(QPalette::Inactive, QPalette::PlaceholderText,	QColor(138, 94,162));
//		palette.setColor(QPalette::Inactive, QPalette::Text,			QColor(208,208,208));
//		palette.setColor(QPalette::Inactive, QPalette::Button,			QColor( 39, 39, 39));
//		palette.setColor(QPalette::Inactive, QPalette::ButtonText,		QColor(208,208,208));
//		palette.setColor(QPalette::Inactive, QPalette::BrightText,		QColor(138, 94,162));

//		palette.setColor(QPalette::Inactive, QPalette::Light,			QColor( 26, 26, 26));
//		palette.setColor(QPalette::Inactive, QPalette::Midlight,		QColor( 32, 32, 32));
//		palette.setColor(QPalette::Inactive, QPalette::Dark,			QColor( 52, 52, 52));
//		palette.setColor(QPalette::Inactive, QPalette::Mid,				QColor(130,130,130));
//		palette.setColor(QPalette::Inactive, QPalette::Shadow,			QColor(208,208,208));

//		palette.setColor(QPalette::Inactive, QPalette::Highlight,		QColor(138, 94,162));
//		palette.setColor(QPalette::Inactive, QPalette::HighlightedText,	QColor( 26, 26, 26));

//		palette.setColor(QPalette::Inactive, QPalette::Link,			QColor(178, 69,102));
//		palette.setColor(QPalette::Inactive, QPalette::LinkVisited,		QColor( 67, 67,140));
	}

	// QPalette::Disabled
	{
//		palette.setColor(QPalette::Disabled, QPalette::Window,			QColor( 39, 39, 39));
		palette.setColor(QPalette::Disabled, QPalette::WindowText,		QColor( 96, 96, 96));
//		palette.setColor(QPalette::Disabled, QPalette::Base,			QColor( 26, 26, 26));
//		palette.setColor(QPalette::Disabled, QPalette::AlternateBase,	QColor( 60, 13, 86));
//		palette.setColor(QPalette::Disabled, QPalette::ToolTipBase,		QColor( 39, 39, 39));
		palette.setColor(QPalette::Disabled, QPalette::ToolTipText,		QColor( 96, 96, 96));
//		palette.setColor(QPalette::Disabled, QPalette::PlaceholderText,	QColor(138, 94,162));
		palette.setColor(QPalette::Disabled, QPalette::Text,			QColor( 96, 96, 96));
//		palette.setColor(QPalette::Disabled, QPalette::Button,			QColor( 39, 39, 39));
		palette.setColor(QPalette::Disabled, QPalette::ButtonText,		QColor( 96, 96, 96));
		palette.setColor(QPalette::Disabled, QPalette::BrightText,		QColor(106,106,169));

//		palette.setColor(QPalette::Disabled, QPalette::Light,			QColor( 26, 26, 26));
//		palette.setColor(QPalette::Disabled, QPalette::Midlight,		QColor( 32, 32, 32));
//		palette.setColor(QPalette::Disabled, QPalette::Dark,			QColor( 52, 52, 52));
//		palette.setColor(QPalette::Disabled, QPalette::Mid,				QColor(130,130,130));
//		palette.setColor(QPalette::Disabled, QPalette::Shadow,			QColor(208,208,208));

//		palette.setColor(QPalette::Disabled, QPalette::Highlight,		QColor(138, 94,162));
//		palette.setColor(QPalette::Disabled, QPalette::HighlightedText,	QColor( 26, 26, 26));

//		palette.setColor(QPalette::Disabled, QPalette::Link,			QColor(178, 69,102));
//		palette.setColor(QPalette::Disabled, QPalette::LinkVisited,		QColor( 67, 67,140));
	}

	return palette;
}
