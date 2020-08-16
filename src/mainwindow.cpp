#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	// Set application theme (dark theme).
	// `QAppliction::setStyle` works, `this->setStyle` does not.
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QApplication::setPalette(Palettes::dark());
	this->setStyleSheet("QToolTip { color: #d0d0d0; background-color: #3c0d56; border: 1px solid #8a5ea2; }");
	// tooltip isn't styled by QPalette--getting overridden by something?

	ui->setupUi(this);

	// Set up the last tab as a "new tab" button.
	ui->tabWidget->setTabEnabled(0, false);
	QPushButton* button_tab_add = new QPushButton("+", ui->tabWidget);
	button_tab_add->setFixedSize(26, 22);
	QFont font_tab_add = button_tab_add->font();
	font_tab_add.setPointSize(14);
	font_tab_add.setBold(true);
	button_tab_add->setFont(font_tab_add);
	ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, button_tab_add);
	QObject::connect(
			button_tab_add, &QPushButton::clicked,
			this, &MainWindow::add_tab );

	// Add a default starting widget if there aren't any saved ones.
	if (ui->tabWidget->count() == 1) {
		add_tab();
	}

	// Set up the tab closing logic (Qt doesn't handle this by default).
	QObject::connect(
			ui->tabWidget, &QTabWidget::tabCloseRequested,
			this, &MainWindow::remove_tab );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::add_tab()
{

	int tab_count = ui->tabWidget->count();
	QWidget* page_new = new QWidget(this);
	page_new->setContentsMargins(4, 4, 4, 4);
	page_new->setLayout(new QHBoxLayout());
	Console* console = new Console(page_new);
	page_new->layout()->addWidget(console->get_ui_widget());

	int tab_i = tab_count - 1;
	ui->tabWidget->insertTab(tab_i, page_new, "Console");
	ui->tabWidget->setCurrentIndex(tab_i);
	QObject::connect(
			console, &Console::exe_updated,
			this, [=](QString name) {
				for (int i=0; i<ui->tabWidget->count()-1; i++) {
					if (ui->tabWidget->widget(i) == page_new) {
						ui->tabWidget->setTabText(i, name);
						break;
					}
				}
			} );
}

void MainWindow::remove_tab(int index)
{
	ui->tabWidget->removeTab(index);
	// Always make sure there's at least one blank console tab;
	// this also prevents the disabled "new tab" tab from showing.
	if (ui->tabWidget->count() == 1) {
		add_tab();
	}
}
