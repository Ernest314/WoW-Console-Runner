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
	// Note: the position of the tab is hard-coded, so adding any other
	// tabs should come after this declaration.
	ui->tabWidget->setTabEnabled(0, false);
	QPushButton* button_tab_add = new QPushButton("+", ui->tabWidget);
	button_tab_add->setToolTip("Add Tab");
	button_tab_add->setFixedSize(26, 22);
	QFont font_tab_add = button_tab_add->font();
	font_tab_add.setPointSize(14);
	font_tab_add.setBold(true);
	button_tab_add->setFont(font_tab_add);
	ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, button_tab_add);
	QObject::connect(
			button_tab_add, &QPushButton::clicked,
			this, &MainWindow::add_tab_blank );

	// Read in saved paths and create corresponding tabs.
	QFile* paths = new QFile(path_paths);
	if (!paths->exists()) {
		paths = Utils::get_created_file(path_paths);
	}
	paths->open(QIODevice::ReadOnly | QIODevice::Text);

	// Create a new tab per line of paths file.
	QTextStream reader = QTextStream(paths);
	while (!reader.atEnd()) {
		add_tab(reader.readLine());
	}
	paths->close();

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
	for (std::pair<QWidget*, Console*> console : consoles) {
		delete console.second;
	}
	delete ui;
}

void MainWindow::update_saved_paths()
{
	// Create a saved paths file if it doesn't already exist.
	QFile* paths = new QFile(path_paths);
	if (!paths->exists()) {
		paths = Utils::get_created_file(path_paths);
	}
	paths->open(QIODevice::WriteOnly | QIODevice::Text);

	// Write out a path per existing tab (can be invalid).
	QTextStream writer = QTextStream(paths);
	for (int i=0; i<ui->tabWidget->count()-1; i++) {
		QWidget* page = ui->tabWidget->widget(i);
		Console* console = consoles[page];
		QString path = console->get_path();
		writer << path << "\n";
		writer.flush();
	}
	paths->close();
}

void MainWindow::add_tab(QString path)
{
	// Create a new tab with the correct children and layout,
	// then insert a new Console into said tab.
	int tab_count = ui->tabWidget->count();
	QWidget* page_new = new QWidget(this);
	page_new->setContentsMargins(4, 4, 4, 4);
	page_new->setLayout(new QHBoxLayout());
	Console* console = new Console(page_new, path);
	page_new->layout()->addWidget(console->get_host());

	int tab_i = tab_count - 1;
	ui->tabWidget->insertTab(tab_i, page_new, "");
	consoles.insert({page_new, console});

	// Set up the new tab to update its text from its child Console,
	// and initialize the tab title.
	ui->tabWidget->setCurrentIndex(tab_i);
	auto update_tab_text = [=](QString name) {
		for (int i=0; i<ui->tabWidget->count()-1; i++) {
			if (ui->tabWidget->widget(i) == page_new) {
				ui->tabWidget->setTabText(i, name);
				break;
			}
		}
	};
	update_tab_text(console->get_host()->get_exe_name());
	QObject::connect(
			console, &Console::exe_updated,
			this, update_tab_text );

	// Set up the new tab to update the saved paths.
	QObject::connect(
			console, &Console::exe_updated,
			this, &MainWindow::update_saved_paths );
}

void MainWindow::remove_tab(int index)
{
	consoles.erase(ui->tabWidget->widget(index));
	ui->tabWidget->removeTab(index);

	// Always make sure there's at least one blank console tab;
	// this also prevents the disabled "new tab" tab from showing.
	if (ui->tabWidget->count() == 1) {
		add_tab();
	}

	// Make sure "New Tab" tab isn't showing.
	int i_new = ui->tabWidget->count() - 1;
	if (ui->tabWidget->currentIndex() == i_new) {
		ui->tabWidget->setCurrentIndex(i_new - 1);
	}

	// Update the files with all the saved tabs.
	update_saved_paths();
}
