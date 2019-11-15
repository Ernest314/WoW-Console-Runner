#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow),
	process_polybius(new QProcess(parent)),
	process_irene(new QProcess(parent))
{
	// Set application theme (dark theme).
	// `QAppliction::setStyle` works, `this->setStyle` does not
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QPalette palette = dark_palette();
	QApplication::setPalette(palette);
	this->setStyleSheet("QToolTip { color: #ffffff; background-color: #353535; border: 1px solid white; }");
	// tooltip isn't styled by QPalette

	ui->setupUi(this);

	// Change button state based on if processes are running.
	QObject::connect(
			process_polybius, &QProcess::stateChanged,
			this, &MainWindow::set_buttons_polybius);
	QObject::connect(
			process_irene, &QProcess::stateChanged,
			this, &MainWindow::set_buttons_irene);

	// Redirect process output to console widgets.
	QObject::connect(
			process_polybius, &QProcess::readyReadStandardOutput,
			this, &MainWindow::read_polybius );
	QObject::connect(
			process_irene, &QProcess::readyReadStandardOutput,
			this, &MainWindow::read_irene );

	// Grab context menu event on consoles (to add "clear" option).
	QObject::connect(
			ui->console_polybius, &QPlainTextEdit::customContextMenuRequested,
			this, &MainWindow::contextMenu_polybius );
	QObject::connect(
			ui->console_irene, &QPlainTextEdit::customContextMenuRequested,
			this, &MainWindow::contextMenu_irene );

	// Initialize lineEdit text with the paths to their
	// corresponding .exe files.
	ui->lineEdit_exe_polybius->setText(read_path_polybius());
	ui->lineEdit_exe_irene->setText(read_path_irene());
}

MainWindow::~MainWindow()
{
	delete process_polybius;
	delete process_irene;

	delete ui;
}

QString MainWindow::read_path_polybius()
{
	if (!QFile::exists(path_saved_paths)) {
		return QDir::homePath();
	}
	QFile* file = new QFile(path_saved_paths);
	file->open(QIODevice::ReadOnly | QIODevice::Text);
	QString data = file->readAll();
	QStringList lines = data.split("\n", QString::SkipEmptyParts);
	QString path = "";
	for (QString line : lines) {
		if (line.startsWith(prefix_polybius)) {
			path = line.remove(prefix_polybius);
		}
	}
	if (path == "") {
		path = QDir::homePath();
	}
	return path;
}
QString MainWindow::read_path_irene()
{
	if (!QFile::exists(path_saved_paths)) {
		return QDir::homePath();
	}
	QFile* file = new QFile(path_saved_paths);
	file->open(QIODevice::ReadOnly | QIODevice::Text);
	QString data = file->readAll().trimmed();
	QStringList lines = data.split("\n", QString::SkipEmptyParts);
	QString path = "";
	for (QString line : lines) {
		if (line.startsWith(prefix_irene)) {
			path = line.remove(prefix_irene);
		}
	}
	if (path == "") {
		path = QDir::homePath();
	}
	return path;
}
void MainWindow::write_path_polybius(QString path)
{
	QFile file(path_saved_paths);
	file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
	QString data = file.readAll().trimmed();
	if (!data.contains(prefix_polybius)) {
		data += "\n" + prefix_polybius + path;
	}
	QString data_new = "";
	QStringList lines = data.split("\n", QString::SkipEmptyParts);
	for (QString line : lines) {
		if (line.startsWith(prefix_polybius)) {
			line = prefix_polybius + path;
		}
		data_new += line + "\n";
	}
	data_new = data_new.trimmed();
	QTextStream out(&file);
	out << data_new;
	return;
}
void MainWindow::write_path_irene(QString path)
{
	QFile file(path_saved_paths);
	file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
	QString data = file.readAll().trimmed();
	if (!data.contains(prefix_irene)) {
		data += "\n" + prefix_irene + path;
	}
	QString data_new = "";
	QStringList lines = data.split("\n", QString::SkipEmptyParts);
	for (QString line : lines) {
		if (line.startsWith(prefix_irene)) {
			line = prefix_irene + path;
		}
		data_new += line + "\n";
	}
	data_new = data_new.trimmed();
	QTextStream out(&file);
	out << data_new;
	return;
}

void MainWindow::on_button_exe_polybius_clicked()
{
	QString path =
			QFileDialog::getOpenFileName(
				this,
				"Polybius.exe path",
				QDir::homePath(),
				"Executable (*.exe)" );
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
				"Executable (*.exe)" );
	if (path != nullptr)
		ui->lineEdit_exe_irene->setText(path);
}

void MainWindow::contextMenu_polybius(const QPoint& pos) {
	QMenu* menu = ui->console_polybius->createStandardContextMenu(pos);
	menu->addAction("Clea&r", this, &MainWindow::clear_polybius);
	menu->exec(ui->console_polybius->mapToGlobal(pos));
	delete menu;
}
void MainWindow::contextMenu_irene(const QPoint& pos) {
	QMenu* menu = ui->console_irene->createStandardContextMenu(pos);
	menu->addAction("Clea&r", this, &MainWindow::clear_irene);
	menu->exec(ui->console_irene->mapToGlobal(pos));
	delete menu;
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

void MainWindow::clear_polybius()
{
	ui->console_polybius->setPlainText("");
}
void MainWindow::clear_irene()
{
	ui->console_irene->setPlainText("");
}

void MainWindow::run_polybius()
{
	stop_polybius();
	clear_polybius();
	QString path = ui->lineEdit_exe_polybius->text();
	if (!QFile::exists(path) || !path.endsWith(".exe")) {
		QMessageBox message_error;
		message_error.setWindowTitle("WoW Console Runner");
		message_error.setText("There is no valid file at that path.");
		message_error.setIcon(QMessageBox::Warning);
		message_error.exec();
		return;
	}
	write_path_polybius(path);
	process_polybius->setProgram(path);
	process_polybius->setWorkingDirectory(get_dir_of_file(path));
	process_polybius->setReadChannel(QProcess::StandardOutput);
	process_polybius->start();
}
void MainWindow::run_irene()
{
	stop_irene();
	clear_irene();
	QString path = ui->lineEdit_exe_irene->text();
	if (!QFile::exists(path) || !path.endsWith(".exe")) {
		QMessageBox message_error;
		message_error.setWindowTitle("WoW Console Runner");
		message_error.setText("There is no valid file at that path.");
		message_error.setIcon(QMessageBox::Warning);
		message_error.exec();
		return;
	}
	write_path_irene(path);
	process_irene->setProgram(path);
	process_irene->setWorkingDirectory(get_dir_of_file(path));
	process_irene->setReadChannel(QProcess::StandardOutput);
	process_irene->start();
}

void MainWindow::stop_polybius()
{
	if (process_polybius->program() != nullptr) {
		process_polybius->close();
	}
}
void MainWindow::stop_irene()
{
	if (process_irene->program() != nullptr) {
		process_irene->close();
	}
}

void MainWindow::read_polybius()
{
	while (process_polybius->canReadLine()) {
		QString line = process_polybius->readLine();
		QString buffer = ui->console_polybius->toPlainText();
		if (static_cast<unsigned int>(buffer.size()) > chars_buffer) {
			int cutoff = buffer.indexOf("\n", chars_buffer/2);
			buffer = buffer.remove(0, cutoff + 1);
		}
		ui->console_polybius->setPlainText(buffer + line);
	}
}

void MainWindow::read_irene()
{
	while (process_irene->canReadLine()) {
		QString line = process_irene->readLine();
		QString buffer = ui->console_irene->toPlainText();
		if (static_cast<unsigned int>(buffer.size()) > chars_buffer) {
			int cutoff = buffer.indexOf("\n", chars_buffer/2);
			buffer = buffer.remove(0, cutoff + 1);
		}
		ui->console_irene->setPlainText(buffer + line);
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

QString MainWindow::get_dir_of_file(QString path)
{
	QDir dir = QDir(path);
	dir.cdUp();
	return dir.path();
}
